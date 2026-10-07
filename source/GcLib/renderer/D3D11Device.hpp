#pragma once

#include "Renderer.hpp"

#if defined(_WIN32)
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <array>

namespace renderer {
	class D3D11Device final : public Device {
		struct Texture;
		struct Buffer;
		struct Pipeline;
		Microsoft::WRL::ComPtr<ID3D11Device> device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain_;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget_;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture_;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthTarget_;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_;
		SDL_Window* window_ = nullptr;
		Configuration configuration_;
		std::string error_;
		bool frameOpen_ = false;
		Pipeline* boundPipeline_ = nullptr;
		Buffer* boundIndexBuffer_ = nullptr;
		std::size_t boundIndexOffset_ = 0;
		IndexFormat boundIndexFormat_ = IndexFormat::UInt16;

		bool CreateRenderTarget();
		bool CreateDepthTarget();
	public:
		~D3D11Device() override;

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
	};
}
#endif
