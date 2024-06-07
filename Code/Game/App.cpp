#include "Game/App.hpp"
#include "GameCommon.hpp"
#include "Engine/Core/Time.hpp"

App::App()
{

}

App::~App()
{

}

void App::Startup()
{
	// Create engine subsystems and game
	DevConsoleConfig devConsoleConfig;
	devConsoleConfig.m_font = std::string("Data/Fonts/SquirrelFixedFont");
	devConsoleConfig.m_numLines = 60.0f;
	g_theConsole = new DevConsole(devConsoleConfig);

	InputConfig inputConfig;
	g_theInput =  new InputSystem( inputConfig );

	WindowConfig windowConfig;
	windowConfig.m_inputSystem = g_theInput;
	windowConfig.m_windowTitle = "SD1_A4: Starship Gold";
	windowConfig.m_clientAspect = 2.0f;
	g_theWindow = new Window(windowConfig);

	RenderConfig renderConfig;
	renderConfig.m_window = g_theWindow;
	g_theRenderer = new Renderer(renderConfig);

	AudioConfig audioConfig;
	g_theAudio = new AudioSystem(audioConfig);

	EventSystemConfig eventSystemConfig;
	g_theEventSystem = new EventSystem(eventSystemConfig);

	g_theEventSystem->SubscribeEventCallbackFunction("Close", App::HandleQuitRequest);

	m_theGame = new Game(this);

	// Start up engine subsystems and game
	g_theEventSystem->StartUp();
	g_theInput->StartUp();
	g_theWindow->Startup();
	g_theRenderer->Startup();
	g_theAudio->Startup();
	g_theConsole->Startup();
	m_theGame->StartUp();

	g_theConsole->AddLine(DevConsole::WARNING, "Please input 'timescale value=xx.x', be aware of the space and '='!");
	g_theConsole->AddLine(DevConsole::WARNING, "Press E to move forward");
	g_theConsole->AddLine(DevConsole::WARNING, "Press S/F to rotate the plane");
	g_theConsole->AddLine(DevConsole::WARNING, "Press space to fire a bullet");
	g_theConsole->AddLine(DevConsole::WARNING, "Press Esc to quit game");
	g_theConsole->AddLine(DevConsole::WARNING, "Press I to instantiate a new asteroid");
	g_theConsole->AddLine(DevConsole::WARNING, "Press F1 to enable the debugger mode");
	g_theConsole->AddLine(DevConsole::WARNING, "Press L to switch weapon");

}

void App::RunFrame()
{
	while (!IsQuitting())
	{
		BeginFrame();
		Update();
		Render();
		EndFrame();
	}
}

void App::BeginFrame()
{
	g_theInput->BeginFrame();
	g_theWindow->BeginFrame();
	g_theRenderer->BeginFrame();
	g_theAudio->BeginFrame();
	g_theConsole->BeginFrame();

	if (g_theInput->IsKeyDown(KEYCODE_F8) && !g_theInput->WasKeyJustPressed(KEYCODE_F8))
	{
		ReloadGame();
	}

	if (g_theInput->IsKeyDown(KEYCODE_TILDE) && !g_theInput->WasKeyJustPressed(KEYCODE_TILDE))
	{
		g_theConsole->ToggleMode();
	}

	if (g_theInput->IsKeyDown('T') && !g_theInput->WasKeyJustPressed('T'))
	{
		if (Clock::GetSystemClock().GetTimeScale() == 0.1f)
		{
			Clock::GetSystemClock().SetTimeScale(1.0f);
		}
		else
		{
			Clock::GetSystemClock().SetTimeScale(0.1f);
		}
	}

	if (g_theInput->IsKeyDown('P') && !g_theInput->WasKeyJustPressed('P'))
	{
		Clock::GetSystemClock().TogglePause();
	}

	if (g_theInput->IsKeyDown('O'))
	{
		Clock::GetSystemClock().StepSingleFrame();
	}
}

void App::Update()
{
	m_theGame->Update();
}

void App::Render() const
{
	if (IsQuitting())
	{
		return;
	}
	g_theRenderer->SetModelConstants();
	m_theGame->Render();

	float cameraAspect = g_theWindow->GetAspect();
	AABB2 cameraBox = AABB2(Vec2(0.0f, 0.0f), Vec2(100.0f * cameraAspect, 100.0f));
	Camera devConsoleCamera;
	devConsoleCamera.SetOrthographicView(Vec2(0.0f, 0.0f), Vec2(100.0f * cameraAspect, 100.0f));
	g_theRenderer->BeginCamera(devConsoleCamera);
	g_theConsole->Render(cameraBox, g_theRenderer);
	g_theRenderer->EndCamera(devConsoleCamera);
}


void App::EndFrame()
{
	if (m_isQuitting)
	{
		return;
	}
	g_theInput->EndFrame();
	g_theWindow->EndFrame();
	g_theRenderer->EndFrame();
	g_theAudio->EndFrame();
	g_theConsole->EndFrame();
}

void App::Shutdown()
{
	m_theGame->ShutDown();
	g_theConsole->Shutdown();
	g_theInput->Shutdown();
	g_theRenderer->Shutdown();
	g_theWindow->Shutdown();
	g_theAudio->Shutdown();
	m_isQuitting = true;
}


void App::ClearGameInstance()
{
	m_theGame = nullptr;
}

void App::ReloadGame()
{
	m_theGame->ShutDown();

	m_theGame = new Game(this);

	m_theGame->StartUp();
}


void App::HandleKeyPressed(unsigned char keyCode)
{
	g_theInput->HandleKeyPressed(keyCode);
}

void App::HandleKeyReleased(unsigned char keyCode)
{
	g_theInput->HandleKeyReleased(keyCode);
}

void App::HandleQuitRequested()
{
	Shutdown();
}

bool App::HandleQuitRequest(EventArgs const& args)
{
	UNUSED(args);
	g_theApp->HandleQuitRequested();
	return false;
}

