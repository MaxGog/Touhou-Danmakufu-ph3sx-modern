#include "MetalDevice.hpp"

#if defined(__APPLE__)
#include <SDL3/SDL_metal.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <array>
#include <cstring>

namespace renderer {
	struct MetalDevice::State {
		id<MTLDevice> device = nil;
		id<MTLCommandQueue> commandQueue = nil;
		CAMetalLayer* layer = nil;
		id<CAMetalDrawable> drawable = nil;
		id<MTLCommandBuffer> commandBuffer = nil;
		id<MTLRenderCommandEncoder> encoder = nil;
		id<MTLSamplerState> sampler = nil;
		id<MTLTexture> depthTexture = nil;
		Texture* activeRenderTarget = nullptr;
		id<MTLBuffer> boundIndexBuffer = nil;
		std::size_t boundIndexSize = 0;
		std::size_t boundIndexOffset = 0;
		IndexFormat boundIndexFormat = IndexFormat::UInt16;
		Pipeline* boundPipeline = nullptr;
		SDL_MetalView view = nullptr;
		Configuration configuration;
	};

	struct MetalDevice::Texture final : TextureResource {
		id<MTLTexture> texture = nil;
		id<MTLTexture> depthTexture = nil;
		bool shaderResource = false;
		bool renderTarget = false;
		bool cpuWritable = false;
		TextureFormat format = TextureFormat::RGBA8Unorm;
		std::uint32_t width = 0;
		std::uint32_t height = 0;
	};

	struct MetalDevice::Buffer final : BufferResource {
		id<MTLBuffer> buffer = nil;
		BufferUsage usage = BufferUsage::Vertex;
		std::size_t size = 0;
	};

	struct MetalDevice::Pipeline final : PipelineResource {
		id<MTLRenderPipelineState> pipelineState = nil;
		id<MTLDepthStencilState> depthState = nil;
		std::array<std::uint32_t, 31> vertexStrides{};
		TextureFormat colorFormat = TextureFormat::BGRA8Unorm;
	};

	namespace {
		MTLPixelFormat TextureMetalFormat(TextureFormat format) {
			switch (format) {
			case TextureFormat::R8Unorm: return MTLPixelFormatR8Unorm;
			case TextureFormat::RGBA8Unorm: return MTLPixelFormatRGBA8Unorm;
			case TextureFormat::BGRA8Unorm: return MTLPixelFormatBGRA8Unorm;
			case TextureFormat::RGBA16Float: return MTLPixelFormatRGBA16Float;
			case TextureFormat::Depth32Float: return MTLPixelFormatDepth32Float;
			default: return MTLPixelFormatInvalid;
			}
		}

		NSUInteger TextureBytesPerPixel(TextureFormat format) {
			switch (format) {
			case TextureFormat::R8Unorm: return 1;
			case TextureFormat::RGBA8Unorm:
			case TextureFormat::BGRA8Unorm:
			case TextureFormat::Depth32Float: return 4;
			case TextureFormat::RGBA16Float: return 8;
			default: return 0;
			}
		}

		MTLVertexFormat VertexMetalFormat(VertexFormat format) {
			switch (format) {
			case VertexFormat::Float2: return MTLVertexFormatFloat2;
			case VertexFormat::Float3: return MTLVertexFormatFloat3;
			case VertexFormat::Float4: return MTLVertexFormatFloat4;
			case VertexFormat::UInt8x4Unorm: return MTLVertexFormatUChar4Normalized;
			default: return MTLVertexFormatInvalid;
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

		std::string MetalError(NSError* error, const char* fallback) {
			const char* text = error.localizedDescription.UTF8String;
			return text ? text : fallback;
		}
	}

	MetalDevice::MetalDevice() : state_(std::make_unique<State>()) {}

	MetalDevice::~MetalDevice() {
		Shutdown();
	}

	bool MetalDevice::Initialize(SDL_Window* window, const Configuration& configuration) {
		Shutdown();
		if (!window || configuration.width <= 0 || configuration.height <= 0) {
			error_ = "A valid SDL Metal window and positive render dimensions are required.";
			return false;
		}
		if ((SDL_GetWindowFlags(window) & SDL_WINDOW_METAL) == 0) {
			error_ = "The SDL window must be created with SDL_WINDOW_METAL.";
			return false;
		}

		state_->device = MTLCreateSystemDefaultDevice();
		if (!state_->device) {
			error_ = "No Metal device is available.";
			return false;
		}
		state_->commandQueue = [state_->device newCommandQueue];
		if (!state_->commandQueue) {
			error_ = "Failed to create the Metal command queue.";
			Shutdown();
			return false;
		}

		state_->view = SDL_Metal_CreateView(window);
		if (!state_->view) {
			error_ = SDL_GetError();
			Shutdown();
			return false;
		}
		state_->layer = (__bridge CAMetalLayer*)SDL_Metal_GetLayer(state_->view);
		if (!state_->layer) {
			error_ = "SDL did not provide a CAMetalLayer.";
			Shutdown();
			return false;
		}
		state_->layer.device = state_->device;
		state_->layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
		state_->layer.framebufferOnly = YES;
		state_->layer.displaySyncEnabled = configuration.verticalSync;
		state_->configuration = configuration;
		MTLSamplerDescriptor* samplerDescription = [[MTLSamplerDescriptor alloc] init];
		samplerDescription.minFilter = MTLSamplerMinMagFilterLinear;
		samplerDescription.magFilter = MTLSamplerMinMagFilterLinear;
		samplerDescription.mipFilter = MTLSamplerMipFilterNotMipmapped;
		samplerDescription.sAddressMode = MTLSamplerAddressModeClampToEdge;
		samplerDescription.tAddressMode = MTLSamplerAddressModeClampToEdge;
		state_->sampler = [state_->device newSamplerStateWithDescriptor:samplerDescription];
		if (!state_->sampler) {
			error_ = "Failed to create the Metal sampler state.";
			Shutdown();
			return false;
		}
		return Resize(configuration.width, configuration.height);
	}

	bool MetalDevice::Resize(int width, int height) {
		if (!state_->device || !state_->commandQueue || !state_->layer ||
			!state_->view || width < 0 || height < 0) {
			error_ = "The Metal device must be initialized and dimensions cannot be negative.";
			return false;
		}
		if (width == 0 || height == 0)
			return true;
		state_->configuration.width = width;
		state_->configuration.height = height;
		state_->layer.drawableSize = CGSizeMake(width, height);
		MTLTextureDescriptor* depthDescription =
			[MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float
				width:width height:height mipmapped:NO];
		depthDescription.storageMode = MTLStorageModePrivate;
		depthDescription.usage = MTLTextureUsageRenderTarget;
		state_->depthTexture = [state_->device newTextureWithDescriptor:depthDescription];
		if (!state_->depthTexture) {
			error_ = "Failed to create the Metal depth texture.";
			return false;
		}
		error_.clear();
		return true;
	}

	bool MetalDevice::CreateRenderEncoder(bool clear, Color clearColor) {
		MTLRenderPassDescriptor* pass = [MTLRenderPassDescriptor renderPassDescriptor];
		const bool isSwapchain = state_->activeRenderTarget == nullptr;
		pass.colorAttachments[0].texture = isSwapchain
			? state_->drawable.texture : state_->activeRenderTarget->texture;
		pass.colorAttachments[0].loadAction = clear ? MTLLoadActionClear : MTLLoadActionLoad;
		pass.colorAttachments[0].storeAction = MTLStoreActionStore;
		pass.colorAttachments[0].clearColor = MTLClearColorMake(
			clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);
		pass.depthAttachment.texture = isSwapchain
			? state_->depthTexture : state_->activeRenderTarget->depthTexture;
		pass.depthAttachment.loadAction = clear ? MTLLoadActionClear : MTLLoadActionLoad;
		pass.depthAttachment.storeAction = MTLStoreActionStore;
		pass.depthAttachment.clearDepth = 1.0;
		state_->encoder = [state_->commandBuffer renderCommandEncoderWithDescriptor:pass];
		if (!state_->encoder) {
			error_ = "Failed to create the Metal render encoder.";
			return false;
		}
		return true;
	}

	bool MetalDevice::SetRenderTarget(TextureResource* resource, bool clear,
		Color clearColor)
	{
		if (!state_->encoder || !state_->commandBuffer) {
			error_ = "Metal SetRenderTarget requires an open frame.";
			return false;
		}
		auto* texture = resource ? dynamic_cast<Texture*>(resource) : nullptr;
		if (resource && (!texture || !texture->renderTarget ||
			texture->format == TextureFormat::Depth32Float)) {
			error_ = "The Metal render target must be a color texture created for rendering.";
			return false;
		}
		state_->activeRenderTarget = texture;
		[state_->encoder endEncoding];
		state_->encoder = nil;
		if (!CreateRenderEncoder(clear, clearColor)) {
			state_->activeRenderTarget = nullptr;
			return false;
		}
		state_->boundPipeline = nullptr;
		error_.clear();
		return true;
	}

	bool MetalDevice::BeginFrame(Color clearColor) {
		if (!IsInitialized() || state_->encoder) {
			error_ = "Metal BeginFrame requires an initialized device with no open frame.";
			return false;
		}
		state_->drawable = [state_->layer nextDrawable];
		if (!state_->drawable) {
			error_ = "CAMetalLayer did not provide a drawable.";
			return false;
		}
		state_->commandBuffer = [state_->commandQueue commandBuffer];
		if (!state_->commandBuffer) {
			error_ = "Failed to create a Metal command buffer.";
			state_->drawable = nil;
			return false;
		}

		state_->activeRenderTarget = nullptr;
		if (!CreateRenderEncoder(true, clearColor)) {
			state_->commandBuffer = nil;
			state_->drawable = nil;
			return false;
		}
		state_->boundPipeline = nullptr;
		state_->boundIndexBuffer = nil;
		state_->boundIndexSize = 0;
		state_->boundIndexOffset = 0;
		error_.clear();
		return true;
	}

	std::unique_ptr<TextureResource> MetalDevice::CreateTexture(
		const TextureDescription& description)
	{
		if (!IsInitialized() || description.width == 0 || description.height == 0 ||
			TextureMetalFormat(description.format) == MTLPixelFormatInvalid ||
			(description.format == TextureFormat::Depth32Float &&
				!description.renderTarget) ||
			(description.format != TextureFormat::Depth32Float &&
				!description.shaderResource && !description.renderTarget)) {
			error_ = "Invalid texture dimensions, format, or Metal usage.";
			return nullptr;
		}
		const NSUInteger minimumRowPitch =
			description.width * TextureBytesPerPixel(description.format);
		if (!description.initialData.empty() &&
			(description.format == TextureFormat::Depth32Float ||
				description.renderTarget ||
				description.rowPitch < minimumRowPitch ||
				description.initialData.size() / description.rowPitch < description.height)) {
			error_ = "Invalid initial data or row pitch for the Metal texture.";
			return nullptr;
		}

		MTLTextureDescriptor* nativeDescription =
			[MTLTextureDescriptor texture2DDescriptorWithPixelFormat:
				TextureMetalFormat(description.format)
				width:description.width height:description.height mipmapped:NO];
		nativeDescription.storageMode = (description.format == TextureFormat::Depth32Float ||
			description.renderTarget)
			? MTLStorageModePrivate : MTLStorageModeShared;
		nativeDescription.usage = MTLTextureUsageUnknown;
		if (description.shaderResource)
			nativeDescription.usage |= MTLTextureUsageShaderRead;
		if (description.renderTarget)
			nativeDescription.usage |= MTLTextureUsageRenderTarget;

		auto texture = std::make_unique<Texture>();
		texture->texture = [state_->device newTextureWithDescriptor:nativeDescription];
		if (!texture->texture) {
			error_ = "Metal failed to allocate the texture.";
			return nullptr;
		}
		if (description.renderTarget && description.format != TextureFormat::Depth32Float) {
			MTLTextureDescriptor* depthDescription =
				[MTLTextureDescriptor texture2DDescriptorWithPixelFormat:
					MTLPixelFormatDepth32Float
					width:description.width height:description.height mipmapped:NO];
			depthDescription.storageMode = MTLStorageModePrivate;
			depthDescription.usage = MTLTextureUsageRenderTarget;
			texture->depthTexture = [state_->device newTextureWithDescriptor:depthDescription];
			if (!texture->depthTexture) {
				error_ = "Metal failed to allocate the render-target depth texture.";
				return nullptr;
			}
		}
		if (!description.initialData.empty()) {
			[texture->texture replaceRegion:
				MTLRegionMake2D(0, 0, description.width, description.height)
				mipmapLevel:0 withBytes:description.initialData.data()
				bytesPerRow:description.rowPitch];
		}
		texture->shaderResource = description.shaderResource;
		texture->renderTarget = description.renderTarget;
		texture->cpuWritable = description.format != TextureFormat::Depth32Float &&
			!description.renderTarget;
		texture->format = description.format;
		texture->width = description.width;
		texture->height = description.height;
		error_.clear();
		return texture;
	}

	std::unique_ptr<BufferResource> MetalDevice::CreateBuffer(
		const BufferDescription& description)
	{
		const std::size_t byteCount = description.size != 0
			? description.size : description.initialData.size();
		if (!IsInitialized() || byteCount == 0 ||
			(!description.initialData.empty() && description.initialData.size() > byteCount) ||
			(description.usage == BufferUsage::Uniform &&
				(byteCount % 16 != 0 || byteCount > 65536))) {
			error_ = "Invalid Metal buffer size or initial data.";
			return nullptr;
		}
		auto buffer = std::make_unique<Buffer>();
		buffer->usage = description.usage;
		buffer->size = byteCount;
		buffer->buffer = [state_->device newBufferWithLength:byteCount
			options:MTLResourceStorageModeShared];
		if (!buffer->buffer) {
			error_ = "Metal failed to allocate the buffer.";
			return nullptr;
		}
		if (!description.initialData.empty())
			std::memcpy(buffer->buffer.contents, description.initialData.data(),
				description.initialData.size());
		error_.clear();
		return buffer;
	}

	bool MetalDevice::UpdateTexture(TextureResource* resource,
		std::span<const std::byte> data, std::size_t rowPitch)
	{
		auto* texture = dynamic_cast<Texture*>(resource);
		const std::size_t minimumPitch = texture
			? texture->width * TextureBytesPerPixel(texture->format) : 0;
		if (!IsInitialized() || !texture || !texture->cpuWritable || data.empty() ||
			rowPitch < minimumPitch || data.size() / rowPitch < texture->height) {
			error_ = "Invalid or non-writable Metal texture update.";
			return false;
		}
		[texture->texture replaceRegion:
			MTLRegionMake2D(0, 0, texture->width, texture->height)
			mipmapLevel:0 withBytes:data.data() bytesPerRow:rowPitch];
		error_.clear();
		return true;
	}

	bool MetalDevice::UpdateBuffer(BufferResource* resource, std::size_t offset,
		std::span<const std::byte> data)
	{
		auto* buffer = dynamic_cast<Buffer*>(resource);
		if (!IsInitialized() || !buffer || data.empty() || offset > buffer->size ||
			data.size() > buffer->size - offset ||
			(buffer->usage == BufferUsage::Uniform &&
				((offset % 16) != 0 || (data.size() % 16) != 0))) {
			error_ = "Invalid Metal buffer update range or resource.";
			return false;
		}
		std::memcpy(static_cast<std::byte*>(buffer->buffer.contents) + offset,
			data.data(), data.size());
		error_.clear();
		return true;
	}

	std::unique_ptr<PipelineResource> MetalDevice::CreatePipeline(
		const PipelineDescription& description)
	{
		if (!IsInitialized() || !description.shader ||
			description.shader->backend != Backend::Metal ||
			description.shader->vertex.empty() || description.shader->fragment.empty() ||
			description.shader->vertexEntryPoint.empty() ||
			description.shader->fragmentEntryPoint.empty() ||
			TextureMetalFormat(description.colorFormat) == MTLPixelFormatInvalid ||
			description.colorFormat == TextureFormat::Depth32Float) {
			error_ = "A compiled Metal vertex/fragment shader pair is required.";
			return nullptr;
		}

		NSError* nativeError = nil;
		NSString* vertexSource = [[NSString alloc] initWithBytes:description.shader->vertex.data()
			length:description.shader->vertex.size() encoding:NSUTF8StringEncoding];
		NSString* fragmentSource = [[NSString alloc] initWithBytes:description.shader->fragment.data()
			length:description.shader->fragment.size() encoding:NSUTF8StringEncoding];
		if (!vertexSource || !fragmentSource) {
			error_ = "Slang returned invalid UTF-8 Metal shader source.";
			return nullptr;
		}
		id<MTLLibrary> vertexLibrary = [state_->device newLibraryWithSource:vertexSource
			options:nil error:&nativeError];
		if (!vertexLibrary) {
			error_ = MetalError(nativeError, "Metal failed to compile the vertex shader.");
			return nullptr;
		}
		id<MTLLibrary> fragmentLibrary = [state_->device newLibraryWithSource:fragmentSource
			options:nil error:&nativeError];
		if (!fragmentLibrary) {
			error_ = MetalError(nativeError, "Metal failed to compile the fragment shader.");
			return nullptr;
		}
		NSString* vertexName = [NSString stringWithUTF8String:
			description.shader->vertexEntryPoint.c_str()];
		NSString* fragmentName = [NSString stringWithUTF8String:
			description.shader->fragmentEntryPoint.c_str()];
		id<MTLFunction> vertexFunction = [vertexLibrary newFunctionWithName:vertexName];
		id<MTLFunction> fragmentFunction = [fragmentLibrary newFunctionWithName:fragmentName];
		if (!vertexFunction || !fragmentFunction) {
			error_ = "Metal did not find the requested Slang entry-point functions.";
			return nullptr;
		}

		auto pipeline = std::make_unique<Pipeline>();
		pipeline->colorFormat = description.colorFormat;
		MTLVertexDescriptor* vertexDescriptor = [[MTLVertexDescriptor alloc] init];
		for (const VertexAttribute& attribute : description.vertexAttributes) {
			const MTLVertexFormat format = VertexMetalFormat(attribute.format);
			if (format == MTLVertexFormatInvalid || attribute.stride == 0 ||
				attribute.bufferSlot >= pipeline->vertexStrides.size() || attribute.location >= 31 ||
				attribute.offset > attribute.stride ||
				VertexFormatSize(attribute.format) > attribute.stride - attribute.offset ||
				(pipeline->vertexStrides[attribute.bufferSlot] != 0 &&
					pipeline->vertexStrides[attribute.bufferSlot] != attribute.stride)) {
				error_ = "Invalid Metal vertex input description.";
				return nullptr;
			}
			pipeline->vertexStrides[attribute.bufferSlot] = attribute.stride;
			vertexDescriptor.attributes[attribute.location].format = format;
			vertexDescriptor.attributes[attribute.location].offset = attribute.offset;
			vertexDescriptor.attributes[attribute.location].bufferIndex = attribute.bufferSlot;
			vertexDescriptor.layouts[attribute.bufferSlot].stride = attribute.stride;
			vertexDescriptor.layouts[attribute.bufferSlot].stepFunction =
				MTLVertexStepFunctionPerVertex;
		}

		MTLRenderPipelineDescriptor* nativeDescription =
			[[MTLRenderPipelineDescriptor alloc] init];
		nativeDescription.vertexFunction = vertexFunction;
		nativeDescription.fragmentFunction = fragmentFunction;
		nativeDescription.vertexDescriptor =
			description.vertexAttributes.empty() ? nil : vertexDescriptor;
		nativeDescription.colorAttachments[0].pixelFormat =
			TextureMetalFormat(description.colorFormat);
		if (description.alphaBlending) {
			auto colorAttachment = nativeDescription.colorAttachments[0];
			colorAttachment.blendingEnabled = YES;
			colorAttachment.rgbBlendOperation = MTLBlendOperationAdd;
			colorAttachment.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
			colorAttachment.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
			colorAttachment.alphaBlendOperation = MTLBlendOperationAdd;
			colorAttachment.sourceAlphaBlendFactor = MTLBlendFactorOne;
			colorAttachment.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
		}
		nativeDescription.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

		pipeline->pipelineState = [state_->device
			newRenderPipelineStateWithDescriptor:nativeDescription error:&nativeError];
		if (!pipeline->pipelineState) {
			error_ = MetalError(nativeError, "Metal failed to create the render pipeline.");
			return nullptr;
		}
		MTLDepthStencilDescriptor* depthDescription =
			[[MTLDepthStencilDescriptor alloc] init];
		depthDescription.depthCompareFunction = description.depthTest
			? MTLCompareFunctionLessEqual : MTLCompareFunctionAlways;
		depthDescription.depthWriteEnabled = description.depthWrite;
		pipeline->depthState = [state_->device
			newDepthStencilStateWithDescriptor:depthDescription];
		if (!pipeline->depthState) {
			error_ = "Metal failed to create the depth state.";
			return nullptr;
		}
		error_.clear();
		return pipeline;
	}

	bool MetalDevice::BindPipeline(PipelineResource* resource) {
		auto* pipeline = dynamic_cast<Pipeline*>(resource);
		if (!pipeline || !state_->encoder) {
			error_ = "Binding a Metal pipeline requires an active frame and a native pipeline.";
			return false;
		}
		const TextureFormat targetFormat = state_->activeRenderTarget
			? state_->activeRenderTarget->format : TextureFormat::BGRA8Unorm;
		if (pipeline->colorFormat != targetFormat) {
			error_ = "The Metal pipeline color format does not match the active render target.";
			return false;
		}
		state_->boundPipeline = pipeline;
		[state_->encoder setRenderPipelineState:pipeline->pipelineState];
		[state_->encoder setDepthStencilState:pipeline->depthState];
		error_.clear();
		return true;
	}

	bool MetalDevice::BindVertexBuffer(std::uint32_t slot, BufferResource* resource,
		std::uint32_t stride, std::uint32_t offset)
	{
		auto* buffer = dynamic_cast<Buffer*>(resource);
		if (!state_->encoder || !buffer || buffer->usage != BufferUsage::Vertex ||
			stride == 0 || offset >= buffer->size || stride > buffer->size - offset ||
			!state_->boundPipeline || slot >= state_->boundPipeline->vertexStrides.size() ||
			state_->boundPipeline->vertexStrides[slot] != stride) {
			error_ = "A vertex buffer, valid slot, and active Metal frame are required.";
			return false;
		}
		[state_->encoder setVertexBuffer:buffer->buffer offset:offset atIndex:slot];
		error_.clear();
		return true;
	}

	bool MetalDevice::BindIndexBuffer(BufferResource* resource, IndexFormat format,
		std::uint32_t offset)
	{
		auto* buffer = dynamic_cast<Buffer*>(resource);
		const std::size_t alignment = format == IndexFormat::UInt16 ? 2 : 4;
		if (!state_->encoder || !buffer || buffer->usage != BufferUsage::Index ||
			offset >= buffer->size || offset % alignment != 0 ||
			(format != IndexFormat::UInt16 && format != IndexFormat::UInt32)) {
			error_ = "A valid index buffer, aligned offset, and active Metal frame are required.";
			return false;
		}
		state_->boundIndexBuffer = buffer->buffer;
		state_->boundIndexSize = buffer->size;
		state_->boundIndexOffset = offset;
		state_->boundIndexFormat = format;
		error_.clear();
		return true;
	}

	bool MetalDevice::BindTexture(std::uint32_t slot, TextureResource* resource) {
		auto* texture = dynamic_cast<Texture*>(resource);
		if (!state_->encoder || !texture || !texture->shaderResource || slot >= 31) {
			error_ = "A shader-readable texture, valid slot, and active Metal frame are required.";
			return false;
		}
		[state_->encoder setFragmentTexture:texture->texture atIndex:slot];
		[state_->encoder setFragmentSamplerState:state_->sampler atIndex:slot];
		error_.clear();
		return true;
	}

	bool MetalDevice::BindUniformBuffer(std::uint32_t slot, BufferResource* resource) {
		auto* buffer = dynamic_cast<Buffer*>(resource);
		if (!state_->encoder || !buffer || buffer->usage != BufferUsage::Uniform ||
			slot >= 31) {
			error_ = "A uniform buffer, valid slot, and active Metal frame are required.";
			return false;
		}
		[state_->encoder setVertexBuffer:buffer->buffer offset:0 atIndex:slot];
		[state_->encoder setFragmentBuffer:buffer->buffer offset:0 atIndex:slot];
		error_.clear();
		return true;
	}

	namespace {
		MTLPrimitiveType MetalPrimitiveType(PrimitiveTopology topology) {
			switch (topology) {
			case PrimitiveTopology::PointList: return MTLPrimitiveTypePoint;
			case PrimitiveTopology::LineList: return MTLPrimitiveTypeLine;
			case PrimitiveTopology::LineStrip: return MTLPrimitiveTypeLineStrip;
			case PrimitiveTopology::TriangleList: return MTLPrimitiveTypeTriangle;
			case PrimitiveTopology::TriangleStrip: return MTLPrimitiveTypeTriangleStrip;
			default: return MTLPrimitiveTypeTriangle;
			}
		}
	}

	bool MetalDevice::Draw(PrimitiveTopology topology, std::uint32_t vertexCount,
		std::uint32_t firstVertex)
	{
		if (!state_->encoder || !state_->boundPipeline || vertexCount == 0) {
			error_ = "Metal Draw requires an active frame, pipeline, and non-zero vertex count.";
			return false;
		}
		if (topology < PrimitiveTopology::PointList ||
			topology > PrimitiveTopology::TriangleStrip) {
			error_ = "Unsupported Metal primitive topology.";
			return false;
		}
		[state_->encoder drawPrimitives:MetalPrimitiveType(topology)
			vertexStart:firstVertex vertexCount:vertexCount];
		error_.clear();
		return true;
	}

	bool MetalDevice::DrawIndexed(PrimitiveTopology topology, std::uint32_t indexCount,
		std::uint32_t firstIndex, std::int32_t baseVertex)
	{
		const std::size_t indexSize = state_->boundIndexFormat == IndexFormat::UInt16 ? 2 : 4;
		const std::size_t firstByte = static_cast<std::size_t>(firstIndex) * indexSize;
		const std::size_t indexBytes = static_cast<std::size_t>(indexCount) * indexSize;
		if (!state_->encoder || !state_->boundPipeline || !state_->boundIndexBuffer ||
			indexCount == 0 || firstByte > state_->boundIndexSize - state_->boundIndexOffset ||
			indexBytes > state_->boundIndexSize - state_->boundIndexOffset - firstByte) {
			error_ = "Metal DrawIndexed requires a valid frame, pipeline, and index range.";
			return false;
		}
		if (topology < PrimitiveTopology::PointList ||
			topology > PrimitiveTopology::TriangleStrip) {
			error_ = "Unsupported Metal primitive topology.";
			return false;
		}
		const MTLIndexType indexType = state_->boundIndexFormat == IndexFormat::UInt16
			? MTLIndexTypeUInt16 : MTLIndexTypeUInt32;
		[state_->encoder drawIndexedPrimitives:MetalPrimitiveType(topology)
			indexCount:indexCount indexType:indexType
			indexBuffer:state_->boundIndexBuffer
			indexBufferOffset:state_->boundIndexOffset + firstByte
			instanceCount:1 baseVertex:baseVertex baseInstance:0];
		error_.clear();
		return true;
	}

	bool MetalDevice::Present() {
		if (!IsInitialized() || !state_->encoder || !state_->commandBuffer || !state_->drawable) {
			error_ = "Metal Present requires an open frame.";
			return false;
		}
		[state_->encoder endEncoding];
		[state_->commandBuffer presentDrawable:state_->drawable];
		[state_->commandBuffer commit];
		state_->encoder = nil;
		state_->commandBuffer = nil;
		state_->drawable = nil;
		state_->activeRenderTarget = nullptr;
		state_->boundPipeline = nullptr;
		state_->boundIndexBuffer = nil;
		state_->boundIndexSize = 0;
		state_->boundIndexOffset = 0;
		error_.clear();
		return true;
	}

	void MetalDevice::Shutdown() {
		if (!state_)
			return;
		if (state_->encoder) {
			[state_->encoder endEncoding];
			state_->encoder = nil;
		}
		state_->commandBuffer = nil;
		state_->drawable = nil;
		state_->boundPipeline = nullptr;
		state_->boundIndexBuffer = nil;
		state_->boundIndexSize = 0;
		state_->boundIndexOffset = 0;
		state_->depthTexture = nil;
		state_->activeRenderTarget = nullptr;
		state_->sampler = nil;
		if (state_->view) {
			SDL_Metal_DestroyView(state_->view);
			state_->view = nullptr;
		}
		state_->layer = nil;
		state_->commandQueue = nil;
		state_->device = nil;
		state_->configuration = {};
	}

	bool MetalDevice::IsInitialized() const {
		return state_ && state_->device && state_->commandQueue && state_->layer &&
			state_->view && state_->depthTexture;
	}
}
#endif
