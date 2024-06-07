#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"


class Debris : public Entity
{
public:

	Debris();
	Debris(Game* gameInstance, Vec2 startPos);
	~Debris();

	void						StartUp();
	virtual void				Update() override;
	virtual void				Render() override;
	virtual void				Die() override;
	virtual void				GetDamage(int damageValue) override;

	void						SetColor(Rgba8& color);
	Vec2						m_initialVelocity;

private: 

	RandomNumberGenerator*		m_rng = nullptr;
	Rgba8						m_color;
	int							m_shape;
	unsigned char				m_trans = 127;
	Vec2						m_randomShape[MAX_DEBRIS_EDGES];
};

