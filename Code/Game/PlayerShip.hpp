#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

enum class PlayerWeapon
{
	WEAPON_NORMALBULLET,
	WEAPON_MISSILE,
	WEAPON_MACHINEGUN,
	WEAPON_NUM
};

class PlayerShip : public Entity
{
public:

	PlayerShip( Game* gameInstance, Vec2 startPosition);

	void					StartUp();
	
	virtual void			Update() override;
	void					UpdateShipPosition();
	void					UpdateAllTimers();
	void					UpdateWeaponTimer();
	void					UpdateMachinegunCoolDown();
	void					UpdateInvulnerableTimer();
	void					UpdateMissileCoolDown();
	void					UpdateFromKeyBoard();
	void					UpdateFromController();

	virtual void			Render() override;
	void					RenderShip() const;
	void					RenderShipDecoration() const;
	void					RenderShipWeapon() const;
	void					RenderShipAnimation() const;
	
	virtual void			Die() override;
	virtual void			GetDamage(int damageValue) override;

	Rgba8					GetMainColor() const;
	Rgba8					GetThrustColor() const;
	bool					GetIsInvulnerable() const;

	void					SetIsInvunlnerable(const bool isInvunlnerable);
	void					ResetPlayership();
	void					SwitchWeapon(int weaponNum);

	int						m_respawnTimes;
	
private:

	RandomNumberGenerator*	m_rng;
	Rgba8					m_color;
	PlayerWeapon			m_weapon;
	Vec2					m_firePosition;
	Rgba8					m_thrustColor;
	float					m_accleration;
	unsigned char			m_transparency;

	bool					m_hasWeapon =false;
	float					m_weaponTimer;

	int						m_currentMissileNumber;
	float					m_missileCoolDownTimer;

	bool					m_canSpawnMachinegunBullet=true;
	float					m_machinegunCoolDownTimer;

	bool					m_invulnerable =false;
	bool					m_transparencyDecreased = true;
	float					m_invulnerableTimer;

};

