#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

class Wasp : public Entity
{
public:

	Wasp(Game* gameInstance, Vec2 startPosition);

	void					StartUp();
	virtual void			Update() override;
	virtual void			Render() override;
	virtual void		    GetDamage(int damage) override;
	virtual void			Die() override;

	void					FollowPlayer(const Vec2& location);

	void					SetPlayerStatus(bool playAlive);

	Rgba8					m_color;

private:

	Vec2					m_followLocation;
	bool					m_isPlayerAlive;
	RandomNumberGenerator*	m_rng;
};

