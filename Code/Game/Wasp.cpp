#include "Game/Wasp.hpp"
#include "Game/Game.hpp"

extern Renderer* g_theRenderer;
extern AudioSystem* g_theAudio;
Wasp::Wasp(Game* gameInstance, Vec2 startPosition)
{
	m_game = gameInstance;
	m_position = startPosition;
}

void Wasp::StartUp()
{
	m_health = 3;
	m_isPlayerAlive = true;
	m_speed = 0.f;
	m_rng = new RandomNumberGenerator();
	m_color = Rgba8(255, 255, 0, 255);
	m_physicsRadius = WASP_PHYSICS_RADIUS;
	m_cosmeticRadius = WASP_COSMETIC_RADIUS;
}

void Wasp::Update()
{
	if (m_isPlayerAlive) 
	{
		m_faceDirection = (m_followLocation - m_position).GetNormalized();
		m_orientationDegrees = m_faceDirection.GetOrientationDegrees();
	}
	m_speed = WASP_ACCELERATION * m_game->m_gameClock->GetDeltaSeconds();
	m_speed = GetClamped(m_speed, 0.0f, 10.0f);
	m_velocity += m_faceDirection * m_speed;
	m_position += m_velocity * m_game->m_gameClock->GetDeltaSeconds();
}

void Wasp::Render()
{
	Vertex_PCU waspVertsGlobal[12]
	{
		Vertex_PCU(-1.f, 0.f,	255, 255, 0, 255),
		Vertex_PCU(0.f, -0.5f,	255, 255, 0, 255),
		Vertex_PCU(0.f, 0.5f,	255, 255, 0, 255),
								
		Vertex_PCU(0.f, -0.5f,	255, 255, 0, 255),
		Vertex_PCU(2.f,	0.f,	255, 255, 0, 255),
		Vertex_PCU(0.f, 0.5f,	255, 255, 0, 255),
								
		Vertex_PCU(-0.5f, 1.f,	255, 255, 0, 255),
		Vertex_PCU(0.f, 0.5f,	255, 255, 0, 255),
		Vertex_PCU(+2.f, 0.f,	255, 255, 0, 255),
								
		Vertex_PCU(-0.5f, -1.f,	255, 255, 0, 255),
		Vertex_PCU(+2.f, 0.f,	255, 255, 0, 255),
		Vertex_PCU(0.f, -0.5f,	255, 255, 0, 255),
	};
	TransformVertexArrayXY3D(12, waspVertsGlobal, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(12, waspVertsGlobal);
}

void Wasp::GetDamage(int damage)
{
	m_health -= damage;
	SoundID attractSound = g_theAudio->CreateOrGetSound("Data/Audio/EnemyHit.wav");
	g_theAudio->StartSound(attractSound);
	g_theAudio->StopSound(attractSound);

	if (m_health <= 0)
	{
		Die();
	}
}

void Wasp::Die()
{
	int debrisNum = m_rng->RollRandomIntRange(3, 12);
	SoundID attractSound = g_theAudio->CreateOrGetSound("Data/Audio/EnemyDied.wav");
	g_theAudio->StartSound(attractSound);
	g_theAudio->StopSound(attractSound);
	m_game->SpawnMultipleDebrisForEntity(m_position, debrisNum, m_color, DEBRIS_COSMETIC_RADIUS,m_velocity);
	m_health = 0;
	m_isDead = true;
	m_isGarbage = true;
}

void Wasp::FollowPlayer(const Vec2& location)
{
	m_followLocation = location;
}

void Wasp::SetPlayerStatus(bool playAlive)
{
	m_isPlayerAlive = playAlive;
}
