#include "Game/Missile.hpp"
#include "Game/Game.hpp"


extern Renderer* g_theRenderer;
Missile::Missile()
{

}

Missile::Missile(Game* gameInstance, Vec2 startPos)
{
	m_game = gameInstance;
	m_position = startPos;
}

Missile::~Missile()
{

}

void Missile::StartUp()
{
	m_rng = new RandomNumberGenerator();
	m_startPosition = m_position;
	m_health=1;
	m_scale = 0.7f;
	m_radius = 0.0f;
	m_speed = MISSILE_SPEED;
	m_physicsRadius = MISSILE_PHYSICS_RADIUS;
	m_cosmeticRadius = MISSILE_COSMETIC_RADIUS;
}

void Missile::Update()
{
	if (m_isDead&&!m_isGarbage) 
	{
		if (m_brokenTimer<MISSILE_EXPLOSION_DELAY) 
		{
			m_brokenTimer+=m_game->m_gameClock->GetDeltaSeconds();

			float thetaDegrees = 360.f / MISSILE_EXPLOSION_EDGES;
			float proportion  = GetClampedZeroToOne(m_brokenTimer*10.0f/MISSILE_EXPLOSION_DELAY);
			m_radius = proportion * MISSILE_EXPLOSION_RADIUS;
			float thetaRadians = ConvertDegreeToRadians(thetaDegrees);

			for (int shapeIndex = 0; shapeIndex < MISSILE_EXPLOSION_EDGES; shapeIndex++)
			{
				float r = m_rng->RollRandomFloatInRange(m_radius * 0.4f, m_radius);

				float theta = thetaRadians * shapeIndex;

				m_randomShape[shapeIndex] = Vec2(cosf(theta) * r, sinf(theta) * r);
			}

			return;
		}
		else 
		{
			m_brokenTimer =0.0f;
			m_isGarbage = true;
		}
	}

	if (m_target != nullptr) 
	{
		Vec2 uncorrectedDirection = m_target->m_position - m_position;
		uncorrectedDirection.Normalized();

		UpdateRotationDirection(uncorrectedDirection);

		m_positionLerpValue+=m_game->m_gameClock->GetDeltaSeconds();
		float proprotion = GetClampedZeroToOne(m_positionLerpValue);
		m_position = Interpolate(m_startPosition,m_target->m_position,proprotion);

		return;

	}
	else 
	{
		m_velocity = m_speed * m_faceDirection;
		m_position += m_velocity * m_game->m_gameClock->GetDeltaSeconds();
		m_startPosition = m_position;
	}
}

void Missile::UpdateRotationDirection(Vec2& uncorrectedVector)
{
	float newDirectionAngle = uncorrectedVector.GetOrientationDegrees();
	m_orientationDegrees = GetTurnedTowardDegrees(m_orientationDegrees, newDirectionAngle, MISSILE_MAX_ROTATION_SPEED);
	m_faceDirection = Vec2(1.0f, 0.0f).GetRotatedDegrees(m_orientationDegrees);
}

void Missile::Render()
{

	if (m_isDead&&!m_isGarbage)
	{
		RenderExplosion();
		return;
	}
	RenderMissile();
	RenderFlame();
}

void Missile::RenderMissile() const
{
	Vertex_PCU MissileVertsGlobal[]
	{
		Vertex_PCU(4.f, 0.f,	255, 0, 0, 255),
		Vertex_PCU(3.f, 0.6f,	255, 0, 0, 255),
		Vertex_PCU(3.f, -0.6f,	255, 0, 0, 255),

		Vertex_PCU(-2.f, 0.6f,	192,192,192, 255),
		Vertex_PCU(3.f, 0.6f,	192,192,192, 255),
		Vertex_PCU(3.f, -0.6f,	192,192,192, 255),

		Vertex_PCU(-2.f,0.6f,	192,192,192, 255),
		Vertex_PCU(-2.f, -0.6f,	192,192,192, 255),
		Vertex_PCU(3.f, -0.6f,	192,192,192, 255),

		Vertex_PCU(1.f, 0.6f,	96,96,96, 255),
		Vertex_PCU(-1.f, 0.6f,	96,96,96, 255),
		Vertex_PCU(-1.f, 1.5f,	96,96,96, 255),

		Vertex_PCU(1.f, -0.6f,	96,96,96, 255),
		Vertex_PCU(-1.f, -0.6f,	96,96,96, 255),
		Vertex_PCU(-1.f, -1.5f,	96,96,96, 255),

	};
	int vertNum = sizeof(MissileVertsGlobal) / sizeof(MissileVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, MissileVertsGlobal, m_scale, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(vertNum, MissileVertsGlobal);


}

void Missile::RenderFlame() const
{
	Vertex_PCU MissileVertsGlobal[3]
	{
		Vertex_PCU(-2.f, 0.6f,	255, 255, 51, 255),
		Vertex_PCU(-2.f, -0.6f,	255, 255, 51, 255),
		Vertex_PCU(-3.5f,0.f,	255, 0, 0, 0),

	};

	TransformVertexArrayXY3D(3, MissileVertsGlobal, m_scale, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(3, MissileVertsGlobal);


}

void Missile::RenderExplosion() const
{

	for (int shapeIndex = 0; shapeIndex < MISSILE_EXPLOSION_EDGES; shapeIndex++)
	{
		if (shapeIndex == MISSILE_EXPLOSION_EDGES - 1)
		{
			Vertex_PCU asteroidVerts[3] =
			{
				Vertex_PCU(m_randomShape[shapeIndex].x, m_randomShape[shapeIndex].y,			255, 153, 51, 255),
				Vertex_PCU(0.f, 0.f,															255, 255, 204, 255),
				Vertex_PCU(m_randomShape[0].x, m_randomShape[0].y,								255, 153, 51, 255)
			};
			TransformVertexArrayXY3D(3, asteroidVerts, 1.f, m_orientationDegrees, m_position);
			g_theRenderer->DrawVertexArray(3, asteroidVerts);
		}
		else
		{
			Vertex_PCU asteroidVerts[3] =
			{
				Vertex_PCU(m_randomShape[shapeIndex].x, m_randomShape[shapeIndex].y,			255, 153, 51, 255),
				Vertex_PCU(0.f, 0.f,															255, 255, 204, 255),
				Vertex_PCU(m_randomShape[shapeIndex + 1].x, m_randomShape[shapeIndex + 1].y,	255, 153, 51, 255)
			};
			TransformVertexArrayXY3D(3, asteroidVerts, 1.f, m_orientationDegrees, m_position);
			g_theRenderer->DrawVertexArray(3, asteroidVerts);
		}
	}
}

void Missile::Die()
{
	m_health = 0;
	m_isDead = true;
}

void Missile::GetDamage(int damageValue)
{
	m_health -= damageValue;

	if (m_health <= 0)
	{
		Die();
	}
}

float Missile::GetScale() const
{
	return m_scale;
}

void Missile::SetScale( float scale)
{
	m_scale =scale;
}

Entity* Missile::GetTarget() const
{
	return m_target;
}

void Missile::SetTarget(Entity* target)
{
	m_target = target;
}
