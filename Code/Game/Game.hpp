#pragma once
#include "Game/GameCommon.hpp"
#include "Game/PlayerShip.hpp"
#include "Game/Asteroid.hpp"
#include "Game/Bullet.hpp"
#include "Game/Beetle.hpp"
#include "Game/Wasp.hpp"
#include "Game/Debris.hpp"
#include "Game/Missile.hpp"
#include "Game/Pickup.hpp"
#include "Game/UIElements.hpp"

class Game
{
public:

	Game(App* const& app);
	~Game();
	
	void						StartUp();
	void						ShutDown();

	void						SpawnAttractMode();
	void						SpawnAsteroidForAttractMode();
	void						SpawnStarsPosition();
	void						SpawnPlayerShip();								// Spawn a new player ship for the whole game, if exists then return 0
	void						SpawnPickUps(Vec2& spawnPosition);
	void						SpawnAsteroidAtBeginning();						// Spawn an asteroid for the game 
	void						SpawnOneAsteroid();
	void						SpawnBullet(Vec2& firePosition);									// Spawn a bullet for the player ship
	void						SpawnMissile(Vec2& firePosition);
	void						SpawnBeetle();									// Spawn beetle one enemy
	void						SpawnWasp();
	void						SpawnPlayerUI();
	void						SpawnMultipleDebrisForEntity(Vec2& spawnPosition, int spawnNumber, Rgba8& color, float cosmeticRadius, Vec2& initVelocity);
	
	void						GenerateWaves(int waveIndex);

	void						Update();
	void						UpdateAttractMode();
	void						UpdateBullets();
	void						UpdateMissile();
	void						UpdateMissileAim(Missile& missile);
	void						UpdateAsteroids();
	void						UpdateEnemies();
	void						UpdateBeetles();
	void						UpdatePickups();
	void						UpdateDebris();
	void						UpdateWasps();
	void						UpdateEntities();


	void						Render() const;
	void						RenderAttractMode() const;
	void						RenderBackGroundStars() const;
	void						RenderPlayerShip() const;
	void						RenderAsteroids() const;
	void						RenderBullets() const;
	void						RenderMissiles() const;
	void						RenderBeetles() const;
	void						RenderWasps() const;
	void						RenderDebris() const;
	void						RenderPickups() const;
	void						RenderPlayerUI() const;
	void						RenderPlayerLives() const;
	void						RenderPlayerMissileCoolDown() const;
	void						RenderDeveloperMode() const;

	void						CheckCollision();
	void						CheckStarshipCollisionWithEdges();
	void						CheckStarShipCollisionWithEntities();
	void						CheckBulletCollisionWithEdges();
	void						CheckMissileCollisionWithEdges();
	void						CheckMissileCollisionWithEntities();
	void						CheckBulletCollisionWithEntities();
	void						CheckAttractAsteroidCollisionWithEdges();
	void						CheckAsteroidCollisionWithEdges();

	Vec2						GeneratePositionOffscreen( float const& cosmeticRadius, bool onlyTopRight);		// return a position in world space which is off screen

	void						InvokeEndTimer();
	void						ResetEndTimer();
	void						DeleteAllGarbage();

	PlayerShip*	const			GetPlayerShip() const;					// Return the address of the m_playership

	bool						m_invokeEndTimer = false;
	static bool				ChangeTimeScale(EventArgs const& args);
	Clock*					m_gameClock;
private:
	Camera*					m_worldCamera;
	Camera*					m_screenCamera;
	Camera*					m_attractCamera;

	bool					m_enterAttractMode = true;
	float					m_endTimer;

	bool					m_developerMode = false;

	int						m_waveindex = 0;

	PlayerShip*				m_Playership = nullptr;

	Asteroid*				m_asteroidsForAttractMode[MAX_ASTEROIDS_ATTRACTMODE] = {};

	Asteroid*				m_asteroids[MAX_ASTEROIDS] = {};
	Bullet*					m_bullets[MAX_BULLETS] = {};
	Missile*				m_missile[MAX_MISSILE] = {};
	Beetle*					m_beetle[MAX_BEETLES] ={};
	Wasp*					m_wasps[MAX_WASPS] = {};
	Debris*					m_debris[MAX_DEBRIS] = {};
	Pickup*					m_pickups[MAX_PICKUP] = {};
	Vertex_PCU				m_stars[MAX_STARS_NUMBER] = {};

	Entity*					m_entities[MAX_ENTITIES]={};

	UIElements*				m_attractMode[3]={};
	UIElements*				m_playerUiTool;

	RandomNumberGenerator*	m_rng = nullptr;


};