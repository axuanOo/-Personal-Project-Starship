#pragma once
#include "Engine/Core/EngineCommon.hpp"

class InputSystem;

struct WindowConfig 
{
	InputSystem*	m_inputSystem = nullptr;
	std::string		m_windowTitle = "Untitled App";
	float			m_clientAspect = 2.0f / 3.0f;
	bool			m_isFullscreen = false;
};

class Window 
{
public:

	Window();
	Window( WindowConfig const& config);
	~Window();

	void				Startup();
	void				BeginFrame();
	void				EndFrame();
	void				Shutdown();

	WindowConfig const& GetConfig() const;
	float				GetAspect() const;
	void*				GetHwnd() const;
	IntVec2				GetClientDimensions() const;
	void*				GetWindowDC() const;
	static Window*		GetMainWindowInstance();
	Vec2				GetNormalizedCursorPos() const;
	bool				GetWindowFocus() const;

protected:
	void				CreateOSWindow();
	void				RunMessagePump();

protected:
	static Window*		s_mainWindow;

private:
	WindowConfig		m_config;
	IntVec2				m_clientDimensions;
	void*				m_hwnd = nullptr;			// HWND is Windows (handle to the current window)
	void*				m_dc = nullptr;				// HDC in Windows (handle to the display device context)
};