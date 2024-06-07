#include "Game/Entity.hpp"

Entity::Entity()

{

}

Entity::~Entity()
{

}

Entity::Entity(Game* gameInstance, Vec2 startPosition)
{
	m_game = gameInstance;
	m_position = startPosition;
}

bool Entity::IsIsOffscreen()
{
	return false;
}

Vec2 Entity::GetForwardNormal() 
{
	m_faceDirection = Vec2(1.f, 0.f);
	m_faceDirection.RotateDegrees(m_orientationDegrees);
	return  m_faceDirection;
}

bool Entity::IsAlive()
{
	return !m_isDead;
}

