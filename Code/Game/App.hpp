#pragma once
#include "Game/Game.hpp"


class App
{
public:
	App();
	~App();
	void		Startup();
	void		BeginFrame();
	void		RunFrame();
	void		Update();
	void		Render() const;
	void		EndFrame();
	void		Shutdown();
	void		ClearGameInstance();
	void		ReloadGame(); 


	bool		IsQuitting			()	const { return m_isQuitting; }
	void		HandleKeyPressed	(unsigned char keyCode);
	void		HandleKeyReleased	(unsigned char keyCode);
	void		HandleQuitRequested ();
	static bool		HandleQuitRequest(EventArgs const& args);


	Game*		m_theGame;

private:

	//void		UpdateShip(float delataSeconds);
	//void		RenderShip() const;

private:

	bool		m_isQuitting = false;
	bool		m_isPaused = false;
	bool		m_isSlowMo = false;

};

