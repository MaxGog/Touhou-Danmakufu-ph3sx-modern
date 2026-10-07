#pragma once

#include "Renderer.hpp"

#if defined(__APPLE__)
namespace renderer {
	class MetalDevice final : public Device {
		struct State;
		struct Texture;
		struct Buffer;
		struct Pipeline;
		std::unique_ptr<State> state_;
		std::string error_;
	public:
		MetalDevice();
		~MetalDevice() override;

		bool Initialize(SDL_Window* window, const Configuration& configuration) override;
		bool Resize(int width, int height) override;
		bool BeginFrame(Color clearColor) override;
		bool SetRenderTarget(TextureResource* texture, bool clear,
			Color clearColor = {}) override;
		std::unique_ptr<TextureResource> CreateTexture(
			const TextureDescription& description) override;
		std::unique_ptr<BufferResource> CreateBuffer(
			const BufferDescription& description) override;
		bool UpdateTexture(TextureResource* texture,
			std::span<const std::byte> data, std::size_t rowPitch) override;
		bool UpdateBuffer(BufferResource* buffer, std::size_t offset,
			std::span<const std::byte> data) override;
		std::unique_ptr<PipelineResource> CreatePipeline(
			const PipelineDescription& description) override;
		bool BindPipeline(PipelineResource* pipeline) override;
		bool BindVertexBuffer(std::uint32_t slot, BufferResource* buffer,
			std::uint32_t stride, std::uint32_t offset) override;
		bool BindIndexBuffer(BufferResource* buffer, IndexFormat format,
			std::uint32_t offset) override;
		bool BindTexture(std::uint32_t slot, TextureResource* texture) override;
		bool BindUniformBuffer(std::uint32_t slot, BufferResource* buffer) override;
		bool Draw(PrimitiveTopology topology, std::uint32_t vertexCount,
			std::uint32_t firstVertex) override;
		bool DrawIndexed(PrimitiveTopology topology, std::uint32_t indexCount,
			std::uint32_t firstIndex, std::int32_t baseVertex) override;
		bool Present() override;
		void Shutdown() override;
		bool IsInitialized() const override;
		const std::string& GetError() const override { return error_; }
	private:
		bool CreateRenderEncoder(bool clear, Color clearColor);
	};
}
#endif
