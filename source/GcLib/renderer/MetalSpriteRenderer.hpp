#pragma once

#include "Renderer.hpp"

#include <array>
#include <string_view>

namespace renderer {
	struct SpriteVertex {
		float position[4]{};
		std::uint32_t color = 0xffffffff;
		float texcoord[2]{};
	};

	class MetalSpriteRenderer {
		Device* device_ = nullptr;
		TextureFormat targetFormat_ = TextureFormat::BGRA8Unorm;
		std::array<std::unique_ptr<PipelineResource>, 4> pipelines_;
		std::unique_ptr<TextureResource> whiteTexture_;
		std::unique_ptr<BufferResource> vertexBuffer_;
		std::unique_ptr<BufferResource> indexBuffer_;
		std::unique_ptr<BufferResource> screenBuffer_;
		std::size_t vertexCapacity_ = 0;
		std::size_t indexCapacity_ = 0;
		bool inverseColor_ = false;
		bool useTexture_ = true;
		std::string error_;

		bool EnsureVertexCapacity(std::size_t required);
		bool EnsureIndexCapacity(std::size_t required);
	public:
		bool Initialize(Device& device,
			TextureFormat targetFormat = TextureFormat::BGRA8Unorm);
		bool ValidateTechnique(std::string_view name) const;
		bool SetTechnique(std::string_view name);
		bool Draw(std::span<const SpriteVertex> vertices,
			std::span<const std::uint16_t> indices, TextureResource* texture,
			std::uint32_t screenWidth, std::uint32_t screenHeight,
			bool alphaBlending = true,
			PrimitiveTopology topology = PrimitiveTopology::TriangleStrip);
		void Shutdown();
		const std::string& GetError() const { return error_; }
	};
}
