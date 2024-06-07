#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/BitmapFont.hpp"

DevConsole* g_theConsole = nullptr;

const Rgba8 DevConsole::INFO_MAJOR = Rgba8(255, 255, 255, 255);
const Rgba8 DevConsole::INFO_MINOR = Rgba8(0, 255, 0, 255);
const Rgba8 DevConsole::WARNING = Rgba8(255, 255, 0, 255);
const Rgba8 DevConsole::ERROR = Rgba8(255, 0, 0, 255);
const Rgba8 DevConsole::COMMAND_ECHO= Rgba8(255, 100, 0, 255);
const Rgba8 DevConsole::COMMAND_REMOTE_ECHO = Rgba8(0, 255, 255, 255);
const Rgba8 DevConsole::INPUT_TEXT= Rgba8(100, 255, 255, 255);
const Rgba8 DevConsole::INPUT_INSERTION_POINT = Rgba8(255, 255, 255, 255);

DevConsole::DevConsole(DevConsoleConfig const& config)
	:m_config(config)
{
}

DevConsole::~DevConsole()
{
	Shutdown();
}

void DevConsole::Startup()
{
	m_lines.reserve((int)m_config.m_numLines);

	g_theConsole->AddLine(DevConsole::INFO_MAJOR, "Type help for a list of commands!");

	m_insertionPointBlinkTimer = new Timer(1.0f, nullptr);
	m_insertionPointBlinkTimer->Start();

	g_theEventSystem->SubscribeEventCallbackFunction("help", DevConsole::Command_Help);
	g_theEventSystem->SubscribeEventCallbackFunction("clear", DevConsole::Command_Clear);
	g_theEventSystem->SubscribeEventCallbackFunction("echo", DevConsole::Command_Echo);
	g_theEventSystem->SubscribeEventCallbackFunction("CharInput", DevConsole::Event_CharInput);
	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed",DevConsole::Event_KeyPressed);
}

void DevConsole::Shutdown()
{

}

void DevConsole::BeginFrame()
{
	m_frameNumber += 1;

	if (m_insertionPointBlinkTimer->GetElapsedFraction() > 1.0f)
	{
		m_insertionPointBlinkTimer->DecrementPeriodIfElapsed();
	}
	else
	{
		if (m_insertionPointBlinkTimer->GetElapsedFraction() > 0.5f)
		{
			m_insertionPointVisibile = false;
		}
		else
		{
			m_insertionPointVisibile = true;
		}
	}
}

void DevConsole::EndFrame()
{

}

void DevConsole::Execute(std::string const& consoleCommandText)
{
	Strings strings = SplitStringOnDelimiter(consoleCommandText, '\n');

	for (int i = 0; i < (int)strings.size(); ++i)
	{
		Strings keyValueText = SplitStringOnDelimiter(strings[i],' ');

		for (int j = 0; j < (int)keyValueText.size(); ++j)
		{
			Strings keyValue = SplitStringOnDelimiter(keyValueText[j], '=');
		}
	}
}

void DevConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	DevConsoleLine line = DevConsoleLine();
	line.m_color = color;
	line.m_content = text;
	line.m_frame = m_frameNumber;
	line.m_timeStamp = Clock::GetSystemClock().GetTotalSeconds();
	m_lines.push_back(line);
}

void DevConsole::Render(AABB2 const& bounds, Renderer* rendereroverride/*=nullptr*/) const
{
	BitmapFont* font = rendereroverride->CreateOrGetBitmapFont(m_config.m_font.c_str());
	switch (m_mode)
	{
	case DevConsoleMode::APPEAR:
		Render_OpenFull(bounds, *rendereroverride, *font);
		return;
	case DevConsoleMode::HIDDEN:
		return;
	}
}


DevConsoleMode DevConsole::GetMode() const
{
	return m_mode;
}

void DevConsole::SetMode(DevConsoleMode mode)
{
	m_mode = mode;
}

void DevConsole::ToggleMode()
{
	switch (m_mode)
	{
	case DevConsoleMode::APPEAR:
		m_mode = DevConsoleMode::HIDDEN;
		g_theConsole->m_insertionPointBlinkTimer->Stop();
		m_isOpen = false;
		break;
	case DevConsoleMode::HIDDEN:
		m_mode = DevConsoleMode::APPEAR;
		m_insertionPointBlinkTimer->Start();
		m_isOpen = true;
		break;
	}
}


void DevConsole::Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect /*= 1.f*/) const
{
	UNUSED(fontAspect);

	std::vector<Vertex_PCU> vertsBox;
	std::vector<Vertex_PCU> vertsText;

	vertsBox.reserve(100);
	vertsText.reserve(1000);

	float singleHeight = bounds.GetDimensions().y / m_config.m_numLines;

	AddVertsForAABB2D(vertsBox, bounds, Rgba8::TRANSPARENT_GREY);

	for (int i = (int)(m_lines.size() - 1); i >= 0; --i)
	{
		Vec2 minPosBox = Vec2(bounds.m_mins.x, bounds.m_mins.y + (m_lines.size() - i) * singleHeight);
		Vec2 maxPosBox = Vec2(minPosBox.x + bounds.GetDimensions().x, minPosBox.y + singleHeight);

		AABB2 box = AABB2(minPosBox, maxPosBox);

		std::string showString = Stringf("[%.f] Frame[#%i], %s", m_lines[i].m_timeStamp, m_lines[i].m_frame, m_lines[i].m_content.c_str());
		font.AddVertsForTextInBox2D(vertsText, box, singleHeight, showString, m_lines[i].m_color, fontAspect, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT);
	}

	Vec2 minPosBoxForCurrentInput = Vec2(bounds.m_mins.x, bounds.m_mins.y);
	Vec2 maxPosBoxForCurrentInput = Vec2(minPosBoxForCurrentInput.x + bounds.GetDimensions().x, minPosBoxForCurrentInput.y + singleHeight);
	AABB2 boxForCurrentInput = AABB2(minPosBoxForCurrentInput, maxPosBoxForCurrentInput);
	font.AddVertsForTextInBox2D(vertsText, boxForCurrentInput, singleHeight, m_inputText, DevConsole::INPUT_TEXT, fontAspect, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT);
	
	renderer.BindShader(nullptr);
	renderer.SetBlendMode(BlendMode::ALPHA);
	renderer.SetModelConstants();
	renderer.BindTexture(nullptr);
	renderer.DrawVertexArray((int)vertsBox.size(), vertsBox.data());

	renderer.BindShader(nullptr);
	renderer.BindTexture(&font.GetTexture());
	renderer.SetModelConstants();
	renderer.DrawVertexArray((int)vertsText.size(), vertsText.data());

	//-----------------------------------------------------------------------------------------------
	// Render the insertion point here

	if (!m_insertionPointVisibile)
	{
		renderer.BindTexture(nullptr);
		renderer.SetModelConstants();
		return;
	}

	std::vector<Vertex_PCU> vertsInsertionPoint;

	vertsInsertionPoint.reserve(4);
	float singleCharLength = singleHeight * fontAspect;
	
	float insertionBoxHeight = singleHeight * 0.8f;
	float insertionBoxWidth = singleCharLength * 0.2f;

	// -------------------------------------------------------------------
	// Find out the length of single character

	if (m_inputText.size() * singleCharLength >= bounds.GetDimensions().x)
	{
		singleCharLength = bounds.GetDimensions().x / m_inputText.size();
	}
	// -------------------------------------------------------------------

	Vec2 insertPointOffset = Vec2(0.0f, 0.0f);

	Vec2 leftBottomPosition = Vec2
	(
		bounds.m_mins.x + m_insertionPointPosition * singleCharLength + insertPointOffset.x, 
		bounds.m_mins.y + 0.5f * (singleHeight - insertionBoxHeight)
	);


	Vec2 RightTopPosition = leftBottomPosition + Vec2(insertionBoxWidth, insertionBoxHeight);

	AABB2 insertPointBox = AABB2(leftBottomPosition, RightTopPosition);

	AddVertsForAABB2D(vertsInsertionPoint, insertPointBox, Rgba8::WHITE);
	
	renderer.BindShader(nullptr);
	renderer.BindTexture(nullptr);
	renderer.SetModelConstants();
	renderer.DrawVertexArray((int)vertsInsertionPoint.size(), vertsInsertionPoint.data());

}


bool DevConsole::Event_KeyPressed(const EventArgs& args)
{
	UNUSED(args);

	if (!g_theConsole->m_isOpen)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);

	// Handle "~/`" when press this key the dev console will close or open
	if (keyCode == KEYCODE_TILDE)
	{
		g_theConsole->ToggleMode();
		return true;
	}

	// Handle "esc" button depends on the current status
	if (keyCode == KEYCODE_ESC)
	{
		if ((int)g_theConsole->m_inputText.size() == 0)
		{
			g_theConsole->ToggleMode();
			return true;
		}
		g_theConsole->m_insertionPointPosition = 0;
		g_theConsole->m_inputText.clear();
	}

	// Handle "Enter" key to run the command line
	if (keyCode == KEYCODE_ENTER)
	{
		if ((int)g_theConsole->m_inputText.size() == 0)
		{
			g_theConsole->ToggleMode();
			return true;
		}

		g_theConsole->AddLine(INPUT_TEXT,g_theConsole->m_inputText);

		NamedStrings commands;
		commands.SetValue("InputText", g_theConsole->m_inputText);
		g_theEventSystem->FireEvent("echo", commands);

		g_theConsole->m_insertionPointPosition = 0;
		g_theConsole->m_inputText.clear();
	}

	// Handle "left/right arrow" when press the arrow key the insert position will change
	if (keyCode == KEYCODE_LEFTARROW)
	{
		g_theConsole->m_insertionPointBlinkTimer->Start();
		g_theConsole->m_insertionPointPosition < 1 ? g_theConsole->m_insertionPointPosition = 0 : g_theConsole->m_insertionPointPosition -= 1;
	}

	if (keyCode == KEYCODE_RIGHTARROW)
	{
		g_theConsole->m_insertionPointBlinkTimer->Start();
		g_theConsole->m_insertionPointPosition > (int)g_theConsole->m_inputText.size() - 1 ?
			g_theConsole->m_insertionPointPosition = (int)g_theConsole->m_inputText.size() : g_theConsole->m_insertionPointPosition += 1;
	}

	// Handle "up/down arrow"
	if (keyCode == KEYCODE_UPARROW)
	{

		if (!g_theConsole->m_commandHistory.empty())
		{
			g_theConsole->m_insertionPointBlinkTimer->Start();
			g_theConsole->m_historyIndex < 1 ? g_theConsole->m_historyIndex = 0 : g_theConsole->m_historyIndex -= 1;
			g_theConsole->m_inputText = g_theConsole->m_commandHistory[g_theConsole->m_historyIndex];
			g_theConsole->m_insertionPointPosition = (int)g_theConsole->m_inputText.size();
		}
	}

	if (keyCode == KEYCODE_DOWNARROW)
	{
		if (!g_theConsole->m_commandHistory.empty())
		{
			g_theConsole->m_insertionPointBlinkTimer->Start();

			g_theConsole->m_historyIndex > (int)g_theConsole->m_commandHistory.size() - 2 ?
				g_theConsole->m_historyIndex = (int)g_theConsole->m_commandHistory.size() - 1 : g_theConsole->m_historyIndex += 1;
			g_theConsole->m_inputText = g_theConsole->m_commandHistory[g_theConsole->m_historyIndex];
			g_theConsole->m_insertionPointPosition = (int)g_theConsole->m_inputText.size();
		}
	}

	// Handle "home/end" to go to the beginning or end of the input text
	if (keyCode == KEYCODE_HOME)
	{
		g_theConsole->m_insertionPointBlinkTimer->Start();
		g_theConsole->m_insertionPointPosition = 0;
	}

	if (keyCode == KEYCODE_END)
	{
		g_theConsole->m_insertionPointBlinkTimer->Start();
		g_theConsole->m_insertionPointPosition = (int)g_theConsole->m_inputText.size();
	}

	// Handle "Backspace/Delete key" when press these two keys will delete the character
	if (keyCode == KEYCODE_BACKSPACE)
	{
		if ((int)g_theConsole->m_inputText.size() != 0)
		{
			g_theConsole->m_inputText.erase(g_theConsole->m_inputText.begin() + g_theConsole->m_insertionPointPosition - 1);
			g_theConsole->m_insertionPointBlinkTimer->Start();
			g_theConsole->m_insertionPointPosition < 1 ? g_theConsole->m_insertionPointPosition = 0 : g_theConsole->m_insertionPointPosition -= 1;
		}
	}

	if (keyCode == KEYCODE_DELETE)
	{
		if ((int)g_theConsole->m_inputText.size() != 0 && g_theConsole->m_insertionPointPosition != (int)g_theConsole->m_inputText.size())
		{
			g_theConsole->m_inputText.erase(g_theConsole->m_inputText.begin() + g_theConsole->m_insertionPointPosition);
			g_theConsole->m_insertionPointBlinkTimer->Start();
		}
	}

	return true;
}

bool DevConsole::Event_CharInput(const EventArgs& args)
{
	if (!g_theConsole->m_isOpen)
	{
		return false;
	}
	unsigned char inputValue = (unsigned char)args.GetValue("CharName",-1);

	if (inputValue == '`' || inputValue == '~')
	{
		return false;
	}

	if (inputValue >= 32 && inputValue <= 126)
	{
		g_theConsole->m_inputText.insert(g_theConsole->m_inputText.begin() + g_theConsole->m_insertionPointPosition,inputValue);
		g_theConsole->m_insertionPointPosition += 1;
		g_theConsole->m_insertionPointBlinkTimer->Start();
		return true;
	}

	return false;

}

bool DevConsole::Command_Clear(const EventArgs& args)
{
	UNUSED(args);
	if (!g_theConsole->m_isOpen)
	{
		return false;
	}

	g_theConsole->m_lines.clear();
	return true;
}

bool DevConsole::Command_Echo(const EventArgs& args)
{
	if (!g_theConsole->m_isOpen)
	{
		return false;
	}

	const std::string& inputText = args.GetValue("InputText", "");

	Strings eventNameAndArguments = SplitStringOnDelimiter(inputText, ' ');

	std::string& eventName = eventNameAndArguments[0];
	g_theConsole->m_commandHistory.push_back(eventName);
	g_theConsole->m_historyIndex = (int)g_theConsole->m_commandHistory.size();

	if ((int)eventNameAndArguments.size() > 1)
	{
		EventArgs keyValuePairs;
		for (size_t i = 1; i < eventNameAndArguments.size(); i++)
		{
			std::string& eventKeyValue = eventNameAndArguments[i];
			Strings eventKeyValueRawStrings = SplitStringOnDelimiter(eventKeyValue, '=');
			if ((int)eventKeyValueRawStrings.size() < 2)
			{
				g_theConsole->AddLine(DevConsole::ERROR, "Correct input format for event with arguments should be:\"eventname argname1=argvalue1\", should be all lower case!");
				return false;
			}
			keyValuePairs.SetValue(eventKeyValueRawStrings[0], eventKeyValueRawStrings[1]);
		}
		g_theEventSystem->FireEvent(eventName, keyValuePairs);
	}
	else if ((int)eventNameAndArguments.size() == 1)
	{
		if (eventName == "quit")
		{
			g_theEventSystem->FireEvent("Close");
			return true;
		}
		else
		{
			g_theConsole->AddLine(DevConsole::COMMAND_ECHO, "Command Works!");
			g_theEventSystem->FireEvent(eventName);
			return true;
		}
	}

	return false;
}

bool DevConsole::Command_Help(const EventArgs& args)
{
	UNUSED(args);

	if (!g_theConsole->m_isOpen)
	{
		return false;
	}

	const Strings& commandList = g_theEventSystem->GetAllDevConsoleRegisteredCommands();

	for (size_t i = 0; i < commandList.size(); ++i)
	{
		g_theConsole->AddLine(DevConsole::INFO_MINOR,commandList[i]);
	}
	return true;
}
