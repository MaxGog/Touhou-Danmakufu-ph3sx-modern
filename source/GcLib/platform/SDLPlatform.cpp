#include "SDLPlatform.hpp"

namespace platform {
	namespace {
		KeyState UpdateState(bool pressed, KeyState previous) {
			if (pressed)
				return previous == KeyState::Free || previous == KeyState::Pull
					? KeyState::Push : KeyState::Hold;
			return previous == KeyState::Push || previous == KeyState::Hold
				? KeyState::Pull : KeyState::Free;
		}

		int MouseButtonIndex(uint8_t button) {
			switch (button) {
			case SDL_BUTTON_LEFT: return 0;
			case SDL_BUTTON_RIGHT: return 1;
			case SDL_BUTTON_MIDDLE: return 2;
			case SDL_BUTTON_X1: return 3;
			default: return -1;
			}
		}
	}

	SDLWindow::~SDLWindow() {
		Destroy();
	}

	bool SDLWindow::Create(const std::string& title, int width, int height, SDL_WindowFlags flags) {
		if (window_)
			Destroy();
		if (width <= 0 || height <= 0) {
			error_ = "Window dimensions must be positive.";
			return false;
		}

		window_ = SDL_CreateWindow(title.c_str(), width, height, flags);
		if (!window_) {
			error_ = SDL_GetError();
			return false;
		}

		error_.clear();
		width_ = width;
		height_ = height;
		open_ = true;
		focused_ = SDL_GetWindowFlags(window_) & SDL_WINDOW_INPUT_FOCUS;
		return true;
	}

	bool SDLWindow::WrapNative(void* nativeWindow, int width, int height) {
		if (window_)
			Destroy();
		if (!nativeWindow || width <= 0 || height <= 0) {
			error_ = "A native window and positive dimensions are required.";
			return false;
		}

#if defined(_WIN32)
		const char* nativeWindowProperty = SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER;
#elif defined(__APPLE__)
		const char* nativeWindowProperty = SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER;
#else
		error_ = "Wrapping a native window is not implemented for this platform.";
		return false;
#endif
		const SDL_PropertiesID properties = SDL_CreateProperties();
		if (!properties) {
			error_ = SDL_GetError();
			return false;
		}
		if (!SDL_SetPointerProperty(properties, nativeWindowProperty, nativeWindow)) {
			error_ = SDL_GetError();
			SDL_DestroyProperties(properties);
			return false;
		}
		if (!SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER,
			SDL_WINDOW_EXTERNAL)) {
			error_ = SDL_GetError();
			SDL_DestroyProperties(properties);
			return false;
		}
		window_ = SDL_CreateWindowWithProperties(properties);
		SDL_DestroyProperties(properties);
		if (!window_) {
			error_ = SDL_GetError();
			return false;
		}

		error_.clear();
		width_ = width;
		height_ = height;
		open_ = true;
		focused_ = (SDL_GetWindowFlags(window_) & SDL_WINDOW_INPUT_FOCUS) != 0;
		return true;
	}

	void SDLWindow::Destroy() {
		if (window_)
			SDL_DestroyWindow(window_);
		window_ = nullptr;
		open_ = false;
		focused_ = false;
		width_ = 0;
		height_ = 0;
	}

	bool SDLWindow::SetTitle(const std::string& title) {
		if (!window_) {
			error_ = "Cannot set the title without an SDL window.";
			return false;
		}
		if (!SDL_SetWindowTitle(window_, title.c_str())) {
			error_ = SDL_GetError();
			return false;
		}
		error_.clear();
		return true;
	}

	bool SDLWindow::SetVisible(bool visible) {
		if (!window_) {
			error_ = "Cannot change visibility without an SDL window.";
			return false;
		}
		const bool success = visible ? SDL_ShowWindow(window_) : SDL_HideWindow(window_);
		if (!success) {
			error_ = SDL_GetError();
			return false;
		}
		error_.clear();
		return true;
	}

	bool SDLWindow::GetPlatformWindowHandle(void*& handle) {
		handle = nullptr;
		if (!window_) {
			error_ = "Cannot get a platform handle without an SDL window.";
			return false;
		}

		const SDL_PropertiesID properties = SDL_GetWindowProperties(window_);
		if (!properties) {
			error_ = SDL_GetError();
			return false;
		}

#if defined(_WIN32)
		handle = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif defined(__APPLE__)
		handle = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#else
		error_ = "Getting a native window handle is unsupported on this platform.";
		return false;
#endif
		if (!handle) {
			error_ = "SDL did not expose a native window handle.";
			return false;
		}
		error_.clear();
		return true;
	}

	void SDLWindow::HandleEvent(const SDL_Event& event) {
		if (!window_)
			return;

		if (event.type == SDL_EVENT_QUIT) {
			open_ = false;
			return;
		}
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
			event.window.windowID == SDL_GetWindowID(window_)) {
			open_ = false;
			return;
		}
		if (event.type == SDL_EVENT_WINDOW_FOCUS_GAINED &&
			event.window.windowID == SDL_GetWindowID(window_)) {
			focused_ = true;
		}
		else if (event.type == SDL_EVENT_WINDOW_FOCUS_LOST &&
			event.window.windowID == SDL_GetWindowID(window_)) {
			focused_ = false;
		}
		else if (event.type == SDL_EVENT_WINDOW_RESIZED &&
			event.window.windowID == SDL_GetWindowID(window_)) {
			width_ = event.window.data1;
			height_ = event.window.data2;
		}
	}

	SDLInput::~SDLInput() {
		Shutdown();
	}

	bool SDLInput::Initialize(SDL_Window* window) {
		if (!window) {
			error_ = "Cannot initialize input without an SDL window.";
			return false;
		}
		window_ = window;
		Reset();
		return RefreshJoysticks();
	}

	void SDLInput::Shutdown() {
		for (auto& joystick : joysticks_) {
			if (joystick.gamepad)
				SDL_CloseGamepad(joystick.gamepad);
			if (joystick.handle)
				SDL_CloseJoystick(joystick.handle);
		}
		joysticks_.clear();
		pads_.clear();
		padDown_.clear();
		window_ = nullptr;
		refreshJoysticks_ = false;
		Reset();
	}

	bool SDLInput::RefreshJoysticks() {
		if (!window_) {
			error_ = "Input is not initialized.";
			return false;
		}

		for (auto& joystick : joysticks_) {
			if (joystick.gamepad)
				SDL_CloseGamepad(joystick.gamepad);
			if (joystick.handle)
				SDL_CloseJoystick(joystick.handle);
		}
		joysticks_.clear();
		pads_.clear();
		padDown_.clear();

		int count = 0;
		SDL_JoystickID* ids = SDL_GetJoysticks(&count);
		if (!ids && count != 0) {
			error_ = SDL_GetError();
			return false;
		}

		bool success = true;
		for (int i = 0; i < count && joysticks_.size() < MaxJoysticks; ++i) {
			SDL_Joystick* handle = SDL_OpenJoystick(ids[i]);
			if (!handle) {
				error_ = SDL_GetError();
				success = false;
				continue;
			}

			SDL_Gamepad* gamepad = SDL_IsGamepad(ids[i]) ? SDL_OpenGamepad(ids[i]) : nullptr;
			joysticks_.push_back({ handle, gamepad });
			pads_.emplace_back();
			padDown_.emplace_back();
		}
		SDL_free(ids);
		refreshJoysticks_ = false;
		if (success)
			error_.clear();
		return success;
	}

	bool SDLInput::Update() {
		if (!window_) {
			error_ = "Input is not initialized.";
			return false;
		}
		if (refreshJoysticks_ && !RefreshJoysticks())
			return false;

		for (int key = 0; key < MaxKeys; ++key) {
			if (!keyTransitions_[key].empty()) {
				keys_[key] = UpdateState(keyTransitions_[key].front(), keys_[key]);
				keyTransitions_[key].pop_front();
			}
			else {
				keys_[key] = UpdateState(keysDown_[key], keys_[key]);
			}
		}

		for (size_t button = 0; button < mouse_.size(); ++button) {
			if (!mouseTransitions_[button].empty()) {
				mouse_[button] = UpdateState(mouseTransitions_[button].front(), mouse_[button]);
				mouseTransitions_[button].pop_front();
			}
			else {
				mouse_[button] = UpdateState(mouseDown_[button], mouse_[button]);
			}
		}

		mouseMotion_ = pendingMouseMotion_;
		pendingMouseMotion_.fill(0.0f);
		mouseWheel_ = pendingMouseWheel_;
		pendingMouseWheel_ = 0.0f;

		UpdateJoystickStates();

		float x = 0.0f;
		float y = 0.0f;
		if (SDL_GetMouseFocus() == window_) {
			SDL_GetMouseState(&x, &y);
			mouseX_ = static_cast<int>(x);
			mouseY_ = static_cast<int>(y);
		}

		error_.clear();
		return true;
	}

	void SDLInput::UpdateJoystickStates() {
		SDL_UpdateJoysticks();
		const Sint32 axisThreshold = 32767 * joystickResponseThreshold_ / 1000;
		for (size_t index = 0; index < joysticks_.size(); ++index) {
			SDL_Joystick* joystick = joysticks_[index].handle;
			auto& down = padDown_[index];
			auto& states = pads_[index];

			const int axisCount = SDL_GetNumJoystickAxes(joystick);
			const Sint16 axisX = axisCount > 0 ? SDL_GetJoystickAxis(joystick, 0) : 0;
			const Sint16 axisY = axisCount > 1 ? SDL_GetJoystickAxis(joystick, 1) : 0;
			down[0] = axisX < -axisThreshold;
			down[1] = axisX > axisThreshold;
			down[2] = axisY < -axisThreshold;
			down[3] = axisY > axisThreshold;

			const int hatCount = SDL_GetNumJoystickHats(joystick);
			const uint8_t hat = hatCount > 0 ? SDL_GetJoystickHat(joystick, 0) : SDL_HAT_CENTERED;
			down[4] = (hat & SDL_HAT_LEFT) != 0 ||
				(joysticks_[index].gamepad &&
					SDL_GetGamepadButton(joysticks_[index].gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT));
			down[5] = (hat & SDL_HAT_RIGHT) != 0 ||
				(joysticks_[index].gamepad &&
					SDL_GetGamepadButton(joysticks_[index].gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT));
			down[6] = (hat & SDL_HAT_UP) != 0 ||
				(joysticks_[index].gamepad &&
					SDL_GetGamepadButton(joysticks_[index].gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP));
			down[7] = (hat & SDL_HAT_DOWN) != 0 ||
				(joysticks_[index].gamepad &&
					SDL_GetGamepadButton(joysticks_[index].gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN));

			const int buttonCount = SDL_GetNumJoystickButtons(joystick);
			for (int button = 0; button < MaxPadButtons; ++button) {
				down[button + 8] = button < buttonCount &&
					SDL_GetJoystickButton(joystick, button);
			}
			for (size_t key = 0; key < states.size(); ++key)
				states[key] = UpdateState(down[key], states[key]);
		}
	}

	void SDLInput::Reset() {
		keysDown_.fill(false);
		keys_.fill(KeyState::Free);
		for (auto& transitions : keyTransitions_)
			transitions.clear();
		mouseDown_.fill(false);
		mouse_.fill(KeyState::Free);
		for (auto& transitions : mouseTransitions_)
			transitions.clear();
		mouseMotion_.fill(0.0f);
		pendingMouseMotion_.fill(0.0f);
		mouseWheel_ = 0.0f;
		pendingMouseWheel_ = 0.0f;
		mouseX_ = 0;
		mouseY_ = 0;
		for (auto& pad : pads_)
			pad.fill(KeyState::Free);
		for (auto& pad : padDown_)
			pad.fill(false);
	}

	KeyState SDLInput::GetKeyState(int key) const {
		return key >= 0 && key < MaxKeys ? keys_[key] : KeyState::Free;
	}

	KeyState SDLInput::GetMouseState(int button) const {
		return button >= 0 && button < MaxMouseButtons ? mouse_[button] : KeyState::Free;
	}

	KeyState SDLInput::GetPadState(int pad, int button) const {
		if (pad < 0 || static_cast<size_t>(pad) >= pads_.size() ||
			button < 0 || button >= MaxPadStates)
			return KeyState::Free;
		return pads_[pad][button];
	}

	JoystickInfo SDLInput::GetJoystickInfo(size_t index) const {
		if (index >= joysticks_.size())
			return {};
		SDL_Joystick* joystick = joysticks_[index].handle;
		const char* name = SDL_GetJoystickName(joystick);
		return {
			name ? name : "",
			SDL_GetJoystickVendor(joystick),
			SDL_GetJoystickProduct(joystick),
		};
	}

	void SDLInput::HandleEvent(const SDL_Event& event) {
		if (!window_)
			return;
		const SDL_WindowID windowId = SDL_GetWindowID(window_);

		switch (event.type) {
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP: {
			if (event.key.windowID != windowId)
				break;
			for (int key = 0; key < MaxKeys; ++key) {
				if (MapLegacyKey(key) == event.key.scancode) {
					const bool pressed = event.type == SDL_EVENT_KEY_DOWN;
					if (keysDown_[key] != pressed) {
						keysDown_[key] = pressed;
						keyTransitions_[key].push_back(pressed);
					}
					break;
				}
			}
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			if (event.button.windowID != windowId)
				break;
			const int button = MouseButtonIndex(event.button.button);
			if (button >= 0) {
				const bool pressed = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
				if (mouseDown_[button] != pressed) {
					mouseDown_[button] = pressed;
					mouseTransitions_[button].push_back(pressed);
				}
			}
			break;
		}
		case SDL_EVENT_MOUSE_MOTION:
			if (event.motion.windowID != windowId)
				break;
			pendingMouseMotion_[0] += event.motion.xrel;
			pendingMouseMotion_[1] += event.motion.yrel;
			mouseX_ = static_cast<int>(event.motion.x);
			mouseY_ = static_cast<int>(event.motion.y);
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			if (event.wheel.windowID != windowId)
				break;
			pendingMouseWheel_ += event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED
				? -event.wheel.y : event.wheel.y;
			break;
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			if (event.window.windowID == windowId) {
				for (int key = 0; key < MaxKeys; ++key) {
					if (keysDown_[key]) {
						keysDown_[key] = false;
						keyTransitions_[key].push_back(false);
					}
				}
				for (size_t button = 0; button < mouseDown_.size(); ++button) {
					if (mouseDown_[button]) {
						mouseDown_[button] = false;
						mouseTransitions_[button].push_back(false);
					}
				}
			}
			break;
		case SDL_EVENT_JOYSTICK_ADDED:
		case SDL_EVENT_JOYSTICK_REMOVED:
		case SDL_EVENT_GAMEPAD_ADDED:
		case SDL_EVENT_GAMEPAD_REMOVED:
			refreshJoysticks_ = true;
			break;
		default:
			break;
		}
	}

	SDL_Scancode SDLInput::MapLegacyKey(int key) {
		switch (key) {
		case 0x01: return SDL_SCANCODE_ESCAPE;
		case 0x02: return SDL_SCANCODE_1;
		case 0x03: return SDL_SCANCODE_2;
		case 0x04: return SDL_SCANCODE_3;
		case 0x05: return SDL_SCANCODE_4;
		case 0x06: return SDL_SCANCODE_5;
		case 0x07: return SDL_SCANCODE_6;
		case 0x08: return SDL_SCANCODE_7;
		case 0x09: return SDL_SCANCODE_8;
		case 0x0a: return SDL_SCANCODE_9;
		case 0x0b: return SDL_SCANCODE_0;
		case 0x0c: return SDL_SCANCODE_MINUS;
		case 0x0d: return SDL_SCANCODE_EQUALS;
		case 0x0e: return SDL_SCANCODE_BACKSPACE;
		case 0x0f: return SDL_SCANCODE_TAB;
		case 0x10: return SDL_SCANCODE_Q;
		case 0x11: return SDL_SCANCODE_W;
		case 0x12: return SDL_SCANCODE_E;
		case 0x13: return SDL_SCANCODE_R;
		case 0x14: return SDL_SCANCODE_T;
		case 0x15: return SDL_SCANCODE_Y;
		case 0x16: return SDL_SCANCODE_U;
		case 0x17: return SDL_SCANCODE_I;
		case 0x18: return SDL_SCANCODE_O;
		case 0x19: return SDL_SCANCODE_P;
		case 0x1a: return SDL_SCANCODE_LEFTBRACKET;
		case 0x1b: return SDL_SCANCODE_RIGHTBRACKET;
		case 0x1c: return SDL_SCANCODE_RETURN;
		case 0x1d: return SDL_SCANCODE_LCTRL;
		case 0x1e: return SDL_SCANCODE_A;
		case 0x1f: return SDL_SCANCODE_S;
		case 0x20: return SDL_SCANCODE_D;
		case 0x21: return SDL_SCANCODE_F;
		case 0x22: return SDL_SCANCODE_G;
		case 0x23: return SDL_SCANCODE_H;
		case 0x24: return SDL_SCANCODE_J;
		case 0x25: return SDL_SCANCODE_K;
		case 0x26: return SDL_SCANCODE_L;
		case 0x27: return SDL_SCANCODE_SEMICOLON;
		case 0x28: return SDL_SCANCODE_APOSTROPHE;
		case 0x29: return SDL_SCANCODE_GRAVE;
		case 0x2a: return SDL_SCANCODE_LSHIFT;
		case 0x2b: return SDL_SCANCODE_BACKSLASH;
		case 0x2c: return SDL_SCANCODE_Z;
		case 0x2d: return SDL_SCANCODE_X;
		case 0x2e: return SDL_SCANCODE_C;
		case 0x2f: return SDL_SCANCODE_V;
		case 0x30: return SDL_SCANCODE_B;
		case 0x31: return SDL_SCANCODE_N;
		case 0x32: return SDL_SCANCODE_M;
		case 0x33: return SDL_SCANCODE_COMMA;
		case 0x34: return SDL_SCANCODE_PERIOD;
		case 0x35: return SDL_SCANCODE_SLASH;
		case 0x36: return SDL_SCANCODE_RSHIFT;
		case 0x37: return SDL_SCANCODE_KP_MULTIPLY;
		case 0x38: return SDL_SCANCODE_LALT;
		case 0x39: return SDL_SCANCODE_SPACE;
		case 0x3a: return SDL_SCANCODE_CAPSLOCK;
		case 0x3b: return SDL_SCANCODE_F1;
		case 0x3c: return SDL_SCANCODE_F2;
		case 0x3d: return SDL_SCANCODE_F3;
		case 0x3e: return SDL_SCANCODE_F4;
		case 0x3f: return SDL_SCANCODE_F5;
		case 0x40: return SDL_SCANCODE_F6;
		case 0x41: return SDL_SCANCODE_F7;
		case 0x42: return SDL_SCANCODE_F8;
		case 0x43: return SDL_SCANCODE_F9;
		case 0x44: return SDL_SCANCODE_F10;
		case 0x45: return SDL_SCANCODE_NUMLOCKCLEAR;
		case 0x46: return SDL_SCANCODE_SCROLLLOCK;
		case 0x47: return SDL_SCANCODE_KP_7;
		case 0x48: return SDL_SCANCODE_KP_8;
		case 0x49: return SDL_SCANCODE_KP_9;
		case 0x4a: return SDL_SCANCODE_KP_MINUS;
		case 0x4b: return SDL_SCANCODE_KP_4;
		case 0x4c: return SDL_SCANCODE_KP_5;
		case 0x4d: return SDL_SCANCODE_KP_6;
		case 0x4e: return SDL_SCANCODE_KP_PLUS;
		case 0x4f: return SDL_SCANCODE_KP_1;
		case 0x50: return SDL_SCANCODE_KP_2;
		case 0x51: return SDL_SCANCODE_KP_3;
		case 0x52: return SDL_SCANCODE_KP_0;
		case 0x53: return SDL_SCANCODE_KP_PERIOD;
		case 0x57: return SDL_SCANCODE_F11;
		case 0x58: return SDL_SCANCODE_F12;
		case 0x64: return SDL_SCANCODE_F13;
		case 0x65: return SDL_SCANCODE_F14;
		case 0x66: return SDL_SCANCODE_F15;
		case 0x70: return SDL_SCANCODE_LANG1;
		case 0x79: return SDL_SCANCODE_INTERNATIONAL4;
		case 0x7b: return SDL_SCANCODE_INTERNATIONAL5;
		case 0x7d: return SDL_SCANCODE_INTERNATIONAL3;
		case 0x8d: return SDL_SCANCODE_KP_EQUALS;
		case 0x90: return SDL_SCANCODE_INTERNATIONAL6;
		case 0x91: return SDL_SCANCODE_INTERNATIONAL7;
		case 0x92: return SDL_SCANCODE_INTERNATIONAL8;
		case 0x93: return SDL_SCANCODE_INTERNATIONAL9;
		case 0x94: return SDL_SCANCODE_LANG2;
		case 0x95: return SDL_SCANCODE_STOP;
		case 0x9c: return SDL_SCANCODE_KP_ENTER;
		case 0x9d: return SDL_SCANCODE_RCTRL;
		case 0xb3: return SDL_SCANCODE_KP_COMMA;
		case 0xb5: return SDL_SCANCODE_KP_DIVIDE;
		case 0xb7: return SDL_SCANCODE_PRINTSCREEN;
		case 0xb8: return SDL_SCANCODE_RALT;
		case 0xc5: return SDL_SCANCODE_PAUSE;
		case 0xc7: return SDL_SCANCODE_HOME;
		case 0xc8: return SDL_SCANCODE_UP;
		case 0xc9: return SDL_SCANCODE_PAGEUP;
		case 0xcb: return SDL_SCANCODE_LEFT;
		case 0xcd: return SDL_SCANCODE_RIGHT;
		case 0xcf: return SDL_SCANCODE_END;
		case 0xd0: return SDL_SCANCODE_DOWN;
		case 0xd1: return SDL_SCANCODE_PAGEDOWN;
		case 0xd2: return SDL_SCANCODE_INSERT;
		case 0xd3: return SDL_SCANCODE_DELETE;
		case 0xdb: return SDL_SCANCODE_LGUI;
		case 0xdc: return SDL_SCANCODE_RGUI;
		case 0xdd: return SDL_SCANCODE_APPLICATION;
		case 0xde: return SDL_SCANCODE_POWER;
		case 0xdf: return SDL_SCANCODE_SLEEP;
		default: return SDL_SCANCODE_UNKNOWN;
		}
	}

	SDLPlatform::~SDLPlatform() {
		Shutdown();
	}

	bool SDLPlatform::Initialize() {
		if (initialized_)
			return true;
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD)) {
			error_ = SDL_GetError();
			return false;
		}
		initialized_ = true;
		running_ = true;
		error_.clear();
		return true;
	}

	void SDLPlatform::Shutdown() {
		if (!initialized_)
			return;
		SDL_Quit();
		initialized_ = false;
		running_ = false;
	}

	bool SDLPlatform::PollEvents(SDLWindow& window, SDLInput& input,
		const std::function<void(const SDL_Event&)>& eventHandler) {
		if (!initialized_) {
			error_ = "SDL platform is not initialized.";
			return false;
		}
		if (!window.window_ || input.window_ != window.window_) {
			error_ = "The SDL window and input context must be initialized together.";
			return false;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			window.HandleEvent(event);
			input.HandleEvent(event);
			if (eventHandler)
				eventHandler(event);
		}
		running_ = window.IsOpen();
		error_.clear();
		return true;
	}
}
