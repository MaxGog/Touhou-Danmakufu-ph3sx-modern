#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <optional>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace renderer {
	enum class Backend {
		D3D11,
		Metal,
	};

	struct Color {
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
		float alpha = 1.0f;
	};

	struct Configuration {
		int width = 0;
		int height = 0;
		bool verticalSync = true;
		bool debug = false;
	};

	enum class TextureFormat {
		R8Unorm,
		RGBA8Unorm,
		BGRA8Unorm,
		RGBA16Float,
		Depth32Float,
	};

	enum class BufferUsage {
		Vertex,
		Index,
		Uniform,
	};

	enum class IndexFormat {
		UInt16,
		UInt32,
	};

	enum class PrimitiveTopology {
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
	};

	enum class VertexFormat {
		Float2,
		Float3,
		Float4,
		UInt8x4Unorm,
	};

	struct TextureDescription {
		std::uint32_t width = 0;
		std::uint32_t height = 0;
		TextureFormat format = TextureFormat::RGBA8Unorm;
		bool shaderResource = true;
		bool renderTarget = false;
		std::span<const std::byte> initialData;
		std::size_t rowPitch = 0;
	};

	struct BufferDescription {
		BufferUsage usage = BufferUsage::Vertex;
		std::span<const std::byte> initialData;
		std::size_t size = 0;
	};

	struct VertexAttribute {
		std::uint32_t location = 0;
		std::uint32_t bufferSlot = 0;
		std::uint32_t offset = 0;
		std::uint32_t stride = 0;
		VertexFormat format = VertexFormat::Float2;
		std::string semantic = "TEXCOORD";
	};

	struct CompiledShader {
		Backend backend = Backend::D3D11;
		std::string vertexEntryPoint;
		std::string fragmentEntryPoint;
		std::vector<std::uint8_t> vertex;
		std::vector<std::uint8_t> fragment;
	};

	struct PipelineDescription {
		const CompiledShader* shader = nullptr;
		std::vector<VertexAttribute> vertexAttributes;
		TextureFormat colorFormat = TextureFormat::BGRA8Unorm;
		bool alphaBlending = false;
		bool depthTest = false;
		bool depthWrite = false;
	};

	class TextureResource {
	public:
		virtual ~TextureResource() = default;
	};

	class BufferResource {
	public:
		virtual ~BufferResource() = default;
	};

	class PipelineResource {
	public:
		virtual ~PipelineResource() = default;
	};

	class Device {
	public:
		virtual ~Device() = default;

		virtual bool Initialize(SDL_Window* window, const Configuration& configuration) = 0;
		virtual bool Resize(int width, int height) = 0;
		virtual bool BeginFrame(Color clearColor) = 0;
		virtual bool SetRenderTarget(TextureResource* texture, bool clear,
			Color clearColor = {}) = 0;
		virtual std::unique_ptr<TextureResource> CreateTexture(
			const TextureDescription& description) = 0;
		virtual std::unique_ptr<BufferResource> CreateBuffer(
			const BufferDescription& description) = 0;
		virtual bool UpdateTexture(TextureResource* texture,
			std::span<const std::byte> data, std::size_t rowPitch) = 0;
		virtual bool UpdateBuffer(BufferResource* buffer, std::size_t offset,
			std::span<const std::byte> data) = 0;
		virtual std::unique_ptr<PipelineResource> CreatePipeline(
			const PipelineDescription& description) = 0;
		virtual bool BindPipeline(PipelineResource* pipeline) = 0;
		virtual bool BindVertexBuffer(std::uint32_t slot, BufferResource* buffer,
			std::uint32_t stride, std::uint32_t offset = 0) = 0;
		virtual bool BindIndexBuffer(BufferResource* buffer, IndexFormat format,
			std::uint32_t offset = 0) = 0;
		virtual bool BindTexture(std::uint32_t slot, TextureResource* texture) = 0;
		virtual bool BindUniformBuffer(std::uint32_t slot, BufferResource* buffer) = 0;
		virtual bool Draw(PrimitiveTopology topology, std::uint32_t vertexCount,
			std::uint32_t firstVertex = 0) = 0;
		virtual bool DrawIndexed(PrimitiveTopology topology, std::uint32_t indexCount,
			std::uint32_t firstIndex = 0, std::int32_t baseVertex = 0) = 0;
		virtual bool Present() = 0;
		virtual void Shutdown() = 0;
		virtual bool IsInitialized() const = 0;
		virtual const std::string& GetError() const = 0;
	};

	std::unique_ptr<Device> CreateDevice(Backend backend, std::string& error);
	std::optional<SDL_WindowFlags> GetRequiredWindowFlags(Backend backend);
	const char* GetBackendName(Backend backend);
}
