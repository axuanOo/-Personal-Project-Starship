#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Input/InputSystem.hpp"
#include <Windows.h>

const unsigned char KEYCODE_F1		= VK_F1; 
const unsigned char KEYCODE_F2		= VK_F2;  
const unsigned char KEYCODE_F3		= VK_F3;
const unsigned char	KEYCODE_F4		= VK_F4;
const unsigned char KEYCODE_F5		= VK_F5;
const unsigned char	KEYCODE_F6		= VK_F6;
const unsigned char KEYCODE_F7		= VK_F7;
const unsigned char	KEYCODE_F8		= VK_F8;
const unsigned char KEYCODE_F9		= VK_F9;
const unsigned char	KEYCODE_F10		= VK_F10;
const unsigned char KEYCODE_F11		= VK_F11;

const unsigned char KEYCODE_SHIFT	= VK_SHIFT;
const unsigned char KEYCODE_ESC		= VK_ESCAPE;
const unsigned char KEYCODE_SPACE	= VK_SPACE;
const unsigned char KEYCODE_ENTER	= VK_RETURN;
const unsigned char KEYCODE_BACKSPACE =VK_BACK;
const unsigned char KEYCODE_INSERT = VK_INSERT;
const unsigned char KEYCODE_DELETE = VK_DELETE;
const unsigned char KEYCODE_HOME = VK_HOME;
const unsigned char KEYCODE_END = VK_END;

const unsigned char KEYCODE_UPARROW = VK_UP;
const unsigned char KEYCODE_DOWNARROW = VK_DOWN;
const unsigned char KEYCODE_LEFTARROW = VK_LEFT;
const unsigned char KEYCODE_RIGHTARROW = VK_RIGHT;

const unsigned char KEYCODE_LEFT_MOUSE = VK_LBUTTON;
const unsigned char KEYCODE_RIGHT_MOUSE = VK_RBUTTON;
const unsigned char KEYCODE_TILDE = 0xC0;
const unsigned char KEYCODE_LEFTBRACKET = 0xDB;
const unsigned char KEYCODE_RIGHTBRACKET = 0xDD;


InputSystem::InputSystem()
{

}

InputSystem::InputSystem(InputConfig const& config)
{
	m_config = config;
}

void InputSystem::StartUp()
{
	m_cursorState = CursorState();

	for (int ctrlerIndex = 0; ctrlerIndex < NUM_XBOX_CONTROLLERS; ctrlerIndex++)
	{
		m_controllers[ctrlerIndex] = XboxController();
		m_controllers[ctrlerIndex].m_id = ctrlerIndex;
	}

	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed", InputSystem::Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("KeyReleased", InputSystem::Event_KeyReleased);
	g_theEventSystem->SubscribeEventCallbackFunction("MouseScroll", InputSystem::Event_MouseWheelScroll);
}

void InputSystem::Shutdown()
{

}

void InputSystem::BeginFrame()
{
	for (int ctrlerIndex = 0; ctrlerIndex < NUM_XBOX_CONTROLLERS; ctrlerIndex++)
	{
		m_controllers[ctrlerIndex].Update();
	}

	// Check if our current cursor hidden mode can match the window's cursor mode
	if (m_cursorState.m_hiddenMode)
	{
		while (ShowCursor(false) >= 0)
		{
		}
	}
	else 
	{
		while (ShowCursor(true) < 0)
		{
		}
	}

	POINT cursorPosition;
	GetCursorPos(&cursorPosition);
	ClientToScreen(GetActiveWindow(), &cursorPosition);
	m_cursorState.m_currentCursorPosition.x = (int)cursorPosition.x;
	m_cursorState.m_currentCursorPosition.y = (int)cursorPosition.y;

	if (m_cursorState.m_relativeMode)
	{
		m_cursorState.m_cursorClientDelta = m_cursorState.m_currentCursorPosition - m_cursorState.m_previousCursorPosition;

		POINT targetPosition = POINT();
		targetPosition.x = Window::GetMainWindowInstance()->GetClientDimensions().x / 2;
		targetPosition.y = Window::GetMainWindowInstance()->GetClientDimensions().y / 2;

		ClientToScreen(GetActiveWindow(),&targetPosition);
		// Set the cursor position back to the center of the client center
		SetCursorPos((int)targetPosition.x, (int)targetPosition.y);

		GetCursorPos(&cursorPosition);
		ClientToScreen(GetActiveWindow(), &cursorPosition);
		m_cursorState.m_currentCursorPosition.x = (int)cursorPosition.x;
		m_cursorState.m_currentCursorPosition.y = (int)cursorPosition.y;
	}
	else 
	{
		m_cursorState.m_cursorClientDelta = IntVec2::ZERO;
	}
}

void InputSystem::EndFrame()
{
	for (int keyIndex = 0; keyIndex < NUM_KEYCODES; keyIndex++)
	{
		SetKeyStateLastFrame(static_cast<unsigned char>(keyIndex), IsKeyDown(static_cast<unsigned char>(keyIndex)));
	}

	// Set the previous cursor position as the value of the current cursor position;
	m_cursorState.m_previousCursorPosition = m_cursorState.m_currentCursorPosition;
}


bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_wasPressedLastFrame;
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_wasPressedLastFrame && !m_keyStates[keyCode].m_isPressed;
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_isPressed;
}

void InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = true;
}

void InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = false;
}

void InputSystem::SetKeyStateLastFrame(unsigned char keyCode, bool value)
{
	m_keyStates[keyCode].m_wasPressedLastFrame = value;
}

InputConfig const& InputSystem::GetConfig() const
{
	return m_config;
}

XboxController	const& InputSystem::GetController(int contollerID)
{
	return m_controllers[contollerID];
}


int InputSystem::GetMouseScroll() const
{
	return m_mouseScroll;
}

bool InputSystem::Event_KeyPressed(const EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}

	if (g_theConsole!=nullptr)
	{
		if (g_theConsole->GetMode() == DevConsoleMode::APPEAR)
		{
			return false;
		}
	}

	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyPressed(keyCode);
	return true;
}

bool InputSystem::Event_KeyReleased(const EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}

	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyReleased(keyCode);
	return true;
}


bool InputSystem::Event_MouseWheelScroll(const EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}

	g_theInput->m_mouseScroll += args.GetValue("ScrollValue", 0);
	return  true;
}

void CursorState::SetCursorState(bool hiddenMode, bool relativeMode)
{
	m_hiddenMode = hiddenMode;
	m_relativeMode = relativeMode;
}

IntVec2 CursorState::GetCursorClientDelta() const
{
	return m_cursorClientDelta;
}

Vec2 CursorState::GetCursorNomalizedPosition() const
{
	HWND windowHandle = ::GetActiveWindow();
	POINT cursorCoords;
	RECT clientRect;
	::GetCursorPos(&cursorCoords); // in screen coordinates, (0,0) top-left
	::ScreenToClient(windowHandle, &cursorCoords); // relative to the window interior
	::GetClientRect(windowHandle, &clientRect); // size of window interior (0,0 to width,height)
	float cursorX = float(cursorCoords.x) / float(clientRect.right); // normalized x position
	float cursorY = float(cursorCoords.y) / float(clientRect.bottom); // normalized y position
	return Vec2(cursorX, 1.0f - cursorY); // we want (0,0) in the bottom-left
}
