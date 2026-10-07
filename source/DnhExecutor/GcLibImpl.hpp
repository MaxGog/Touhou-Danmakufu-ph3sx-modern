#pragma once

#include "../GcLib/pch.h"

#include "Constant.hpp"

#if defined(DNH_PROJ_EXECUTOR)
#include "../GcLib/platform/SDLPlatform.hpp"
#include "../TouhouDanmakufu/DnhGcLibImpl.hpp"

//*******************************************************************
//EApplication
//*******************************************************************
class EDirectGraphics;
class EApplication : public Singleton<EApplication>, public Application {
	friend Singleton<EApplication>;
protected:
	EDirectGraphics* ptrGraphics;
	platform::SDLPlatform sdlPlatform_;
	platform::SDLWindow sdlWindow_;

	bool bWindowFocused_;

	shared_ptr<Texture> secondaryBackBuffer_;
protected:
	void UpdateFrame(bool enableInput);
	void RenderFrame();
	
	void RenderScene();
	void PresentScene();
public:
	EApplication();
	~EApplication();

	bool _Initialize();
	bool _ProcessPlatformEvents() override;
	bool _Loop();
	bool _Finalize();
public:
	EDirectGraphics* GetPtrGraphics() { return ptrGraphics; }

	bool IsWindowFocused() { return bWindowFocused_; }

	void SetSecondaryBackBuffer(shared_ptr<Texture> texture) { secondaryBackBuffer_ = texture; }
};

//*******************************************************************
//EDirectGraphics
//*******************************************************************
class EDirectGraphics : public Singleton<EDirectGraphics>, public DirectGraphicsPrimaryWindow {
	friend Singleton<EDirectGraphics>;
protected:
	std::wstring defaultWindowTitle_;
	platform::SDLWindow* mainWindow_;
	bool windowSubclassInstalled_;
protected:
	static LRESULT CALLBACK _WindowSubclassProcedure(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
		UINT_PTR subclassId, DWORD_PTR referenceData);
public:
	EDirectGraphics();
	~EDirectGraphics();

	virtual bool Initialize(const std::wstring& windowTitle, platform::SDLWindow& mainWindow);
	void SetRenderStateFor2D(BlendMode type);

	const std::wstring& GetDefaultWindowTitle() { return defaultWindowTitle_; }
	void SetWindowTitle(const std::wstring& title);
	void SetWindowVisible(bool visible);
};

#endif