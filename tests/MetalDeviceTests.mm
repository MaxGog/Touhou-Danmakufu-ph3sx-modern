#include "Renderer.hpp"
#include "MetalDevice.hpp"
#include "MetalSpriteRenderer.hpp"

#include <SDL3/SDL.h>

#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace {
	int Fail(const std::string& message) {
		std::cerr << message << '\n';
		return EXIT_FAILURE;
	}

	std::vector<std::uint8_t> Bytes(const char* source) {
		const auto* begin = reinterpret_cast<const std::uint8_t*>(source);
		return { begin, begin + std::strlen(source) };
	}
}

int main() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Skipping Metal device test: " << SDL_GetError() << '\n';
		return 77;
	}

	SDL_Window* window = SDL_CreateWindow(
		"ph3sx Metal device test", 64, 64, SDL_WINDOW_METAL | SDL_WINDOW_HIDDEN);
	if (!window) {
		std::cerr << "Skipping Metal device test: " << SDL_GetError() << '\n';
		SDL_Quit();
		return 77;
	}

	renderer::MetalDevice device;
	const renderer::Configuration configuration{64, 64, false, false};
	if (!device.Initialize(window, configuration)) {
		const std::string error = device.GetError();
		SDL_DestroyWindow(window);
		SDL_Quit();
		return Fail("Metal device initialization failed: " + error);
	}

	std::array<std::uint8_t, 4 * 4 * 4> pixels{};
	const renderer::TextureDescription textureDescription{
		4, 4, renderer::TextureFormat::RGBA8Unorm, true, false,
		std::as_bytes(std::span(pixels)), 4 * 4,
	};
	auto texture = device.CreateTexture(textureDescription);
	if (!texture)
		return Fail("Metal texture creation failed: " + device.GetError());
	if (!device.UpdateTexture(texture.get(), std::as_bytes(std::span(pixels)), 4 * 4))
		return Fail("Metal texture update failed: " + device.GetError());

	const std::array<std::uint16_t, 3> indices{0, 1, 2};
	const renderer::BufferDescription indexDescription{
		renderer::BufferUsage::Index, std::as_bytes(std::span(indices)), 0,
	};
	auto indexBuffer = device.CreateBuffer(indexDescription);
	if (!indexBuffer)
		return Fail("Metal index-buffer creation failed: " + device.GetError());
	if (!device.UpdateBuffer(indexBuffer.get(), 0, std::as_bytes(std::span(indices))))
		return Fail("Metal index-buffer update failed: " + device.GetError());

	const char* vertexSource =
		"#include <metal_stdlib>\n"
		"using namespace metal;\n"
		"vertex float4 vertexMain(uint vertexId [[vertex_id]]) {\n"
		"  const float2 p[3] = { float2(-1.0, -1.0), float2(3.0, -1.0), float2(-1.0, 3.0) };\n"
		"  return float4(p[vertexId], 0.0, 1.0);\n"
		"}\n";
	const char* fragmentSource =
		"#include <metal_stdlib>\n"
		"using namespace metal;\n"
		"fragment float4 fragmentMain() { return float4(0.2, 0.3, 0.4, 1.0); }\n";
	renderer::CompiledShader shader;
	shader.backend = renderer::Backend::Metal;
	shader.vertexEntryPoint = "vertexMain";
	shader.fragmentEntryPoint = "fragmentMain";
	shader.vertex = Bytes(vertexSource);
	shader.fragment = Bytes(fragmentSource);

	const renderer::PipelineDescription pipelineDescription{
		&shader, {}, renderer::TextureFormat::BGRA8Unorm, false, false, false,
	};
	auto pipeline = device.CreatePipeline(pipelineDescription);
	if (!pipeline)
		return Fail("Metal pipeline creation failed: " + device.GetError());

	const renderer::TextureDescription targetDescription{
		32, 32, renderer::TextureFormat::BGRA8Unorm, true, true, {}, 0,
	};
	auto target = device.CreateTexture(targetDescription);
	if (!target)
		return Fail("Metal render-target creation failed: " + device.GetError());

	if (!device.BeginFrame({0.0f, 0.0f, 0.0f, 1.0f}))
		return Fail("Metal BeginFrame failed: " + device.GetError());
	if (!device.BindPipeline(pipeline.get()))
		return Fail("Metal BindPipeline failed: " + device.GetError());
	if (!device.Draw(renderer::PrimitiveTopology::TriangleList, 3, 0))
		return Fail("Metal Draw failed: " + device.GetError());
	if (!device.BindIndexBuffer(indexBuffer.get(), renderer::IndexFormat::UInt16, 0))
		return Fail("Metal BindIndexBuffer failed: " + device.GetError());
	if (!device.DrawIndexed(renderer::PrimitiveTopology::TriangleList, 3, 0, 0))
		return Fail("Metal DrawIndexed failed: " + device.GetError());
	if (!device.Draw(renderer::PrimitiveTopology::TriangleStrip, 3, 0))
		return Fail("Metal triangle-strip draw failed: " + device.GetError());
	if (!device.SetRenderTarget(target.get(), true, {0.1f, 0.1f, 0.1f, 1.0f}))
		return Fail("Metal offscreen target bind failed: " + device.GetError());
	if (!device.BindPipeline(pipeline.get()) ||
		!device.Draw(renderer::PrimitiveTopology::TriangleList, 3, 0))
		return Fail("Metal offscreen draw failed: " + device.GetError());
	if (!device.SetRenderTarget(nullptr, false) ||
		!device.BindPipeline(pipeline.get()) ||
		!device.Draw(renderer::PrimitiveTopology::TriangleList, 3, 0))
		return Fail("Metal swapchain restoration failed: " + device.GetError());

	renderer::MetalSpriteRenderer sprites;
	if (!sprites.Initialize(device))
		return Fail("Metal sprite pipeline initialization failed: " + sprites.GetError());
	if (!sprites.ValidateTechnique("Render") ||
		!sprites.ValidateTechnique("RenderInv") ||
		!sprites.ValidateTechnique("RenderNoTexture") ||
		sprites.ValidateTechnique("UserD3DXTechnique") ||
		!sprites.SetTechnique("Render")) {
		return Fail("Metal sprite technique compatibility check failed.");
	}
	const std::array<renderer::SpriteVertex, 4> spriteVertices{{
		{{8.0f, 8.0f, 0.5f, 1.0f}, 0xffffffff, {0.0f, 0.0f}},
		{{56.0f, 8.0f, 0.5f, 1.0f}, 0xffffffff, {1.0f, 0.0f}},
		{{8.0f, 56.0f, 0.5f, 1.0f}, 0xffffffff, {0.0f, 1.0f}},
		{{56.0f, 56.0f, 0.5f, 1.0f}, 0xffffffff, {1.0f, 1.0f}},
	}};
	const std::array<std::uint16_t, 6> spriteIndices{0, 1, 2, 1, 2, 3};
	if (!sprites.Draw(spriteVertices, {}, nullptr, 64, 64, true))
		return Fail("Metal sprite strip draw failed: " + sprites.GetError());
	if (!sprites.SetTechnique("RenderInv") ||
		!sprites.Draw(spriteVertices, spriteIndices, texture.get(), 64, 64, true,
		renderer::PrimitiveTopology::TriangleList))
		return Fail("Metal sprite-list draw failed: " + sprites.GetError());
	if (!sprites.SetTechnique("RenderNoTexture") ||
		!sprites.Draw(spriteVertices, {}, texture.get(), 64, 64, false))
		return Fail("Metal textureless sprite draw failed: " + sprites.GetError());
	if (sprites.SetTechnique("UserD3DXTechnique"))
		return Fail("Metal accepted an unsupported custom D3DX technique.");
	sprites.Shutdown();
	if (!device.Present())
		return Fail("Metal Present failed: " + device.GetError());

	device.Shutdown();
	texture.reset();
	target.reset();
	indexBuffer.reset();
	pipeline.reset();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
