#include "MetalSpriteRenderer.hpp"

#include <algorithm>
#include <cstring>
#include <limits>

namespace renderer {
	namespace {
		constexpr const char* vertexSource = R"(
			#include <metal_stdlib>
			using namespace metal;

			struct SpriteInput {
				float4 position [[attribute(0)]];
				float4 color [[attribute(1)]];
				float2 texcoord [[attribute(2)]];
			};

			struct SpriteUniforms {
				float2 screenSize;
				float2 padding;
			};

			struct SpriteOutput {
				float4 position [[position]];
				float4 color;
				float2 texcoord;
			};

			vertex SpriteOutput spriteVertex(
				SpriteInput input [[stage_in]],
				constant SpriteUniforms& uniforms [[buffer(1)]])
			{
				SpriteOutput output;
				float inverseW = input.position.w == 0.0 ? 1.0 : 1.0 / input.position.w;
				float2 pixelPosition = input.position.xy * inverseW;
				output.position = float4(
					pixelPosition.x * (2.0 / uniforms.screenSize.x) - 1.0,
					1.0 - pixelPosition.y * (2.0 / uniforms.screenSize.y),
					input.position.z * inverseW,
					1.0);
				output.color = input.color.bgra;
				output.texcoord = input.texcoord;
				return output;
			}
		)";

		constexpr const char* fragmentSource = R"(
			#include <metal_stdlib>
			using namespace metal;

			struct SpriteOutput {
				float4 position [[position]];
				float4 color;
				float2 texcoord;
			};

			fragment float4 spriteFragment(
				SpriteOutput input [[stage_in]],
				texture2d<float> image [[texture(0)]],
				sampler imageSampler [[sampler(0)]])
			{
				return image.sample(imageSampler, input.texcoord) * input.color;
			}
		)";

		constexpr const char* inverseFragmentSource = R"(
			#include <metal_stdlib>
			using namespace metal;

			struct SpriteOutput {
				float4 position [[position]];
				float4 color;
				float2 texcoord;
			};

			fragment float4 spriteFragmentInverse(
				SpriteOutput input [[stage_in]],
				texture2d<float> image [[texture(0)]],
				sampler imageSampler [[sampler(0)]])
			{
				float4 color = image.sample(imageSampler, input.texcoord);
				color.rgb = 1.0 - color.rgb;
				return color * input.color;
			}
		)";

		CompiledShader MakeShader(const char* fragment, const char* fragmentEntry) {
			CompiledShader shader;
			shader.backend = Backend::Metal;
			shader.vertexEntryPoint = "spriteVertex";
			shader.fragmentEntryPoint = fragmentEntry;
			shader.vertex.assign(vertexSource, vertexSource + std::strlen(vertexSource));
			shader.fragment.assign(fragment, fragment + std::strlen(fragment));
			return shader;
		}

		std::size_t GrowCapacity(std::size_t current, std::size_t required) {
			std::size_t capacity = std::max<std::size_t>(current, 256);
			while (capacity < required) {
				if (capacity > std::numeric_limits<std::size_t>::max() / 2)
					return required;
				capacity *= 2;
			}
			return capacity;
		}
	}

	bool MetalSpriteRenderer::Initialize(Device& device, TextureFormat targetFormat) {
		Shutdown();
		device_ = &device;
		targetFormat_ = targetFormat;

		const std::array<std::uint8_t, 4> whitePixel{255, 255, 255, 255};
		const TextureDescription whiteDescription{
			1, 1, TextureFormat::RGBA8Unorm, true, false,
			std::as_bytes(std::span(whitePixel)), 4,
		};
		whiteTexture_ = device_->CreateTexture(whiteDescription);
		if (!whiteTexture_) {
			const std::string failure = device_->GetError();
			Shutdown();
			error_ = failure;
			return false;
		}

		const BufferDescription screenDescription{
			BufferUsage::Uniform, {}, 16,
		};
		screenBuffer_ = device_->CreateBuffer(screenDescription);
		if (!screenBuffer_) {
			const std::string failure = device_->GetError();
			Shutdown();
			error_ = failure;
			return false;
		}

		const auto attributes = std::vector<VertexAttribute>{
			{0, 0, offsetof(SpriteVertex, position), sizeof(SpriteVertex),
				VertexFormat::Float4, "POSITION"},
			{1, 0, offsetof(SpriteVertex, color), sizeof(SpriteVertex),
				VertexFormat::UInt8x4Unorm, "COLOR"},
			{2, 0, offsetof(SpriteVertex, texcoord), sizeof(SpriteVertex),
				VertexFormat::Float2, "TEXCOORD"},
		};
		for (std::size_t inverse = 0; inverse < 2; ++inverse) {
			CompiledShader shader = inverse == 0
				? MakeShader(fragmentSource, "spriteFragment")
				: MakeShader(inverseFragmentSource, "spriteFragmentInverse");
			for (std::size_t alpha = 0; alpha < 2; ++alpha) {
				const std::size_t slot = inverse * 2 + alpha;
				const PipelineDescription description{
					&shader, attributes, targetFormat_, alpha != 0, false, false,
				};
				pipelines_[slot] = device_->CreatePipeline(description);
				if (!pipelines_[slot]) {
					const std::string failure = device_->GetError();
					Shutdown();
					error_ = failure;
					return false;
				}
			}
		}

		error_.clear();
		return true;
	}

	bool MetalSpriteRenderer::EnsureVertexCapacity(std::size_t required) {
		if (required <= vertexCapacity_)
			return true;
		const std::size_t newCapacity = GrowCapacity(vertexCapacity_, required);
		if (newCapacity > std::numeric_limits<std::size_t>::max() / sizeof(SpriteVertex)) {
			error_ = "Sprite vertex buffer size overflow.";
			return false;
		}
		const BufferDescription description{
			BufferUsage::Vertex, {}, newCapacity * sizeof(SpriteVertex),
		};
		auto buffer = device_->CreateBuffer(description);
		if (!buffer) {
			error_ = device_->GetError();
			return false;
		}
		vertexBuffer_ = std::move(buffer);
		vertexCapacity_ = newCapacity;
		return true;
	}

	bool MetalSpriteRenderer::EnsureIndexCapacity(std::size_t required) {
		if (required <= indexCapacity_)
			return true;
		const std::size_t newCapacity = GrowCapacity(indexCapacity_, required);
		if (newCapacity > std::numeric_limits<std::size_t>::max() / sizeof(std::uint16_t)) {
			error_ = "Sprite index buffer size overflow.";
			return false;
		}
		const BufferDescription description{
			BufferUsage::Index, {}, newCapacity * sizeof(std::uint16_t),
		};
		auto buffer = device_->CreateBuffer(description);
		if (!buffer) {
			error_ = device_->GetError();
			return false;
		}
		indexBuffer_ = std::move(buffer);
		indexCapacity_ = newCapacity;
		return true;
	}

	bool MetalSpriteRenderer::ValidateTechnique(std::string_view name) const {
		return name == "Render" || name == "RenderInv" || name == "RenderNoTexture";
	}

	bool MetalSpriteRenderer::SetTechnique(std::string_view name) {
		if (!ValidateTechnique(name)) {
			error_ = "Unsupported Metal sprite technique. Supported built-ins are Render, RenderInv, and RenderNoTexture.";
			return false;
		}
		inverseColor_ = name == "RenderInv";
		useTexture_ = name != "RenderNoTexture";
		error_.clear();
		return true;
	}

	bool MetalSpriteRenderer::Draw(std::span<const SpriteVertex> vertices,
		std::span<const std::uint16_t> indices, TextureResource* texture,
		std::uint32_t screenWidth, std::uint32_t screenHeight,
		bool alphaBlending, PrimitiveTopology topology)
	{
		if (!device_ || !device_->IsInitialized() || vertices.empty() ||
			screenWidth == 0 || screenHeight == 0 ||
			(topology != PrimitiveTopology::TriangleList &&
				topology != PrimitiveTopology::TriangleStrip)) {
			error_ = "Metal sprite draw requires initialized resources, positive viewport dimensions, and triangle topology.";
			return false;
		}
		if (!indices.empty() && topology != PrimitiveTopology::TriangleList) {
			error_ = "Indexed Metal sprites must use triangle-list topology.";
			return false;
		}
		if (vertices.size() > std::numeric_limits<std::uint32_t>::max() ||
			indices.size() > std::numeric_limits<std::uint32_t>::max()) {
			error_ = "Metal sprite draw exceeds the supported vertex or index count.";
			return false;
		}
		for (const std::uint16_t index : indices) {
			if (index >= vertices.size()) {
				error_ = "Metal sprite index refers to a vertex outside the supplied vertex span.";
				return false;
			}
		}
		if (!EnsureVertexCapacity(vertices.size()) ||
			(!indices.empty() && !EnsureIndexCapacity(indices.size())))
			return false;

		struct alignas(16) ScreenUniform {
			float dimensions[2];
			float padding[2]{};
		};
		const ScreenUniform screen{{static_cast<float>(screenWidth),
			static_cast<float>(screenHeight)}, {0.0f, 0.0f}};
		if (!device_->UpdateBuffer(vertexBuffer_.get(), 0, std::as_bytes(vertices)) ||
			!device_->UpdateBuffer(screenBuffer_.get(), 0, std::as_bytes(std::span(&screen, 1)))) {
			error_ = device_->GetError();
			return false;
		}
		if (!indices.empty() &&
			!device_->UpdateBuffer(indexBuffer_.get(), 0, std::as_bytes(indices))) {
			error_ = device_->GetError();
			return false;
		}

		const std::size_t pipelineIndex =
			(inverseColor_ ? 2 : 0) + (alphaBlending ? 1 : 0);
		if (!device_->BindPipeline(pipelines_[pipelineIndex].get()) ||
			!device_->BindVertexBuffer(0, vertexBuffer_.get(), sizeof(SpriteVertex)) ||
			!device_->BindUniformBuffer(1, screenBuffer_.get()) ||
			!device_->BindTexture(0, useTexture_ && texture ? texture : whiteTexture_.get())) {
			error_ = device_->GetError();
			return false;
		}

		const bool drawn = indices.empty()
			? device_->Draw(topology, static_cast<std::uint32_t>(vertices.size()))
			: device_->BindIndexBuffer(indexBuffer_.get(), IndexFormat::UInt16) &&
				device_->DrawIndexed(topology, static_cast<std::uint32_t>(indices.size()));
		if (!drawn) {
			error_ = device_->GetError();
			return false;
		}
		error_.clear();
		return true;
	}

	void MetalSpriteRenderer::Shutdown() {
		vertexBuffer_.reset();
		indexBuffer_.reset();
		screenBuffer_.reset();
		whiteTexture_.reset();
		for (auto& pipeline : pipelines_)
			pipeline.reset();
		vertexCapacity_ = 0;
		indexCapacity_ = 0;
		inverseColor_ = false;
		useTexture_ = true;
		device_ = nullptr;
		error_.clear();
	}
}
