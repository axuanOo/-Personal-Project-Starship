#include "Game/Bullet.hpp"
#include "Game/Game.hpp"


extern Renderer* g_theRenderer;
Bullet::Bullet()
{

}

Bullet::Bullet(Game* gameInstance, Vec2 startPos)
{
	m_game = gameInstance;
	m_position = startPos;
	m_health=1;
}

Bullet::~Bullet()
{

}

void Bullet::StartUp()
{
	m_faceDirection = Vec2(1.f, 0.f);
	m_physicsRadius = BULLET_PHYSICS_RADIUS;
	m_cosmeticRadius = BULLET_COSMETIC_RADIUS;

}

void Bullet::Update()
{
	UpdateNormalMoveMode();
}

void Bullet::UpdateNormalMoveMode()
{
	m_position += GetForwardNormal() * BULLET_SPEED * m_game->m_gameClock->GetDeltaSeconds();
}


void Bullet::Render()
{
	Vertex_PCU bulletVertsGlobal[6]
	{
		Vertex_PCU(-2.f, 0.f,	255, 0, 0, 0),
		Vertex_PCU(0.f, -0.5f,	255, 0, 0, 255),
		Vertex_PCU(0.5f, 0.f,	255, 255, 0, 255),

		Vertex_PCU(-2.f, 0.f,	255, 0, 0, 0),
		Vertex_PCU(0.5f, 0.f,	255, 255, 0, 255),
		Vertex_PCU(0.f, 0.5f,	255, 0, 0, 255)

	};

	TransformVertexArrayXY3D(6, bulletVertsGlobal, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(6, bulletVertsGlobal);

	return;
}

void Bullet::Die()
{
	m_health = 0;
	m_isDead = true;
	m_isGarbage = true;
}

void Bullet::GetDamage(int damageValue)
{
	m_health -= damageValue;

	if (m_health <= 0)
	{
		Die();
	}
}

