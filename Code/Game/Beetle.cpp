#include "Game/Beetle.hpp"
#include "Game/Game.hpp"

extern Renderer* g_theRenderer;
extern AudioSystem* g_theAudio;
Beetle::Beetle(Game* gameInstance, Vec2 startPosition)
{
	m_game = gameInstance;
	m_position = startPosition;
}

void Beetle::StartUp()
{
	m_health = 3;
	m_speed = 8;
	m_color = Rgba8(0, 255, 0, 255);
	m_rng = new RandomNumberGenerator();
	m_isPlayerAlive = true;
	m_physicsRadius = BEETLE_PHYSICS_RADIUS;
	m_cosmeticRadius = BEETLE_COSMETIC_RADIUS;
}

void Beetle::Update()
{
	if (m_isPlayerAlive) 
	{
		m_faceDirection = (m_followLocation - m_position).GetNormalized();
		m_orientationDegrees = m_faceDirection.GetOrientationDegrees();
	}
	m_velocity = m_faceDirection * m_speed;
	m_position += m_velocity * m_game->m_gameClock->GetDeltaSeconds();
}

void Beetle::Render()
{
	Vertex_PCU beetleVertsGlobal[6]
	{
		Vertex_PCU(-1.f, -1.5f,	0, 255, 0, 255),
		Vertex_PCU(1.f, -1.f,	0, 255, 0, 255),
		Vertex_PCU(+1.f, 1.f,	0, 255, 0, 255),

		Vertex_PCU(-1.f, -1.5f,	0, 255, 0, 255),
		Vertex_PCU(1.f,	1.f,	0, 255, 0, 255),
		Vertex_PCU(-1.f, 1.5f,	0, 255, 0, 255),
	};
	TransformVertexArrayXY3D(6, beetleVertsGlobal, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(6, beetleVertsGlobal);
}

void Beetle::GetDamage(int damage)
{
	m_health -= damage;
	SoundID attractSound = g_theAudio->CreateOrGetSound("Data/Audio/EnemyHit.wav");
	g_theAudio->StartSound(attractSound);
	if (m_health <= 0)
	{
		Die();
	}
}

void Beetle::Die()
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

void Beetle::FollowPlayer( const Vec2& location)
{
	m_followLocation = location;
}

void Beetle::SetPlayerStatus(bool playAlive)
{
	m_isPlayerAlive = playAlive;
}
