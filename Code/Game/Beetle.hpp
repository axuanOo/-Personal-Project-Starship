#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

class Beetle : public Entity
{
public:

	Beetle(Game* gameInstance, Vec2 startPosition);

	void					StartUp();
	virtual void			Update() override;
	virtual void			Render() override;
	void					GetDamage(int damage);
	virtual void			Die() override;

	void					FollowPlayer(const Vec2& location);

	void					SetPlayerStatus(bool playAlive);
	Rgba8					m_color;

private:

	Vec2					m_followLocation;
	bool					m_isPlayerAlive;
	RandomNumberGenerator*	m_rng;
};

