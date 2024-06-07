#pragma once
#include "Game/GameCommon.hpp"
//#include "Game/Game.hpp"

class Game;

class Entity //build for abstract base class
{

public:
	Entity();
	Entity( Game* gameInstance, Vec2 startPosition);
	~Entity();

	virtual void	Update() = 0;
	virtual void	Render() = 0;
	virtual void	Die() = 0;
	virtual void	GetDamage(int damageValue) = 0 ;

	bool			IsIsOffscreen();
	Vec2			GetForwardNormal() ;
	bool			IsAlive();

	Vec2			m_position;									//	the Entity's 2D(x, y) Cartesian origin / center location, in world space
	Vec2			m_velocity;									//	the Entity's linear 2D(x, y) velocity, in world units per second
	Vec2			m_faceDirection;							//	face direction of this starship
	float			m_speed;
	float			m_orientationDegrees;						//	its forward direction angle, in degrees(counter - clock.from + x / east)
	float			m_angularVelocity;							//	the Entity's signed angular velocity(spin rate), in degrees per second
	float			m_physicsRadius;							//	the Entity's(inner, conservative) disc - radius for all physics purposes
	float			m_cosmeticRadius;							//	the Entity's(outer, liberal) disc - radius that encloses all of its vertexes
	int				m_health;									//	how many “hits” the entity can sustain before dying
	bool			m_isDead;									//	whether the Entity is “dead” in the game; affects entityand game logic
	bool			m_isGarbage;								//	whether the Entity should be deleted at the end of Game::Update()
	Game*			m_game;										//	a pointer back to the Game instance
	float			m_score=0.f;

private:



};

