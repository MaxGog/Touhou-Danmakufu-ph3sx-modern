#include "source/GcLib/pch.h"

#include "Application.hpp"

using namespace gstd;

//*******************************************************************
//Application
//*******************************************************************
Application* Application::thisBase_ = nullptr;
Application::Application() {
	::InitCommonControls();
}
Application::~Application() {
	thisBase_ = nullptr;
}
bool Application::Initialize() {
	if (thisBase_) return false;

	thisBase_ = this;
	hAppInstance_ = ::GetModuleHandle(NULL);
	bAppRun_ = true;
	bAppActive_ = true;
	//return _Initialize();

	return true;
}
bool Application::Run() {
	while (bAppRun_) {
		if (!_ProcessPlatformEvents())
			break;
		if (!bAppRun_)
			break;

		if (!bAppActive_) {
			Sleep(10);
			continue;
		}

		if (!_Loop())
			break;
	}

	bAppRun_ = false;
	return true;
}

bool Application::_ProcessPlatformEvents() {
	MSG message{};
	while (::PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
		if (message.message == WM_QUIT) {
			bAppRun_ = false;
			return false;
		}
		::TranslateMessage(&message);
		::DispatchMessageW(&message);
	}
	return true;
}
