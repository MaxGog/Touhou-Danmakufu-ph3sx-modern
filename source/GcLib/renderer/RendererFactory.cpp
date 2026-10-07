#include "Renderer.hpp"

#if defined(_WIN32)
#include "D3D11Device.hpp"
#elif defined(__APPLE__)
#include "MetalDevice.hpp"
#endif

namespace renderer {
	std::unique_ptr<Device> CreateDevice(Backend backend, std::string& error) {
		error.clear();
		switch (backend) {
#if defined(_WIN32)
		case Backend::D3D11:
			return std::make_unique<D3D11Device>();
#endif
#if defined(__APPLE__)
		case Backend::Metal:
			return std::make_unique<MetalDevice>();
#endif
		default:
			error = "The requested renderer backend is not available on this platform.";
			return nullptr;
		}
	}

	std::optional<SDL_WindowFlags> GetRequiredWindowFlags(Backend backend) {
		switch (backend) {
#if defined(_WIN32)
		case Backend::D3D11:
			return SDL_WindowFlags{};
#endif
#if defined(__APPLE__)
		case Backend::Metal:
			return SDL_WINDOW_METAL;
#endif
		default:
			return std::nullopt;
		}
	}

	const char* GetBackendName(Backend backend) {
		switch (backend) {
		case Backend::D3D11: return "Direct3D 11";
		case Backend::Metal: return "Metal";
		default: return "Unknown";
		}
	}
}
