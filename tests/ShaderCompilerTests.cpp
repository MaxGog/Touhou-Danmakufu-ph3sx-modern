#include "ShaderCompiler.hpp"

#include <cstdlib>
#include <iostream>

int main() {
#if defined(_WIN32)
	constexpr renderer::Backend backend = renderer::Backend::D3D11;
#elif defined(__APPLE__)
	constexpr renderer::Backend backend = renderer::Backend::Metal;
#else
#error Shader compiler tests require a supported renderer backend.
#endif

	const renderer::ShaderSource source{
		"ph3sx_minimal_shader",
		R"(
			struct VertexOutput {
				float4 position : SV_Position;
				float2 uv : TEXCOORD0;
			};

			[shader("vertex")]
			VertexOutput vertexMain(uint vertexId : SV_VertexID) {
				VertexOutput output;
				output.position = float4(0.0, 0.0, 0.0, 1.0);
				output.uv = float2(0.0, 0.0);
				return output;
			}

			[shader("fragment")]
			float4 fragmentMain(VertexOutput input) : SV_Target0 {
				return float4(input.uv, 0.0, 1.0);
			}
		)",
		"vertexMain",
		"fragmentMain",
	};

	renderer::ShaderCompiler compiler;
	renderer::CompiledShader compiled;
	std::string error;
	if (!compiler.Compile(source, backend, compiled, error)) {
		std::cerr << "Slang compile failed: " << error << '\n';
		return EXIT_FAILURE;
	}
	if (compiled.vertex.empty() || compiled.fragment.empty() ||
		compiled.vertexEntryPoint != source.vertexEntryPoint ||
		compiled.fragmentEntryPoint != source.fragmentEntryPoint) {
		std::cerr << "Slang compiler returned incomplete shader stages.\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
