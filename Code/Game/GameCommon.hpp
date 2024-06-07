#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/SimpleTriangleFont.hpp"

class App;

extern	Window* g_theWindow;
extern  Renderer* g_theRenderer;
extern  InputSystem* g_theInput;
extern  AudioSystem* g_theAudio;
extern  App*		 g_theApp;

constexpr int PLAYER_LIVES = 4;
constexpr int PLAYER_HP = 100;
constexpr int NUM_STARTING_ASTEROIDS = 6;
constexpr int MAX_ASTEROIDS_ATTRACTMODE = 8;
constexpr int MAX_ASTEROIDS = 40;
constexpr int MAX_BULLETS = 50;
constexpr int MAX_MISSILE =20;
constexpr int MAX_BEETLES = 20;
constexpr int MAX_WASPS = 20;
constexpr int MAX_DEBRIS = 100;
constexpr int MAX_PICKUP = 5;
constexpr int MAX_ENTITIES = 50;
constexpr int MAX_DEBRIS_EDGES = 10;
constexpr int MAX_STARS_NUMBER = 20;

constexpr float WEAPON_MAX_CONTAING_TIME = 6.0f;
constexpr float ATTRACTMODE_ROTATE_SPEED = 15.0f;

constexpr float WORLD_SIZE_X = 200.f;
constexpr float WORLD_SIZE_Y = 100.f;
constexpr float WORLD_CENTER_X = WORLD_SIZE_X / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_SIZE_Y / 2.f;

constexpr float ASTEROID_SPEED = 10.f;
constexpr float ASTEROID_PHYSICS_RADIUS = 1.6f;
constexpr float ASTEROID_COSMETIC_RADIUS = 2.0f;
constexpr int	ASTEROID_DAMAGE = 20;

constexpr float BULLET_LIFETIME_SECONDS = 2.0f;
constexpr float BULLET_SPEED = 50.f;
constexpr float BULLET_PHYSICS_RADIUS = 0.5f;
constexpr float BULLET_COSMETIC_RADIUS = 2.0f;
constexpr int	BULLET_DAMAGE = 1;

constexpr float MACHINEGUN_TIMER = 0.2f;

constexpr float MISSILE_LIFETIME_SECONDS = 10.0f;
constexpr int	MISSILE_DEFAULT_NUMBER = 2;
constexpr float	MISSILE_COOLDOWN_TIME = 1.5f;
constexpr float MISSILE_SPEED = 40.f;
constexpr float MISSILE_MAX_ROTATION_SPEED = 2.f;
constexpr float MISSILE_PHYSICS_RADIUS = 1.0f;
constexpr float MISSILE_COSMETIC_RADIUS = 4.0f;
constexpr float	MISSILE_DETECTING_MAX_ANGLE = 60.f;
constexpr float MISSILE_MAX_DETECTING_DISTANCE = 50.f;
constexpr int	MISSILE_MAX_DETECTING_NUM = 50;
constexpr float	MISSILE_MIN_DETECTING_SCORE = 80.0f;
constexpr float MISSILE_EXPLOSION_RADIUS = 1.5f;
constexpr int   MISSILE_EXPLOSION_EDGES =12;
constexpr float	MISSILE_EXPLOSION_DELAY = 1.0f;
constexpr int	MISSILE_DAMAGE = 3;

constexpr float BEETLE_PHYSICS_RADIUS = 1.0f;
constexpr float BEETLE_COSMETIC_RADIUS = 2.0f;
constexpr	int BEETLE_DAMAGE = 50;

constexpr float WASP_PHYSICS_RADIUS = 1.0f;
constexpr float WASP_COSMETIC_RADIUS = 2.0f;
constexpr float WASP_ACCELERATION = 10.f;
constexpr int	WASP_DAMAGE = 35;

constexpr float PICKUP_PHYSICS_RADIUS = 1.0f;
constexpr float PICKUP_COSMETIC_RADIUS = 2.0f;
constexpr float	PICKUP_LIFTTIME = 5.0f;

constexpr float DEBRIS_COSMETIC_RADIUS = 1.5f;

constexpr float PLAYER_SHIP_ACCELERATION = 30.f;
constexpr float PLAYER_SHIP_TURN_SPEED = 300.f;
constexpr float PLAYER_SHIP_PHYSICS_RADIUS = 1.75f;
constexpr float PLAYER_SHIP_COSMETIC_RADIUS = 2.25f;
constexpr float PLAYER_END_MAX_TIMER = 3.0f;
constexpr float PLAYER_FLAME_PARAMETER = 4.0f;

const Rgba8 CLEAR_COLOR =Rgba8(0, 0, 0, 255);
const Vec2	WORLD_CAMERA_BOTTOMLEFT = Vec2(0.f,0.f);
const Vec2	WORLD_CAMERA_TOPRIGHT = Vec2(200.f,100.f);
const Vec2	SCREEN_CAMERA_BOTTOMLEFT = Vec2(0.f, 0.f);
const Vec2	SCREEN_CAMERA_TOPRIGHT = Vec2(1600.f, 800.f);
const Vec2	ATTRACT_CAMERA_BOTTOMLEFT = Vec2(0.f, 0.f);
const Vec2	ATTRACT_CAMERA_TOPRIGHT = Vec2(200.f, 100.f);

void DebugDrawRing( Vec2 const& Center, float radius, float thickness, Rgba8 const& color);
void DebugDrawLine( Vec2 const& startPos , Vec2 const& endPos, float thickness,Rgba8 const& color);
void DebugDrawGlow( Vec2 const& Center, float radius, float thickness, Rgba8 const& color);




