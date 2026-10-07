#include "Renderer.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace {
	void Require(bool condition, const char* message) {
		if (!condition) {
			std::cerr << message << '\n';
			std::exit(EXIT_FAILURE);
		}
	}
}

int main() {
	std::string error;
#if defined(_WIN32)
	const renderer::Backend available = renderer::Backend::D3D11;
	const renderer::Backend unavailable = renderer::Backend::Metal;
#elif defined(__APPLE__)
	const renderer::Backend available = renderer::Backend::Metal;
	const renderer::Backend unavailable = renderer::Backend::D3D11;
#else
#error Renderer factory tests require a supported native renderer platform.
#endif

	auto device = renderer::CreateDevice(available, error);
	Require(device != nullptr, "The platform renderer backend was not created.");
	Require(error.empty(), "The factory left an error for a successful backend selection.");
	Require(!device->IsInitialized(), "A newly created device must not yet be initialized.");
	device->Shutdown();
	Require(renderer::GetRequiredWindowFlags(available).has_value(),
		"The active backend did not provide its required SDL window flags.");

	auto unsupported = renderer::CreateDevice(unavailable, error);
	Require(unsupported == nullptr, "An unavailable renderer backend was returned.");
	Require(!error.empty(), "The unavailable backend did not report an error.");
	Require(!renderer::GetRequiredWindowFlags(unavailable).has_value(),
		"An unavailable backend returned SDL window flags.");
	Require(std::string(renderer::GetBackendName(available)) != "Unknown",
		"The active renderer backend has no display name.");
	return EXIT_SUCCESS;
}
