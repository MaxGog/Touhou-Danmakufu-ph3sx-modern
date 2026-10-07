#include "ShaderCompiler.hpp"

#include <slang-com-ptr.h>
#include <slang.h>

namespace renderer {
	namespace {
		std::string BlobText(slang::IBlob* blob) {
			if (!blob || !blob->getBufferPointer() || blob->getBufferSize() == 0)
				return {};
			return std::string(
				static_cast<const char*>(blob->getBufferPointer()),
				blob->getBufferSize());
		}

		bool GetEntryPointCode(slang::IComponentType* program, SlangInt entryPoint,
			std::vector<std::uint8_t>& output, slang::IBlob** diagnostics)
		{
			Slang::ComPtr<slang::IBlob> code;
			if (SLANG_FAILED(program->getEntryPointCode(
				entryPoint, 0, code.writeRef(), diagnostics)))
				return false;
			const auto* begin = static_cast<const std::uint8_t*>(code->getBufferPointer());
			output.assign(begin, begin + code->getBufferSize());
			return true;
		}
	}

	bool ShaderCompiler::Compile(const ShaderSource& source, Backend backend,
		CompiledShader& result, std::string& error) const
	{
		result = {};
		error.clear();
		if (source.moduleName.empty() || source.source.empty() ||
			source.vertexEntryPoint.empty() || source.fragmentEntryPoint.empty()) {
			error = "Shader module source and both entry-point names are required.";
			return false;
		}

		Slang::ComPtr<slang::IGlobalSession> globalSession;
		SlangResult status = slang_createGlobalSession(
			SLANG_API_VERSION, globalSession.writeRef());
		if (SLANG_FAILED(status)) {
			error = "Failed to create the Slang global session.";
			return false;
		}

		slang::TargetDesc target{};
		const char* profileName = nullptr;
		switch (backend) {
		case Backend::D3D11:
			target.format = SLANG_DXBC;
			profileName = "sm_5_0";
			break;
		case Backend::Metal:
			target.format = SLANG_METAL;
			profileName = "metal";
			break;
		default:
			error = "The requested shader target is not supported.";
			return false;
		}
		target.profile = globalSession->findProfile(profileName);
		if (target.profile == SLANG_PROFILE_UNKNOWN) {
			error = std::string("Slang does not provide the required profile: ") + profileName;
			return false;
		}

		slang::SessionDesc sessionDescription{};
		sessionDescription.targetCount = 1;
		sessionDescription.targets = &target;
		Slang::ComPtr<slang::ISession> session;
		status = globalSession->createSession(sessionDescription, session.writeRef());
		if (SLANG_FAILED(status)) {
			error = "Failed to create the Slang compilation session.";
			return false;
		}

		Slang::ComPtr<slang::IBlob> diagnostics;
		Slang::ComPtr<slang::IModule> module;
		module = session->loadModuleFromSourceString(
			source.moduleName.c_str(),
			source.moduleName.c_str(),
			source.source.c_str(),
			diagnostics.writeRef());
		if (!module) {
			error = BlobText(diagnostics.get());
			if (error.empty())
				error = "Slang failed to load the shader module.";
			return false;
		}

		Slang::ComPtr<slang::IEntryPoint> vertexEntryPoint;
		status = module->findAndCheckEntryPoint(source.vertexEntryPoint.c_str(),
			SLANG_STAGE_VERTEX, vertexEntryPoint.writeRef(), diagnostics.writeRef());
		if (SLANG_FAILED(status)) {
			error = BlobText(diagnostics.get());
			if (error.empty())
				error = "Slang failed to resolve the vertex shader entry point.";
			return false;
		}

		Slang::ComPtr<slang::IEntryPoint> fragmentEntryPoint;
		status = module->findAndCheckEntryPoint(source.fragmentEntryPoint.c_str(),
			SLANG_STAGE_FRAGMENT, fragmentEntryPoint.writeRef(), diagnostics.writeRef());
		if (SLANG_FAILED(status)) {
			error = BlobText(diagnostics.get());
			if (error.empty())
				error = "Slang failed to resolve the fragment shader entry point.";
			return false;
		}

		slang::IComponentType* components[] = {
			module.get(), vertexEntryPoint.get(), fragmentEntryPoint.get(),
		};
		Slang::ComPtr<slang::IComponentType> compositeProgram;
		status = session->createCompositeComponentType(
			components, 3, compositeProgram.writeRef(), diagnostics.writeRef());
		if (SLANG_FAILED(status)) {
			error = BlobText(diagnostics.get());
			if (error.empty())
				error = "Slang failed to combine the shader entry points.";
			return false;
		}

		Slang::ComPtr<slang::IComponentType> linkedProgram;
		status = compositeProgram->link(linkedProgram.writeRef(), diagnostics.writeRef());
		if (SLANG_FAILED(status)) {
			error = BlobText(diagnostics.get());
			if (error.empty())
				error = "Slang failed to link the shader program.";
			return false;
		}

		CompiledShader compiled{};
		compiled.backend = backend;
		compiled.vertexEntryPoint = source.vertexEntryPoint;
		compiled.fragmentEntryPoint = source.fragmentEntryPoint;
		if (!GetEntryPointCode(linkedProgram.get(), 0, compiled.vertex, diagnostics.writeRef()) ||
			!GetEntryPointCode(linkedProgram.get(), 1, compiled.fragment, diagnostics.writeRef())) {
			error = BlobText(diagnostics.get());
			if (error.empty())
				error = "Slang failed to emit the compiled shader program.";
			return false;
		}
		result = std::move(compiled);
		error.clear();
		return true;
	}
}
