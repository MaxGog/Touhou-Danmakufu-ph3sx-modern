#pragma once

#include "Renderer.hpp"

#include <string>

namespace renderer {
	struct ShaderSource {
		std::string moduleName;
		std::string source;
		std::string vertexEntryPoint;
		std::string fragmentEntryPoint;
	};

	class ShaderCompiler {
	public:
		bool Compile(const ShaderSource& source, Backend backend,
			CompiledShader& result, std::string& error) const;
	};
}
