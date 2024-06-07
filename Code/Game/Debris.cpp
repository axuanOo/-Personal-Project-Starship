#include "Game/Debris.hpp"
#include "Game/Game.hpp"
extern Renderer* g_theRenderer;

Debris::Debris(Game* gameInstance, Vec2 startPos)
{
	m_game = gameInstance;
	m_position = startPos;
}

Debris::~Debris()
{

}

void Debris::StartUp()
{
	m_rng = new RandomNumberGenerator();

	float faceDegrees = m_rng->RollRandomFloatInRange(-180.f, 180.f);
	m_faceDirection = Vec2(1.f, 0.f);
	m_faceDirection.RotateDegrees(faceDegrees);
	m_speed = m_rng->RollRandomFloatInRange(10.f,15.f);

	m_shape = m_rng->RollRandomIntRange(7, MAX_DEBRIS_EDGES);
	float thetaDegrees = 360.f / m_shape;
	float thetaRadians = ConvertDegreeToRadians(thetaDegrees);

	for (int shapeIndex = 0; shapeIndex < m_shape; shapeIndex++)
	{
		float r = m_rng->RollRandomFloatInRange(m_cosmeticRadius*0.3f, m_cosmeticRadius);

		float theta = thetaRadians * shapeIndex;

		m_randomShape[shapeIndex] = Vec2(cosf(theta) * r, sinf(theta) * r);
	}
}

void Debris::Update()
{
	m_velocity = m_speed * m_faceDirection;
 	m_velocity += m_initialVelocity;
	m_position += m_velocity * m_game->m_gameClock->GetDeltaSeconds();
	if (m_trans<2) 
	{
		m_trans = 0;
	}
	else 
	{
		m_trans -= static_cast<unsigned char>(63.5f * m_game->m_gameClock->GetDeltaSeconds());
	}

	if (m_trans < 0.f)
	{
		Die();
	}

	if (m_position.x + m_cosmeticRadius < 0.f)
	{
		Die();
	}
	if (m_position.x - m_cosmeticRadius > WORLD_SIZE_X)
	{
		Die();
	}
	if (m_position.y + m_cosmeticRadius < 0.f)
	{
		Die();
	}
	if (m_position.y - m_cosmeticRadius > WORLD_SIZE_Y)
	{
		Die();
	}
}

void Debris::Render()
{

	for (int shapeIndex = 0; shapeIndex < m_shape; shapeIndex++)
	{
		if (shapeIndex == m_shape - 1)
		{
			Vertex_PCU asteroidVerts[3] =
			{
				Vertex_PCU(m_randomShape[shapeIndex].x, m_randomShape[shapeIndex].y,			m_color.r, m_color.g, m_color.b, m_trans),
				Vertex_PCU(0.f, 0.f,															m_color.r, m_color.g, m_color.b, m_trans),
				Vertex_PCU(m_randomShape[0].x, m_randomShape[0].y,								m_color.r, m_color.g, m_color.b, m_trans)
			};
			TransformVertexArrayXY3D(3, asteroidVerts, 1.f, m_orientationDegrees, m_position);
			g_theRenderer->DrawVertexArray(3, asteroidVerts);
		}
		else
		{
			Vertex_PCU asteroidVerts[3] =
			{
				Vertex_PCU(m_randomShape[shapeIndex].x, m_randomShape[shapeIndex].y,			m_color.r, m_color.g, m_color.b, m_trans),
				Vertex_PCU(0.f, 0.f,															m_color.r, m_color.g, m_color.b, m_trans),
				Vertex_PCU(m_randomShape[shapeIndex + 1].x, m_randomShape[shapeIndex + 1].y,	m_color.r, m_color.g, m_color.b, m_trans)
			};
			TransformVertexArrayXY3D(3, asteroidVerts, 1.f, m_orientationDegrees, m_position);
			g_theRenderer->DrawVertexArray(3, asteroidVerts);
		}
	}
}

void Debris::Die()
{
	m_isDead = true;
	m_isGarbage = true;
}

void Debris::GetDamage(int damageValue)
{
	damageValue =0;
}


void Debris::SetColor(Rgba8& color)
{
	m_color = color;
}

