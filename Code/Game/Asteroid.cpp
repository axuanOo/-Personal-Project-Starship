#include "Asteroid.hpp"
#include "Game/Game.hpp"

extern Renderer* g_theRenderer;
extern AudioSystem* g_theAudio;
Asteroid::Asteroid(Game* gameInstance, Vec2 startPosition)
{
	m_game = gameInstance;
	m_position = startPosition;
}

void Asteroid::StartUp()
{
	m_rng = new RandomNumberGenerator();

	m_orientationDegrees = m_rng->RollRandomFloatInRange(0.f,360.f);

	m_faceDirection = Vec2(m_rng->RollRandomFloatZeroToOne(),m_rng->RollRandomFloatZeroToOne());

	m_faceDirection.Normalized();

	m_velocity = m_faceDirection * ASTEROID_SPEED;

	m_physicsRadius = ASTEROID_PHYSICS_RADIUS;

	m_cosmeticRadius = ASTEROID_COSMETIC_RADIUS;

	m_angularVelocity =static_cast<float>(m_rng->RollRandomIntRange(-200,200));

	m_health = 3;

	m_speed = 1.0f;

	m_color = Rgba8(100, 100, 100, 255);

	/// ------------------------------------------------------------------------------------------------------------------
	m_shape = 16;

	float thetaDegrees = 360.f / m_shape;

	float thetaRadians = ConvertDegreeToRadians(thetaDegrees);

	for (int shapeIndex = 0; shapeIndex < m_shape; shapeIndex++)
	{
		float r = m_rng->RollRandomFloatInRange(ASTEROID_PHYSICS_RADIUS, ASTEROID_COSMETIC_RADIUS);

		float theta = thetaRadians * shapeIndex;

		m_randomShape[shapeIndex] = Vec2(cosf(theta) * r, sinf(theta) * r);
	}

}

void Asteroid::Update()
{
	m_position += m_velocity* m_speed * m_game->m_gameClock->GetDeltaSeconds();
	m_orientationDegrees += m_angularVelocity * m_game->m_gameClock->GetDeltaSeconds();
}

void Asteroid::Render()
{

	for (int shapeIndex = 0; shapeIndex < m_shape; shapeIndex++)
	{
		if (shapeIndex == m_shape -1) 
		{
			Vertex_PCU asteroidVerts[3]=
			{
				Vertex_PCU(0.f, 0.f,	100, 100, 100, 255 ),
				Vertex_PCU(m_randomShape[shapeIndex].x, m_randomShape[shapeIndex].y,	100, 100, 100, 255 ),
				Vertex_PCU(m_randomShape[0].x, m_randomShape[0].y,	100, 100, 100, 255 )
			};
			TransformVertexArrayXY3D(3, asteroidVerts, 1.f, m_orientationDegrees, m_position);
			g_theRenderer->DrawVertexArray(3, asteroidVerts);
		}
		else 
		{
			Vertex_PCU asteroidVerts[3] =
			{
				Vertex_PCU(0.f, 0.f,	100, 100, 100, 255),
				Vertex_PCU(m_randomShape[shapeIndex].x, m_randomShape[shapeIndex].y,	100, 100, 100, 255),
				Vertex_PCU(m_randomShape[shapeIndex+1].x, m_randomShape[shapeIndex+1].y,	100, 100, 100, 255)
			};
			TransformVertexArrayXY3D(3, asteroidVerts, 1.f, m_orientationDegrees, m_position);
			g_theRenderer->DrawVertexArray(3, asteroidVerts);
		}
	}

}

void Asteroid::RenderFlame() const
{
	float maxLengthFlame = 15.0f;
	float randomNumber = m_rng->RollRandomFloatInRange(0.8f, 1.0f);
	maxLengthFlame *= randomNumber;
	Vertex_PCU shipVertsGlobal[]
	{
		Vertex_PCU(-1.0f, 1.0f,														192, 192, 192, 255),
		Vertex_PCU((-1.0f - maxLengthFlame), 0.f,									192, 192, 192, 0),
		Vertex_PCU(-1.0f, -1.0f,													192, 192, 192, 255),
	};


	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, 1.f, m_velocity.GetNormalized().GetOrientationDegrees(), m_position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
}

void Asteroid::GetDamage(int damage) 
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

void Asteroid::Die()
{
	int debrisNum = m_rng->RollRandomIntRange(3, 12);
	m_game->SpawnMultipleDebrisForEntity(m_position, debrisNum, m_color, DEBRIS_COSMETIC_RADIUS,m_velocity);

	SoundID attractSound = g_theAudio->CreateOrGetSound("Data/Audio/EnemyDied.wav");
	g_theAudio->StartSound(attractSound);

	//if (debrisNum >= 6 && debrisNum <= 11)
	//{
	//	m_game->SpawnPickUps(m_position);
	//}
	m_health = 0;
	m_isDead = true;
	m_isGarbage = true;
}

float Asteroid::GetSpeed() const
{
	return m_speed;
}

void Asteroid::SetSpeed(float speed)
{
	m_speed = speed;
}
