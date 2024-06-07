#include "Game/Pickup.hpp"
#include "Game/Game.hpp"

extern Renderer* g_theRenderer;

Pickup::Pickup(Game* gameInstance, Vec2 startPosition)
{
	m_game = gameInstance;
	m_position = startPosition;
}

void Pickup::StartUp()
{
	m_health = 1;
	m_speed = 0.f;
	m_rng = new RandomNumberGenerator();
	m_color = Rgba8(0, 255, 0, 255);
	m_liftTimer = PICKUP_LIFTTIME;
	m_physicsRadius = PICKUP_PHYSICS_RADIUS;
	m_cosmeticRadius = PICKUP_COSMETIC_RADIUS;
}

void Pickup::Update()
{
	UpdatePickupLifetime();
}

void Pickup::UpdatePickupLifetime()
{
	if (m_liftTimer>0.f) 
	{
		m_liftTimer -= m_game->m_gameClock->GetDeltaSeconds();

		if (m_liftTimer < 2.0f)
		{
			m_color.a = static_cast<unsigned char>(m_rng->RollRandomIntRange(0, 255));
		}
	}
	else 
	{
		Die();
	}
}

void Pickup::Render()
{
	Vertex_PCU waspVertsGlobal[12]
	{
		Vertex_PCU(2.f, 0.5f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.f, -0.5f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-2.f, 0.5f,	m_color.r, m_color.g, m_color.b, m_color.a),
								
		Vertex_PCU(-2.f, 0.5f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.f, -0.5f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-2.f, -0.5f,	m_color.r, m_color.g, m_color.b, m_color.a),
								
		Vertex_PCU(0.5f, 2.f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(0.5f, -2.f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-0.5f, 2.f,	m_color.r, m_color.g, m_color.b, m_color.a),
								
		Vertex_PCU(-0.5f, -2.f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(0.5f, -2.f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-0.5f, 2.f,	m_color.r, m_color.g, m_color.b, m_color.a),
	};
	TransformVertexArrayXY3D(12, waspVertsGlobal, 0.5f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(12, waspVertsGlobal);
}

void Pickup::GetDamage(int damage)
{
	m_health -= damage;

	if (m_health <= 0)
	{
		Die();
	}
}

void Pickup::Die()
{
	m_health = 0;
	m_isDead = true;
	m_isGarbage = true;
}

