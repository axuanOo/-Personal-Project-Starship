#pragma once
#include "Engine/Input/AnalogJoystick.hpp"
#include "Engine/Input/KeyButtonState.hpp"
#include "Engine/Math/MathUtils.hpp"

enum class XboxButtonID
{
	XBOX_BUTTON_UP,XBOX_BUTTON_DOWN,XBOX_BUTTON_LEFT,XBOX_BUTTON_RIGHT,
	XBOX_BUTTON_START,XBOX_BUTTON_BACK,
	XBOX_BUTTON_LB,XBOX_BUTTON_RB,XBOX_BUTTON_LS,XBOX_BUTTON_RS,
	XBOX_BUTTON_A,XBOX_BUTTON_B,XBOX_BUTTON_X,XBOX_BUTTON_Y,NUM
};

constexpr float		MAX_STICK				= 32767.f;
constexpr float		MIN_STICK				= -32768.f;
constexpr int		MAX_TRIGGER				= 255;


class XboxController 
{
	friend class InputSystem;

public:
	XboxController();
	XboxController(int id);
	~XboxController();

	bool					IsConnected() const;
	int						GetControllerID() const;
	AnalogJoystick	const&	GetLeftStick() const;
	AnalogJoystick	const&	GetRightStick() const;
	float					GetLeftTrigger() const;
	float					GetRightTrigger() const;
	KeyButtonState	const&  GetButton( XboxButtonID buttonID ) const;
	bool					IsButtonDown( XboxButtonID buttonID ) const;
	bool					WasButtonJustPressed( XboxButtonID buttonID ) const;
	bool					WasButtonJustReleased( XboxButtonID buttonID) const;

private:

	void					Update();
	void					Reset();
	void					UpdateJoystick( AnalogJoystick& out_joystick, short rawX, short rawY);
	void					UpdateTrigger( float& out_triggerValue, unsigned char rawValue);
	void					UpdateButton( XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag );

private:

	int						m_id				= -1;
	bool					m_isConnected		= false;
	float					m_leftTrigger		= 0.f;
	float					m_rightTrigger		= 0.f;
	KeyButtonState			m_buttons[(int)XboxButtonID::NUM];				// (int)XboxButtonID::NUM
	AnalogJoystick			m_leftStick;
	AnalogJoystick			m_rightStick;

};
