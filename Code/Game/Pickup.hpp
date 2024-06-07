#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

class Pickup : public Entity
{
public:

	Pickup(Game* gameInstance, Vec2 startPosition);

	void					StartUp();
	virtual void			Update() override;
	void					UpdatePickupLifetime();
	virtual void			Render() override;
	virtual void		    GetDamage(int damage) override;
	virtual void			Die() override;

private:

	Rgba8					m_color;
	float					m_liftTimer;
	RandomNumberGenerator*	m_rng;
};

