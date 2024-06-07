#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

class Asteroid : public Entity
{
public:
	
	Asteroid(Game* gameInstance, Vec2 startPosition);

	void					StartUp();
	virtual void			Update() override;
	virtual void			Render() override;
	void					RenderFlame() const;
	void					GetDamage(int damage);
	virtual void			Die() override;

	float					GetSpeed() const;
	void					SetSpeed(float speed);
	
	Rgba8					m_color;

private:

	int						m_shape;
	float					m_speed;
	Vec2					m_randomShape[16];
	RandomNumberGenerator*  m_rng = nullptr;

};

