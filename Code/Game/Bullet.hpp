#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"


class Bullet : public Entity
{
public:

	Bullet();
	Bullet(Game* gameInstance, Vec2 startPos);
	~Bullet();

	void			StartUp();
	virtual void	Update() override;
	void			UpdateNormalMoveMode();
	virtual void	Render() override;
	virtual void	Die() override;
	virtual void	GetDamage(int damageValue) override;

};

