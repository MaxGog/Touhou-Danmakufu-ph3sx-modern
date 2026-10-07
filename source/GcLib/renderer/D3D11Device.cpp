#include "D3D11Device.hpp"

#if defined(_WIN32)
#include <cstdio>
#include <limits>
#include <string>
#include <vector>

namespace renderer {
	namespace {
		std::string HResultMessage(const char* operation, HRESULT result) {
			char message[160]{};
			std::snprintf(message, sizeof(message), "%s failed (HRESULT 0x%08lX).",
				operation, static_cast<unsigned long>(result));
			return message;
		}

		DXGI_FORMAT TextureDxgiFormat(TextureFormat format, bool depthResource = false) {
			switch (format) {
			case TextureFormat::R8Unorm: return DXGI_FORMAT_R8_UNORM;
			case TextureFormat::RGBA8Unorm: return DXGI_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::BGRA8Unorm: return DXGI_FORMAT_B8G8R8A8_UNORM;
			case TextureFormat::RGBA16Float: return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case TextureFormat::Depth32Float:
				return depthResource ? DXGI_FORMAT_R32_TYPELESS : DXGI_FORMAT_UNKNOWN;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}

		std::uint32_t TextureBytesPerPixel(TextureFormat format) {
			switch (format) {
			case TextureFormat::R8Unorm: return 1;
			case TextureFormat::RGBA8Unorm:
			case TextureFormat::BGRA8Unorm:
			case TextureFormat::Depth32Float: return 4;
			case TextureFormat::RGBA16Float: return 8;
			default: return 0;
			}
		}

		DXGI_FORMAT VertexDxgiFormat(VertexFormat format) {
			switch (format) {
			case VertexFormat::Float2: return DXGI_FORMAT_R32G32_FLOAT;
			case VertexFormat::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
			case VertexFormat::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case VertexFormat::UInt8x4Unorm: return DXGI_FORMAT_R8G8B8A8_UNORM;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}

		std::uint32_t VertexFormatSize(VertexFormat format) {
			switch (format) {
			case VertexFormat::Float2: return 8;
			case VertexFormat::Float3: return 12;
			case VertexFormat::Float4: return 16;
			case VertexFormat::UInt8x4Unorm: return 4;
			default: return 0;
			}
		}
	}

	struct D3D11Device::Texture final : TextureResource {
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResource;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> renderTargetDepth;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthTarget;
		TextureFormat format = TextureFormat::RGBA8Unorm;
		UINT width = 0;
		UINT height = 0;
	};

	struct D3D11Device::Buffer final : BufferResource {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		BufferUsage usage = BufferUsage::Vertex;
		std::size_t size = 0;
	};

	struct D3D11Device::Pipeline final : PipelineResource {
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> fragmentShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;
		std::array<std::uint32_t, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT> vertexStrides{};
	};

	D3D11Device::~D3D11Device() {
		Shutdown();
	}

	bool D3D11Device::Initialize(SDL_Window* window, const Configuration& configuration) {
		Shutdown();
		if (!window || configuration.width <= 0 || configuration.height <= 0) {
			error_ = "A valid SDL window and positive render dimensions are required.";
			return false;
		}

		const SDL_PropertiesID properties = SDL_GetWindowProperties(window);
		if (!properties) {
			error_ = SDL_GetError();
			return false;
		}
		HWND nativeWindow = static_cast<HWND>(
			SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
		if (!nativeWindow) {
			error_ = "SDL did not expose a Win32 HWND for the renderer window.";
			return false;
		}

		UINT deviceFlags = configuration.debug ? D3D11_CREATE_DEVICE_DEBUG : 0;
		const D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		D3D_FEATURE_LEVEL featureLevel{};
		HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			deviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
			&device_, &featureLevel, &context_);
		if (result == E_INVALIDARG) {
			result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
				deviceFlags, &featureLevels[1], 1, D3D11_SDK_VERSION,
				&device_, &featureLevel, &context_);
		}
		if (FAILED(result)) {
			error_ = HResultMessage("D3D11CreateDevice", result);
			Shutdown();
			return false;
		}

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		Microsoft::WRL::ComPtr<IDXGIFactory2> factory;
		result = device_.As(&dxgiDevice);
		if (SUCCEEDED(result))
			result = dxgiDevice->GetAdapter(&adapter);
		if (SUCCEEDED(result))
			result = adapter->GetParent(IID_PPV_ARGS(&factory));
		if (FAILED(result)) {
			error_ = HResultMessage("Creating the DXGI factory", result);
			Shutdown();
			return false;
		}

		DXGI_SWAP_CHAIN_DESC1 description{};
		description.Width = static_cast<UINT>(configuration.width);
		description.Height = static_cast<UINT>(configuration.height);
		description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.SampleDesc.Count = 1;
		description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		description.BufferCount = 2;
		description.Scaling = DXGI_SCALING_STRETCH;
		description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		description.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		result = factory->CreateSwapChainForHwnd(device_.Get(), nativeWindow,
			&description, nullptr, nullptr, &swapChain_);
		if (FAILED(result)) {
			error_ = HResultMessage("IDXGIFactory2::CreateSwapChainForHwnd", result);
			Shutdown();
			return false;
		}

		factory->MakeWindowAssociation(nativeWindow, DXGI_MWA_NO_ALT_ENTER);
		window_ = window;
		configuration_ = configuration;
		if (!CreateRenderTarget()) {
			Shutdown();
			return false;
		}
		if (!CreateDepthTarget()) {
			Shutdown();
			return false;
		}
		D3D11_SAMPLER_DESC samplerDescription{};
		samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
		result = device_->CreateSamplerState(&samplerDescription, &sampler_);
		if (FAILED(result)) {
			error_ = HResultMessage("ID3D11Device::CreateSamplerState", result);
			Shutdown();
			return false;
		}
		error_.clear();
		return true;
	}

	bool D3D11Device::SetRenderTarget(TextureResource* resource, bool clear,
		Color clearColor)
	{
		if (!frameOpen_) {
			error_ = "D3D11 SetRenderTarget requires an open frame.";
			return false;
		}
		auto* texture = resource ? dynamic_cast<Texture*>(resource) : nullptr;
		if (resource && (!texture || !texture->renderTarget ||
			texture->format == TextureFormat::Depth32Float)) {
			error_ = "The D3D11 render target must be a color texture created for rendering.";
			return false;
		}
		ID3D11RenderTargetView* target =
			texture ? texture->renderTarget.Get() : renderTarget_.Get();
		ID3D11DepthStencilView* depth =
			texture ? texture->renderTargetDepth.Get() : depthTarget_.Get();
		context_->OMSetRenderTargets(1, &target, depth);
		const UINT width = texture ? texture->width : configuration_.width;
		const UINT height = texture ? texture->height : configuration_.height;
		D3D11_VIEWPORT viewport{};
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MaxDepth = 1.0f;
		context_->RSSetViewports(1, &viewport);
		if (clear) {
			const float color[] = {
				clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha,
			};
			context_->ClearRenderTargetView(target, color);
			context_->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
		error_.clear();
		return true;
	}

	bool D3D11Device::CreateRenderTarget() {
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT result = swapChain_->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		if (SUCCEEDED(result))
			result = device_->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget_);
		if (FAILED(result)) {
			error_ = HResultMessage("Creating the D3D11 render target", result);
			return false;
		}
		return true;
	}

	bool D3D11Device::CreateDepthTarget() {
		D3D11_TEXTURE2D_DESC description{};
		description.Width = static_cast<UINT>(configuration_.width);
		description.Height = static_cast<UINT>(configuration_.height);
		description.MipLevels = 1;
		description.ArraySize = 1;
		description.Format = DXGI_FORMAT_D32_FLOAT;
		description.SampleDesc.Count = 1;
		description.Usage = D3D11_USAGE_DEFAULT;
		description.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		HRESULT result = device_->CreateTexture2D(&description, nullptr, &depthTexture_);
		if (SUCCEEDED(result))
			result = device_->CreateDepthStencilView(depthTexture_.Get(), nullptr, &depthTarget_);
		if (FAILED(result)) {
			error_ = HResultMessage("Creating the D3D11 depth target", result);
			return false;
		}
		return true;
	}

	bool D3D11Device::Resize(int width, int height) {
		if (!device_ || !context_ || !swapChain_ || width < 0 || height < 0) {
			error_ = "The D3D11 device must be initialized and dimensions cannot be negative.";
			return false;
		}
		if (width == 0 || height == 0)
			return true;

		context_->OMSetRenderTargets(0, nullptr, nullptr);
		renderTarget_.Reset();
		depthTarget_.Reset();
		depthTexture_.Reset();
		HRESULT result = swapChain_->ResizeBuffers(0, static_cast<UINT>(width),
			static_cast<UINT>(height), DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(result)) {
			error_ = HResultMessage("IDXGISwapChain::ResizeBuffers", result);
			return false;
		}
		configuration_.width = width;
		configuration_.height = height;
		return CreateRenderTarget() && CreateDepthTarget();
	}

	bool D3D11Device::BeginFrame(Color clearColor) {
		if (!IsInitialized() || frameOpen_) {
			error_ = "D3D11 BeginFrame requires an initialized device with no open frame.";
			return false;
		}
		const float color[] = {
			clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha,
		};
		context_->OMSetRenderTargets(1, renderTarget_.GetAddressOf(), depthTarget_.Get());
		D3D11_VIEWPORT viewport{};
		viewport.Width = static_cast<float>(configuration_.width);
		viewport.Height = static_cast<float>(configuration_.height);
		viewport.MaxDepth = 1.0f;
		context_->RSSetViewports(1, &viewport);
		context_->ClearRenderTargetView(renderTarget_.Get(), color);
		context_->ClearDepthStencilView(depthTarget_.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		boundPipeline_ = nullptr;
		boundIndexBuffer_ = nullptr;
		frameOpen_ = true;
		error_.clear();
		return true;
	}

	std::unique_ptr<TextureResource> D3D11Device::CreateTexture(
		const TextureDescription& description)
	{
		if (!IsInitialized() || description.width == 0 || description.height == 0 ||
			(description.format != TextureFormat::Depth32Float &&
				TextureDxgiFormat(description.format) == DXGI_FORMAT_UNKNOWN) ||
			(description.renderTarget && description.format == TextureFormat::Depth32Float) ||
			(description.format == TextureFormat::Depth32Float && !description.renderTarget)) {
			error_ = "Invalid texture dimensions, format, or usage.";
			return nullptr;
		}
		const std::size_t minimumRowPitch =
			static_cast<std::size_t>(description.width) * TextureBytesPerPixel(description.format);
		if (!description.initialData.empty() &&
			(description.rowPitch < minimumRowPitch ||
				description.rowPitch > std::numeric_limits<UINT>::max() ||
				description.initialData.size() / description.rowPitch < description.height)) {
			error_ = "Texture row pitch is smaller than one pixel row.";
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC nativeDescription{};
		nativeDescription.Width = description.width;
		nativeDescription.Height = description.height;
		nativeDescription.MipLevels = 1;
		nativeDescription.ArraySize = 1;
		const bool isDepth = description.format == TextureFormat::Depth32Float;
		nativeDescription.Format = TextureDxgiFormat(description.format, isDepth);
		nativeDescription.SampleDesc.Count = 1;
		nativeDescription.Usage = D3D11_USAGE_DEFAULT;
		if (description.shaderResource)
			nativeDescription.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		if (isDepth)
			nativeDescription.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		else if (description.renderTarget)
			nativeDescription.BindFlags |= D3D11_BIND_RENDER_TARGET;

		D3D11_SUBRESOURCE_DATA initialData{};
		const D3D11_SUBRESOURCE_DATA* initialDataPointer = nullptr;
		if (!description.initialData.empty()) {
			initialData.pSysMem = description.initialData.data();
			initialData.SysMemPitch = static_cast<UINT>(description.rowPitch);
			initialDataPointer = &initialData;
		}

		auto texture = std::make_unique<Texture>();
		texture->format = description.format;
		texture->width = description.width;
		texture->height = description.height;
		HRESULT result = device_->CreateTexture2D(
			&nativeDescription, initialDataPointer, &texture->texture);
		if (SUCCEEDED(result) && description.shaderResource) {
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription{};
			viewDescription.Format = isDepth ? DXGI_FORMAT_R32_FLOAT : nativeDescription.Format;
			viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDescription.Texture2D.MipLevels = 1;
			result = device_->CreateShaderResourceView(
				texture->texture.Get(), &viewDescription, &texture->shaderResource);
		}
		if (SUCCEEDED(result) && isDepth) {
			D3D11_DEPTH_STENCIL_VIEW_DESC viewDescription{};
			viewDescription.Format = DXGI_FORMAT_D32_FLOAT;
			viewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			result = device_->CreateDepthStencilView(
				texture->texture.Get(), &viewDescription, &texture->depthTarget);
		}
		if (SUCCEEDED(result) && description.renderTarget && !isDepth)
			result = device_->CreateRenderTargetView(
				texture->texture.Get(), nullptr, &texture->renderTarget);
		if (SUCCEEDED(result) && description.renderTarget && !isDepth) {
			D3D11_TEXTURE2D_DESC depthDescription{};
			depthDescription.Width = description.width;
			depthDescription.Height = description.height;
			depthDescription.MipLevels = 1;
			depthDescription.ArraySize = 1;
			depthDescription.Format = DXGI_FORMAT_D32_FLOAT;
			depthDescription.SampleDesc.Count = 1;
			depthDescription.Usage = D3D11_USAGE_DEFAULT;
			depthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			result = device_->CreateTexture2D(&depthDescription, nullptr,
				&texture->depthTexture);
			if (SUCCEEDED(result))
				result = device_->CreateDepthStencilView(texture->depthTexture.Get(),
					nullptr, &texture->renderTargetDepth);
		}
		if (FAILED(result)) {
			error_ = HResultMessage("Creating a D3D11 texture", result);
			return nullptr;
		}
		error_.clear();
		return texture;
	}

	std::unique_ptr<BufferResource> D3D11Device::CreateBuffer(
		const BufferDescription& description)
	{
		const std::size_t byteCount = description.size != 0
			? description.size : description.initialData.size();
		if (!IsInitialized() || byteCount == 0 ||
			byteCount > std::numeric_limits<UINT>::max() ||
			(description.usage == BufferUsage::Uniform &&
				(byteCount % 16 != 0 || byteCount > 65536)) ||
			(!description.initialData.empty() && description.initialData.size() > byteCount)) {
			error_ = "Invalid D3D11 buffer size or initial data.";
			return nullptr;
		}

		D3D11_BUFFER_DESC nativeDescription{};
		nativeDescription.ByteWidth = static_cast<UINT>(byteCount);
		switch (description.usage) {
		case BufferUsage::Vertex: nativeDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER; break;
		case BufferUsage::Index: nativeDescription.BindFlags = D3D11_BIND_INDEX_BUFFER; break;
		case BufferUsage::Uniform: nativeDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER; break;
		}
		D3D11_SUBRESOURCE_DATA initialData{};
		const D3D11_SUBRESOURCE_DATA* initialDataPointer = nullptr;
		if (!description.initialData.empty()) {
			initialData.pSysMem = description.initialData.data();
			initialDataPointer = &initialData;
		}

		auto buffer = std::make_unique<Buffer>();
		buffer->usage = description.usage;
		buffer->size = byteCount;
		const HRESULT result = device_->CreateBuffer(
			&nativeDescription, initialDataPointer, &buffer->buffer);
		if (FAILED(result)) {
			error_ = HResultMessage("ID3D11Device::CreateBuffer", result);
			return nullptr;
		}
		error_.clear();
		return buffer;
	}

	bool D3D11Device::UpdateTexture(TextureResource* resource,
		std::span<const std::byte> data, std::size_t rowPitch)
	{
		auto* texture = dynamic_cast<Texture*>(resource);
		const std::size_t minPitch = texture
			? texture->width * TextureBytesPerPixel(texture->format) : 0;
		if (!IsInitialized() || !texture || data.empty() ||
			texture->format == TextureFormat::Depth32Float || rowPitch < minPitch ||
			rowPitch > std::numeric_limits<UINT>::max() ||
			data.size() / rowPitch < texture->height) {
			error_ = "Invalid D3D11 texture update data, pitch, or resource.";
			return false;
		}
		context_->UpdateSubresource(texture->texture.Get(), 0, nullptr, data.data(),
			static_cast<UINT>(rowPitch), 0);
		error_.clear();
		return true;
	}

	bool D3D11Device::UpdateBuffer(BufferResource* resource, std::size_t offset,
		std::span<const std::byte> data)
	{
		auto* buffer = dynamic_cast<Buffer*>(resource);
		if (!IsInitialized() || !buffer || data.empty() || offset > buffer->size ||
			data.size() > buffer->size - offset ||
			(buffer->usage == BufferUsage::Uniform &&
				((offset % 16) != 0 || (data.size() % 16) != 0))) {
			error_ = "Invalid D3D11 buffer update range or resource.";
			return false;
		}
		D3D11_BOX region{};
		region.left = static_cast<UINT>(offset);
		region.right = static_cast<UINT>(offset + data.size());
		region.top = 0;
		region.bottom = 1;
		region.front = 0;
		region.back = 1;
		context_->UpdateSubresource(buffer->buffer.Get(), 0, &region, data.data(), 0, 0);
		error_.clear();
		return true;
	}

	std::unique_ptr<PipelineResource> D3D11Device::CreatePipeline(
		const PipelineDescription& description)
	{
		if (!IsInitialized() || !description.shader ||
			description.shader->backend != Backend::D3D11 ||
			description.shader->vertex.empty() || description.shader->fragment.empty()) {
			error_ = "A compiled D3D11 vertex/fragment shader pair is required.";
			return nullptr;
		}

		auto pipeline = std::make_unique<Pipeline>();
		HRESULT result = device_->CreateVertexShader(
			description.shader->vertex.data(), description.shader->vertex.size(),
			nullptr, &pipeline->vertexShader);
		if (SUCCEEDED(result))
			result = device_->CreatePixelShader(
				description.shader->fragment.data(), description.shader->fragment.size(),
				nullptr, &pipeline->fragmentShader);
		if (FAILED(result)) {
			error_ = HResultMessage("Creating D3D11 shader stages", result);
			return nullptr;
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
		inputElements.reserve(description.vertexAttributes.size());
		for (const VertexAttribute& attribute : description.vertexAttributes) {
			const DXGI_FORMAT format = VertexDxgiFormat(attribute.format);
			if (format == DXGI_FORMAT_UNKNOWN || attribute.semantic.empty() ||
				attribute.stride == 0 || attribute.bufferSlot >= pipeline->vertexStrides.size() ||
				attribute.offset > attribute.stride ||
				VertexFormatSize(attribute.format) > attribute.stride - attribute.offset ||
				(pipeline->vertexStrides[attribute.bufferSlot] != 0 &&
					pipeline->vertexStrides[attribute.bufferSlot] != attribute.stride)) {
				error_ = "Invalid D3D11 vertex input description.";
				return nullptr;
			}
			pipeline->vertexStrides[attribute.bufferSlot] = attribute.stride;
			inputElements.push_back({
				attribute.semantic.c_str(),
				attribute.location,
				format,
				attribute.bufferSlot,
				attribute.offset,
				D3D11_INPUT_PER_VERTEX_DATA,
				0,
			});
		}
		if (!inputElements.empty()) {
			result = device_->CreateInputLayout(inputElements.data(),
				static_cast<UINT>(inputElements.size()),
				description.shader->vertex.data(), description.shader->vertex.size(),
				&pipeline->inputLayout);
			if (FAILED(result)) {
				error_ = HResultMessage("Creating the D3D11 input layout", result);
				return nullptr;
			}
		}

		D3D11_BLEND_DESC blendDescription{};
		auto& blend = blendDescription.RenderTarget[0];
		blend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		if (description.alphaBlending) {
			blend.BlendEnable = TRUE;
			blend.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend.BlendOp = D3D11_BLEND_OP_ADD;
			blend.SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		}
		result = device_->CreateBlendState(&blendDescription, &pipeline->blendState);
		if (FAILED(result)) {
			error_ = HResultMessage("Creating the D3D11 blend state", result);
			return nullptr;
		}

		D3D11_DEPTH_STENCIL_DESC depthDescription{};
		depthDescription.DepthEnable = description.depthTest;
		depthDescription.DepthWriteMask = description.depthWrite
			? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		result = device_->CreateDepthStencilState(&depthDescription, &pipeline->depthState);
		if (FAILED(result)) {
			error_ = HResultMessage("Creating the D3D11 depth state", result);
			return nullptr;
		}
		error_.clear();
		return pipeline;
	}

	bool D3D11Device::BindPipeline(PipelineResource* resource) {
		auto* pipeline = dynamic_cast<Pipeline*>(resource);
		if (!pipeline || !frameOpen_) {
			error_ = "Binding a D3D11 pipeline requires an active frame and a native pipeline.";
			return false;
		}
		boundPipeline_ = pipeline;
		context_->IASetInputLayout(pipeline->inputLayout.Get());
		context_->VSSetShader(pipeline->vertexShader.Get(), nullptr, 0);
		context_->PSSetShader(pipeline->fragmentShader.Get(), nullptr, 0);
		context_->OMSetBlendState(pipeline->blendState.Get(), nullptr, 0xffffffff);
		context_->OMSetDepthStencilState(pipeline->depthState.Get(), 0);
		error_.clear();
		return true;
	}

	bool D3D11Device::BindVertexBuffer(std::uint32_t slot, BufferResource* resource,
		std::uint32_t stride, std::uint32_t offset)
	{
		auto* buffer = dynamic_cast<Buffer*>(resource);
		if (!frameOpen_ || !buffer || buffer->usage != BufferUsage::Vertex ||
			stride == 0 || offset >= buffer->size || stride > buffer->size - offset ||
			!boundPipeline_ || slot >= boundPipeline_->vertexStrides.size() ||
			boundPipeline_->vertexStrides[slot] != stride) {
			error_ = "A vertex buffer, non-zero stride, and active frame are required.";
			return false;
		}
		ID3D11Buffer* nativeBuffer = buffer->buffer.Get();
		context_->IASetVertexBuffers(slot, 1, &nativeBuffer, &stride, &offset);
		error_.clear();
		return true;
	}

	bool D3D11Device::BindIndexBuffer(BufferResource* resource, IndexFormat format,
		std::uint32_t offset)
	{
		auto* buffer = dynamic_cast<Buffer*>(resource);
		const UINT alignment = format == IndexFormat::UInt16 ? 2u : 4u;
		if (!frameOpen_ || !buffer || buffer->usage != BufferUsage::Index ||
			offset >= buffer->size || offset % alignment != 0 ||
			(format != IndexFormat::UInt16 && format != IndexFormat::UInt32)) {
			error_ = "A valid index buffer, aligned offset, and active frame are required.";
			return false;
		}
		const DXGI_FORMAT nativeFormat = format == IndexFormat::UInt16
			? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		context_->IASetIndexBuffer(buffer->buffer.Get(), nativeFormat, offset);
		boundIndexBuffer_ = buffer;
		boundIndexOffset_ = offset;
		boundIndexFormat_ = format;
		error_.clear();
		return true;
	}

	bool D3D11Device::BindTexture(std::uint32_t slot, TextureResource* resource) {
		auto* texture = dynamic_cast<Texture*>(resource);
		if (!frameOpen_ || !texture || !texture->shaderResource) {
			error_ = "A shader-readable texture and active frame are required.";
			return false;
		}
		ID3D11ShaderResourceView* view = texture->shaderResource.Get();
		ID3D11SamplerState* sampler = sampler_.Get();
		context_->PSSetShaderResources(slot, 1, &view);
		context_->PSSetSamplers(slot, 1, &sampler);
		error_.clear();
		return true;
	}

	bool D3D11Device::BindUniformBuffer(std::uint32_t slot, BufferResource* resource) {
		auto* buffer = dynamic_cast<Buffer*>(resource);
		if (!frameOpen_ || !buffer || buffer->usage != BufferUsage::Uniform || slot >= 14) {
			error_ = "A uniform buffer, valid slot, and active frame are required.";
			return false;
		}
		ID3D11Buffer* nativeBuffer = buffer->buffer.Get();
		context_->VSSetConstantBuffers(slot, 1, &nativeBuffer);
		context_->PSSetConstantBuffers(slot, 1, &nativeBuffer);
		error_.clear();
		return true;
	}

	bool D3D11Device::Draw(PrimitiveTopology topology, std::uint32_t vertexCount,
		std::uint32_t firstVertex)
	{
		if (!frameOpen_ || !boundPipeline_ || vertexCount == 0) {
			error_ = "D3D11 Draw requires an active frame, pipeline, and non-zero vertex count.";
			return false;
		}
		D3D11_PRIMITIVE_TOPOLOGY nativeTopology;
		switch (topology) {
		case PrimitiveTopology::PointList:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case PrimitiveTopology::LineList:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case PrimitiveTopology::LineStrip:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;
		case PrimitiveTopology::TriangleList:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		case PrimitiveTopology::TriangleStrip:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;
		default:
			error_ = "Unsupported D3D11 primitive topology.";
			return false;
		}
		context_->IASetPrimitiveTopology(nativeTopology);
		context_->Draw(vertexCount, firstVertex);
		error_.clear();
		return true;
	}

	bool D3D11Device::DrawIndexed(PrimitiveTopology topology, std::uint32_t indexCount,
		std::uint32_t firstIndex, std::int32_t baseVertex)
	{
		const std::size_t indexSize = boundIndexFormat_ == IndexFormat::UInt16 ? 2 : 4;
		const std::size_t firstByte = static_cast<std::size_t>(firstIndex) * indexSize;
		const std::size_t indexBytes = static_cast<std::size_t>(indexCount) * indexSize;
		if (!frameOpen_ || !boundPipeline_ || !boundIndexBuffer_ || indexCount == 0 ||
			firstByte > boundIndexBuffer_->size - boundIndexOffset_ ||
			indexBytes > boundIndexBuffer_->size - boundIndexOffset_ - firstByte) {
			error_ = "D3D11 DrawIndexed requires a valid frame, pipeline, and index range.";
			return false;
		}
		D3D11_PRIMITIVE_TOPOLOGY nativeTopology;
		switch (topology) {
		case PrimitiveTopology::PointList:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case PrimitiveTopology::LineList:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case PrimitiveTopology::LineStrip:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;
		case PrimitiveTopology::TriangleList:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		case PrimitiveTopology::TriangleStrip:
			nativeTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;
		default:
			error_ = "Unsupported D3D11 primitive topology.";
			return false;
		}
		context_->IASetPrimitiveTopology(nativeTopology);
		context_->DrawIndexed(indexCount, firstIndex, baseVertex);
		error_.clear();
		return true;
	}

	bool D3D11Device::Present() {
		if (!IsInitialized() || !frameOpen_) {
			error_ = "D3D11 Present requires an open frame.";
			return false;
		}
		frameOpen_ = false;
		const HRESULT result = swapChain_->Present(configuration_.verticalSync ? 1 : 0, 0);
		if (FAILED(result)) {
			error_ = HResultMessage("IDXGISwapChain::Present", result);
			return false;
		}
		error_.clear();
		return true;
	}

	void D3D11Device::Shutdown() {
		frameOpen_ = false;
		boundPipeline_ = nullptr;
		boundIndexBuffer_ = nullptr;
		renderTarget_.Reset();
		depthTarget_.Reset();
		depthTexture_.Reset();
		sampler_.Reset();
		swapChain_.Reset();
		if (context_)
			context_->ClearState();
		context_.Reset();
		device_.Reset();
		window_ = nullptr;
	}

	bool D3D11Device::IsInitialized() const {
		return device_ && context_ && swapChain_ && renderTarget_ && depthTarget_;
	}
}
#endif
