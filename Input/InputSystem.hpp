#pragma once
#include "Engine/Input/XboxController.hpp"
#include "Engine/Core/EngineCommon.hpp"

constexpr int NUM_KEYCODES				=256;
constexpr int NUM_XBOX_CONTROLLERS		=4;

extern const unsigned char KEYCODE_F1; 
extern const unsigned char KEYCODE_F2;  
extern const unsigned char KEYCODE_F3;
extern const unsigned char KEYCODE_F4;
extern const unsigned char KEYCODE_F5;
extern const unsigned char KEYCODE_F6;
extern const unsigned char KEYCODE_F7;
extern const unsigned char KEYCODE_F8;
extern const unsigned char KEYCODE_F9;
extern const unsigned char KEYCODE_F10;
extern const unsigned char KEYCODE_F11;

extern const unsigned char KEYCODE_ESC;
extern const unsigned char KEYCODE_SPACE;
extern const unsigned char KEYCODE_ENTER;
extern const unsigned char KEYCODE_SHIFT;
extern const unsigned char KEYCODE_BACKSPACE;
extern const unsigned char KEYCODE_INSERT;
extern const unsigned char KEYCODE_DELETE;
extern const unsigned char KEYCODE_HOME;
extern const unsigned char KEYCODE_END;

extern const unsigned char KEYCODE_UPARROW;
extern const unsigned char KEYCODE_DOWNARROW;
extern const unsigned char KEYCODE_LEFTARROW;
extern const unsigned char KEYCODE_RIGHTARROW;

extern const unsigned char KEYCODE_LEFT_MOUSE;
extern const unsigned char KEYCODE_RIGHT_MOUSE;
extern const unsigned char KEYCODE_TILDE;
extern const unsigned char KEYCODE_LEFTBRACKET;
extern const unsigned char KEYCODE_RIGHTBRACKET;

extern InputSystem* g_theInput;

struct InputConfig
{

};

struct CursorState
{
	IntVec2 m_currentCursorPosition;
	IntVec2 m_previousCursorPosition;
	IntVec2 m_cursorClientDelta;
	IntVec2 m_cursorClientPosition;

	bool m_hiddenMode = false;
	bool m_relativeMode = false;

	// Hidden mode controls whether the cursor is visible or not
	// Relative mode will calculate a cursor client delta and then
	// reset the cursor to the region center each frame. Both of 
	// these together can be used to implement a FPS-style mouse lock camera
	void SetCursorState(bool hiddenMode, bool relativeMode);

	// Returns the current frame cursor delta in pixels, relative to the 
	// client region. Only valid in relative mode, will be zero otherwise
	IntVec2 GetCursorClientDelta() const;

	// Returns the cursor position, normalized to the range [0,1], relative
	// to the client space, with the y-axis inverted to map from windows
	// convention to game camera conventions
	Vec2 GetCursorNomalizedPosition() const;
};


class InputSystem
{
public:
	InputSystem();
	InputSystem(InputConfig const& config);
	~InputSystem();

	void						StartUp();
	void						Shutdown();
	void						BeginFrame();
	void						EndFrame();

	bool						WasKeyJustPressed(unsigned char keyCode);
	bool						WasKeyJustReleased(unsigned char keyCode);
	bool						IsKeyDown(unsigned char keyCode);
	void						HandleKeyPressed(unsigned char keyCode);
	void						HandleKeyReleased(unsigned char keyCode);

	void						SetKeyStateLastFrame(unsigned char keyCode, bool value);
	InputConfig const&			GetConfig() const;
	XboxController	const&		GetController(int contollerID);
	int							GetMouseScroll() const;

	static bool					Event_KeyPressed(const EventArgs& args);
	static bool					Event_KeyReleased(const EventArgs& args);
	static bool					Event_MouseWheelScroll(const EventArgs& args);

	CursorState					m_cursorState;
protected:

	KeyButtonState				m_keyStates  [NUM_KEYCODES];
	XboxController				m_controllers[NUM_XBOX_CONTROLLERS];
	
private:

	InputConfig					m_config;
	int							m_mouseScroll = 0;
	
};