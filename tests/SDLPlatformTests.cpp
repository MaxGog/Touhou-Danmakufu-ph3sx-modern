#include "SDLPlatform.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {
	void Require(bool condition, const std::string& message) {
		if (!condition)
			throw std::runtime_error(message);
	}

	void Push(SDL_Event event) {
		if (!SDL_PushEvent(&event))
			throw std::runtime_error(SDL_GetError());
	}
}

int main() {
	platform::SDLPlatform sdl;
	Require(sdl.Initialize(), "SDL initialization failed: " + sdl.GetError());

	platform::SDLWindow window;
	Require(window.Create("SDL platform tests", 320, 240, SDL_WINDOW_HIDDEN),
		"Window creation failed: " + window.GetError());
	Require(window.SetTitle("SDL platform input tests"),
		"Setting the SDL window title failed: " + window.GetError());
	Require(window.SetVisible(false),
		"Hiding the SDL window failed: " + window.GetError());

	platform::SDLInput input;
	Require(input.Initialize(window.GetNativeWindow()),
		"Input initialization failed: " + input.GetError());

	const SDL_WindowID windowId = SDL_GetWindowID(window.GetNativeWindow());
	auto update = [&]() {
		Require(sdl.PollEvents(window, input), "Event polling failed: " + sdl.GetError());
		Require(input.Update(), "Input update failed: " + input.GetError());
	};

	SDL_Event event{};
	event.type = SDL_EVENT_KEY_DOWN;
	event.key.windowID = windowId;
	event.key.scancode = SDL_SCANCODE_Z;
	Push(event);
	update();
	Require(input.GetKeyState(0x2c) == platform::KeyState::Push,
		"Legacy DirectInput Z scan code did not map to SDL Z.");
	update();
	Require(input.GetKeyState(0x2c) == platform::KeyState::Hold,
		"Pressed key did not transition from Push to Hold.");

	event = {};
	event.type = SDL_EVENT_KEY_UP;
	event.key.windowID = windowId;
	event.key.scancode = SDL_SCANCODE_Z;
	Push(event);
	update();
	Require(input.GetKeyState(0x2c) == platform::KeyState::Pull,
		"Released key did not transition to Pull.");
	update();
	Require(input.GetKeyState(0x2c) == platform::KeyState::Free,
		"Released key did not transition from Pull to Free.");

	event = {};
	event.type = SDL_EVENT_KEY_DOWN;
	event.key.windowID = windowId;
	event.key.scancode = SDL_SCANCODE_X;
	Push(event);
	event = {};
	event.type = SDL_EVENT_KEY_UP;
	event.key.windowID = windowId;
	event.key.scancode = SDL_SCANCODE_X;
	Push(event);
	update();
	Require(input.GetKeyState(0x2d) == platform::KeyState::Push,
		"A press and release between logic updates lost its press edge.");
	update();
	Require(input.GetKeyState(0x2d) == platform::KeyState::Pull,
		"A queued release did not follow its queued press.");

	event = {};
	event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
	event.button.windowID = windowId;
	event.button.button = SDL_BUTTON_LEFT;
	Push(event);
	event = {};
	event.type = SDL_EVENT_MOUSE_MOTION;
	event.motion.windowID = windowId;
	event.motion.x = 10.0f;
	event.motion.y = 12.0f;
	event.motion.xrel = 2.5f;
	event.motion.yrel = -1.0f;
	Push(event);
	event = {};
	event.type = SDL_EVENT_MOUSE_WHEEL;
	event.wheel.windowID = windowId;
	event.wheel.y = 1.25f;
	Push(event);
	update();
	Require(input.GetMouseState(0) == platform::KeyState::Push,
		"Mouse button did not transition to Push.");
	Require(input.GetMouseMoveX() == 2.5f && input.GetMouseMoveY() == -1.0f,
		"Mouse motion delta was not preserved.");
	Require(input.GetMouseMoveZ() == 1.25f, "Precise wheel delta was not preserved.");
	update();
	Require(input.GetMouseMoveX() == 0.0f && input.GetMouseMoveZ() == 0.0f,
		"Mouse motion and wheel deltas were not cleared after a frame.");

	event = {};
	event.type = SDL_EVENT_MOUSE_BUTTON_UP;
	event.button.windowID = windowId;
	event.button.button = SDL_BUTTON_LEFT;
	Push(event);
	update();
	Require(input.GetMouseState(0) == platform::KeyState::Pull,
		"Released mouse button did not transition to Pull.");

	event = {};
	event.type = SDL_EVENT_KEY_DOWN;
	event.key.windowID = windowId;
	event.key.scancode = SDL_SCANCODE_LEFT;
	Push(event);
	update();
	Require(input.GetKeyState(0xcb) == platform::KeyState::Push,
		"Legacy DirectInput Left scan code did not map to SDL Left.");

	event = {};
	event.type = SDL_EVENT_WINDOW_FOCUS_LOST;
	event.window.windowID = windowId;
	Push(event);
	update();
	Require(input.GetKeyState(0xcb) == platform::KeyState::Pull,
		"Focus loss did not release a held key.");

	event = {};
	event.type = SDL_EVENT_QUIT;
	Push(event);
	update();
	Require(!sdl.IsRunning() && !window.IsOpen(),
		"SDL quit event did not stop the platform loop.");

	input.Shutdown();
	window.Destroy();
	sdl.Shutdown();

	std::cout << "SDL platform input tests passed\n";
	return 0;
}
