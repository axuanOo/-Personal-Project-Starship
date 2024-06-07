#pragma once
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Rgba8.hpp"
#include <string>
#include <vector>

class Renderer;
class Camera;
class Timer;
class BitmapFont;
struct AABB2;

typedef NamedStrings EventArgs;

class DevConsole;
extern DevConsole* g_theConsole;

//--------------------------------------------------------------------------------------
// Devconsole defaults. A renderer and camera must be provided
struct DevConsoleConfig
{
	std::string			m_font;
	float				m_numLines;
	Renderer*			m_renderer = nullptr;
	Camera*				m_camera = nullptr;
	float				m_fontAspect = 0.7f;
	int					m_linesOnScreen = 40;
	int					m_maxCommandHistory = 128;
	bool				m_startOpen = false;
};


//--------------------------------------------------------------------------------------
// Stores the text and color for an individual line of text
struct DevConsoleLine
{

	Rgba8		m_color;
	std::string m_content;
	int			m_frame;
	double		m_timeStamp;
};

enum class DevConsoleMode 
{
	HIDDEN,
	APPEAR
};


class DevConsole
{
public:

	DevConsole(DevConsoleConfig const& config);
	~DevConsole();

	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void Execute(std::string const& consoleCommandText);
	void AddLine(Rgba8 const& color, std::string const& text);

	void Render(AABB2 const& bounds, Renderer* rendereroverride=nullptr) const;

	DevConsoleMode GetMode() const;
	void SetMode(DevConsoleMode mode);
	void ToggleMode();

	static const Rgba8 ERROR;
	static const Rgba8 WARNING;
	static const Rgba8 INFO_MAJOR;
	static const Rgba8 INFO_MINOR;
	static const Rgba8 COMMAND_ECHO;
	static const Rgba8 COMMAND_REMOTE_ECHO;
	static const Rgba8 INPUT_TEXT;
	static const Rgba8 INPUT_INSERTION_POINT;


	// Handle key input
	static bool Event_KeyPressed(const EventArgs& args);

	// Handle char input by appending valid characters to our current input line
	static bool Event_CharInput(const EventArgs& args);

	// Clear all lines of text
	static bool Command_Clear(const EventArgs& args);

	// Echo the text in the argument to the dev console
	static bool Command_Echo(const EventArgs& args);

	// Display all current registered commands in the event system
	static bool Command_Help(const EventArgs& args);

protected:

	void Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect = 1.f) const;

	// True if the dev console is currently visible and accepting input
	bool										  m_isOpen =false;

	// All lines added to the dev console since the last time it was cleared
	std::vector<DevConsoleLine>					  m_lines;  // #ToDo: support a max limited # of lines (e.g. fixed circular buffer)

	// Our current line of the input text
	std::string									  m_inputText;

	// Index of the insertion point in our current input text
	int												m_insertionPointPosition = 0;

	// True if our insertion point is currently in the visible phase of blinking
	bool											m_insertionPointVisibile = true;

	// Timer for controlling insertion point visibility
	Timer*											m_insertionPointBlinkTimer;

	// History of all commands executed
	std::vector<std::string>						m_commandHistory;

	// Our current index in our history of commands as we are scrolling
	int												m_historyIndex = 0;

protected:               
	DevConsoleConfig                              m_config;
	DevConsoleMode                                m_mode = DevConsoleMode::HIDDEN;
	int                                           m_frameNumber = 0;
};
