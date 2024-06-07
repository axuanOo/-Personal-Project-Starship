#include "Engine/Input/XboxController.hpp"
#include <Windows.h>
#include <Xinput.h>
#pragma comment( lib, "xinput9_1_0" ) // Xinput 1_4 doesn't work in older Windows versions; use XInput 9_1_0 explicitly for best compatibility


XboxController::XboxController(int id)
{
	m_id = id;
}

XboxController::XboxController()
{

}

XboxController::~XboxController()
{

}

bool XboxController::IsConnected() const
{
	return m_isConnected;
}

int XboxController::GetControllerID() const
{
	return m_id;
}

AnalogJoystick	const& XboxController::GetLeftStick() const
{
	return m_leftStick;
}

AnalogJoystick	const& XboxController::GetRightStick() const
{
	return m_rightStick;
}

float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}

float XboxController::GetRightTrigger() const
{
	return m_rightTrigger;
}

KeyButtonState	const& XboxController::GetButton(XboxButtonID buttonID) const
{
	return m_buttons[(int)buttonID];
}

bool XboxController::IsButtonDown(XboxButtonID buttonID) const
{
	return m_buttons[(int)buttonID].m_isPressed;
}


bool XboxController::WasButtonJustPressed(XboxButtonID buttonID) const
{
	return m_buttons[(int)buttonID].m_wasPressedLastFrame;
}

bool XboxController::WasButtonJustReleased(XboxButtonID buttonID) const
{
	return m_buttons[(int)buttonID].m_wasPressedLastFrame && !m_buttons[(int)buttonID].m_isPressed;
}

void XboxController::Update()
{
	XINPUT_STATE xboxControllerState = {};
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
	m_isConnected = errorStatus == ERROR_SUCCESS ? true : false;

	if (errorStatus == ERROR_SUCCESS)
	{
		UpdateButton(XboxButtonID::XBOX_BUTTON_UP, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_UP);
		UpdateButton(XboxButtonID::XBOX_BUTTON_DOWN, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
		UpdateButton(XboxButtonID::XBOX_BUTTON_LEFT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
		UpdateButton(XboxButtonID::XBOX_BUTTON_RIGHT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);
		UpdateButton(XboxButtonID::XBOX_BUTTON_LB, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
		UpdateButton(XboxButtonID::XBOX_BUTTON_RB, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		UpdateButton(XboxButtonID::XBOX_BUTTON_LS, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
		UpdateButton(XboxButtonID::XBOX_BUTTON_RS, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
		UpdateButton(XboxButtonID::XBOX_BUTTON_START, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_START);
		UpdateButton(XboxButtonID::XBOX_BUTTON_BACK, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_BACK);
		UpdateButton(XboxButtonID::XBOX_BUTTON_A, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_A);
		UpdateButton(XboxButtonID::XBOX_BUTTON_B, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_B);
		UpdateButton(XboxButtonID::XBOX_BUTTON_X, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_X);
		UpdateButton(XboxButtonID::XBOX_BUTTON_Y, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_Y);

		UpdateTrigger(m_leftTrigger,xboxControllerState.Gamepad.bLeftTrigger);
		UpdateTrigger(m_rightTrigger, xboxControllerState.Gamepad.bRightTrigger);

		UpdateJoystick(m_leftStick, xboxControllerState.Gamepad.sThumbLX, xboxControllerState.Gamepad.sThumbLY);
		UpdateJoystick(m_rightStick, xboxControllerState.Gamepad.sThumbRX, xboxControllerState.Gamepad.sThumbRY);
	}
	else
	{
		m_isConnected = false;
		Reset();
		return;
	}
}

void XboxController::Reset()
{
	m_leftTrigger		= 0.f;
	m_rightTrigger		= 0.f;
}

void XboxController::UpdateJoystick(AnalogJoystick& out_joystick, short rawX, short rawY)
{
	float rawPositionX = RangeMapClamped((float)rawX, MIN_STICK, MAX_STICK, -1.f, 1.f);
	float rawPositionY = RangeMapClamped((float)rawY, MIN_STICK, MAX_STICK, -1.f, 1.f);

	out_joystick.UpdatePosition(rawPositionX, rawPositionY);
}

void XboxController::UpdateTrigger(float& out_triggerValue, unsigned char rawValue)
{
	out_triggerValue = static_cast<float>(rawValue / MAX_TRIGGER);
}

void XboxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
	m_buttons[(int)buttonID].m_wasPressedLastFrame = m_buttons[(int)buttonID].m_isPressed;
	bool isButtonDown = static_cast<bool>(buttonFlags & buttonFlag);
	m_buttons[(int)buttonID].m_isPressed = isButtonDown;

}
