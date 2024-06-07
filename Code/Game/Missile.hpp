#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"


class Missile : public Entity
{
public:

	Missile();
	Missile(Game* gameInstance, Vec2 startPos);
	~Missile();

	void					StartUp();
	virtual void			Update() override;
	void					UpdateRotationDirection(Vec2& uncorrectedVector);

	virtual void			Render() override;
	void					RenderMissile() const;
	void					RenderFlame() const;
	void					RenderExplosion() const;
	virtual void			Die() override;
	virtual void			GetDamage(int damageValue) override;

	float					GetScale() const;
	void					SetScale(float scale);

	Entity*					GetTarget() const;
	void					SetTarget(Entity* target);


private:

	RandomNumberGenerator*	m_rng;
	Entity*					m_target;
	Vec2					m_randomShape[MISSILE_EXPLOSION_EDGES];
	Vec2					m_startPosition;
	float					m_scale;
	float					m_brokenTimer = 0.f;
	float					m_radius;
	float					m_positionLerpValue=0.0f;
};

