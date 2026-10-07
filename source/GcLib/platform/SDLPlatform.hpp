#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <string>
#include <vector>

namespace platform {
	enum class KeyState : uint8_t {
		Free,
		Push,
		Pull,
		Hold,
	};

	struct Point {
		int x = 0;
		int y = 0;
	};

	struct JoystickInfo {
		std::string name;
		uint16_t vendorId = 0;
		uint16_t productId = 0;
	};

	class SDLInput;

	class SDLWindow {
		SDL_Window* window_ = nullptr;
		bool open_ = false;
		bool focused_ = false;
		int width_ = 0;
		int height_ = 0;
		std::string error_;

		void HandleEvent(const SDL_Event& event);
		friend class SDLPlatform;
	public:
		SDLWindow() = default;
		~SDLWindow();

		SDLWindow(const SDLWindow&) = delete;
		SDLWindow& operator=(const SDLWindow&) = delete;

		bool Create(const std::string& title, int width, int height,
			SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
		bool WrapNative(void* nativeWindow, int width, int height);
		void Destroy();
		bool SetTitle(const std::string& title);
		bool SetVisible(bool visible);

		SDL_Window* GetNativeWindow() const { return window_; }
		bool GetPlatformWindowHandle(void*& handle);
		bool IsOpen() const { return open_; }
		bool IsFocused() const { return focused_; }
		int GetWidth() const { return width_; }
		int GetHeight() const { return height_; }
		const std::string& GetError() const { return error_; }
	};

	class SDLInput {
	public:
		static constexpr int MaxKeys = 256;
		static constexpr int MaxMouseButtons = 4;
		static constexpr int MaxPadButtons = 16;
		static constexpr int MaxPadStates = 32;
		static constexpr int MaxJoysticks = 4;

	private:
		struct Joystick {
			SDL_Joystick* handle = nullptr;
			SDL_Gamepad* gamepad = nullptr;
		};

		SDL_Window* window_ = nullptr;
		std::array<bool, MaxKeys> keysDown_{};
		std::array<KeyState, MaxKeys> keys_{};
		std::array<std::deque<bool>, MaxKeys> keyTransitions_{};
		std::array<bool, MaxMouseButtons> mouseDown_{};
		std::array<KeyState, MaxMouseButtons> mouse_{};
		std::array<std::deque<bool>, MaxMouseButtons> mouseTransitions_{};
		std::array<float, 2> mouseMotion_{};
		std::array<float, 2> pendingMouseMotion_{};
		std::vector<Joystick> joysticks_;
		std::vector<std::array<KeyState, MaxPadStates>> pads_;
		std::vector<std::array<bool, MaxPadStates>> padDown_;
		bool refreshJoysticks_ = false;
		int mouseX_ = 0;
		int mouseY_ = 0;
		float mouseWheel_ = 0;
		float pendingMouseWheel_ = 0;
		int joystickResponseThreshold_ = 500;
		std::string error_;

		void HandleEvent(const SDL_Event& event);
		void UpdateJoystickStates();
		static SDL_Scancode MapLegacyKey(int key);
		friend class SDLPlatform;
	public:
		SDLInput() = default;
		~SDLInput();

		SDLInput(const SDLInput&) = delete;
		SDLInput& operator=(const SDLInput&) = delete;

		bool Initialize(SDL_Window* window);
		void Shutdown();
		bool Update();
		bool RefreshJoysticks();
		void Reset();
		void SetJoystickResponseThreshold(int threshold) {
			joystickResponseThreshold_ = threshold < 0 ? 0 : threshold > 1000 ? 1000 : threshold;
		}

		KeyState GetKeyState(int key) const;
		KeyState GetMouseState(int button) const;
		KeyState GetPadState(int pad, int button) const;
		float GetMouseMoveX() const { return mouseMotion_[0]; }
		float GetMouseMoveY() const { return mouseMotion_[1]; }
		float GetMouseMoveZ() const { return mouseWheel_; }
		Point GetMousePosition() const { return { mouseX_, mouseY_ }; }

		size_t GetJoystickCount() const { return joysticks_.size(); }
		JoystickInfo GetJoystickInfo(size_t index) const;
		const std::string& GetError() const { return error_; }
	};

	class SDLPlatform {
		bool initialized_ = false;
		bool running_ = false;
		std::string error_;
	public:
		SDLPlatform() = default;
		~SDLPlatform();

		SDLPlatform(const SDLPlatform&) = delete;
		SDLPlatform& operator=(const SDLPlatform&) = delete;

		bool Initialize();
		void Shutdown();
		bool PollEvents(SDLWindow& window, SDLInput& input,
			const std::function<void(const SDL_Event&)>& eventHandler = {});
		bool IsRunning() const { return running_; }
		const std::string& GetError() const { return error_; }
	};
}
