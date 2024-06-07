#include "Game/PlayerShip.hpp"
#include "Game/App.hpp"

extern Renderer*		g_theRenderer;
extern InputSystem*		g_theInput;
extern AudioSystem*		g_theAudio;

PlayerShip::PlayerShip(Game* gameInstance, Vec2 startPosition)
{
	m_game = gameInstance;
	m_position = startPosition;
}

void PlayerShip::StartUp()
{

	m_faceDirection = Vec2(1.f, 0.f);
	m_health = PLAYER_HP;
	m_invulnerable = false;
	m_respawnTimes = PLAYER_LIVES-1;
	m_currentMissileNumber = MISSILE_DEFAULT_NUMBER;
	m_missileCoolDownTimer = MISSILE_COOLDOWN_TIME;

	m_hasWeapon = false;
	m_weaponTimer = WEAPON_MAX_CONTAING_TIME;
	m_canSpawnMachinegunBullet = true;
	m_machinegunCoolDownTimer = MACHINEGUN_TIMER;

	m_color = Rgba8(200, 200, 200, 255);
	m_thrustColor = Rgba8(255, 102, 0, 255);
	m_weapon = PlayerWeapon::WEAPON_NORMALBULLET;
	m_rng = new RandomNumberGenerator();
	m_physicsRadius = PLAYER_SHIP_PHYSICS_RADIUS;
	m_cosmeticRadius = PLAYER_SHIP_COSMETIC_RADIUS;
	m_transparency = 255;
}


void PlayerShip::Update()
{
	if (!IsAlive())
	{
		m_game->m_invokeEndTimer =true;
	}
	if (m_game->m_gameClock->GetDeltaSeconds()>0.f) 
	{
		m_accleration = 0.f;
	}
	UpdateAllTimers();
	UpdateFromKeyBoard();
	UpdateFromController();
	m_position += m_velocity * m_game->m_gameClock->GetDeltaSeconds();
	m_firePosition = m_position + 3.0f*GetForwardNormal();
}

void PlayerShip::UpdateShipPosition()
{
	m_accleration = PLAYER_SHIP_ACCELERATION;
	Vec2 acclerationDirectional = m_faceDirection * m_accleration;
	m_velocity += acclerationDirectional *m_game->m_gameClock->GetDeltaSeconds();
}

void PlayerShip::UpdateAllTimers()
{
	UpdateInvulnerableTimer();
	UpdateWeaponTimer();
	UpdateMachinegunCoolDown();
	UpdateMissileCoolDown();
}

void PlayerShip::UpdateWeaponTimer()
{
	if (!m_hasWeapon)
	{
		return;
	}
	if (m_weaponTimer > 0.0f)
	{
		m_weaponTimer -= m_game->m_gameClock->GetDeltaSeconds();
	}
	else
	{
		SwitchWeapon(0);
	}
}

void PlayerShip::UpdateMachinegunCoolDown()
{
	if (m_canSpawnMachinegunBullet) 
	{
		return;
	}
	if (m_machinegunCoolDownTimer > 0.0f)
	{
		m_machinegunCoolDownTimer -= m_game->m_gameClock->GetDeltaSeconds();
	}
	else
	{
		m_machinegunCoolDownTimer = MACHINEGUN_TIMER;
		m_canSpawnMachinegunBullet = true;
	}
}

void PlayerShip::UpdateInvulnerableTimer()
{
	if (!m_invulnerable) 
	{
		return;
	}

	if (m_invulnerable) 
	{
		if (m_invulnerableTimer < 3.0f)
		{
			m_invulnerableTimer += m_game->m_gameClock->GetDeltaSeconds();

			if (m_transparencyDecreased)
			{
				m_transparency -= 20;
				if (m_transparency < 0)
				{
					m_transparencyDecreased = false;
				}
			}
			else if (!m_transparencyDecreased)
			{
				m_transparency += 20;
				if (m_transparency > 255)
				{
					m_transparencyDecreased = true;
				}
			}
		}
		else
		{
			m_invulnerable = false;
			m_transparency = 255;
			m_invulnerableTimer = 0.0f;
			m_transparencyDecreased = true;
		}
	}
}

void PlayerShip::UpdateMissileCoolDown()
{
	if (m_currentMissileNumber ==2) 
	{
		return;
	}
	else 
	{
		if (m_missileCoolDownTimer>0.0f) 
		{
			m_missileCoolDownTimer -=m_game->m_gameClock->GetDeltaSeconds();
		}
		else 
		{
			m_missileCoolDownTimer = MISSILE_COOLDOWN_TIME;
			m_currentMissileNumber += 1;
		}
	}
}

void PlayerShip::UpdateFromKeyBoard()
{
	if (g_theInput->IsKeyDown('N') && m_isDead && m_respawnTimes > 0)
	{
		m_respawnTimes-=1;
		ResetPlayership();
		SoundID respawnSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerRespawn.wav");
		g_theAudio->StartSound(respawnSound);
	}

	if (m_isDead) return;

	if (g_theInput->IsKeyDown('E'))
	{
		UpdateShipPosition();
	}

	if (g_theInput->IsKeyDown('S'))
	{
		m_orientationDegrees += (PLAYER_SHIP_TURN_SPEED * m_game->m_gameClock->GetDeltaSeconds());
	}

	if (g_theInput->IsKeyDown('F'))
	{
		m_orientationDegrees -= (PLAYER_SHIP_TURN_SPEED * m_game->m_gameClock->GetDeltaSeconds());
	}

	if (g_theInput->IsKeyDown('L')&&!g_theInput->WasKeyJustPressed('L') )
	{
		SwitchWeapon(1);
	}

	if (!g_theInput->WasKeyJustPressed(KEYCODE_SPACE) && g_theInput->IsKeyDown(KEYCODE_SPACE))
	{
		SoundID bulletSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerShootNormal.wav");
		SoundID missileSound = g_theAudio->CreateOrGetSound("Data/Audio/MissileSpawn.wav");
		switch (m_weapon)
		{
			case PlayerWeapon::WEAPON_NORMALBULLET:
				m_game->SpawnBullet(m_firePosition);
				g_theAudio->StartSound(bulletSound);
				break;

			case PlayerWeapon::WEAPON_MISSILE:
				if (m_currentMissileNumber >=1) 
				{
					m_game->SpawnMissile(m_firePosition);
					g_theAudio->StartSound(missileSound);
					m_currentMissileNumber -= 1;
				}
				break;
		}

	}

	if (g_theInput->IsKeyDown(KEYCODE_SPACE))
	{
		SoundID bulletSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerMachineGun.wav");
		Vec2	perpendicularFaceDirection = m_faceDirection.GetRotated90Degrees().GetNormalized();
		Vec2	spawnPosition = m_firePosition;
		switch (m_weapon)
		{
			case PlayerWeapon::WEAPON_MACHINEGUN:
				if (m_canSpawnMachinegunBullet) 
				{
					spawnPosition = m_firePosition + perpendicularFaceDirection;
					m_game->SpawnBullet(spawnPosition);
					m_game->SpawnBullet(m_firePosition);
					spawnPosition = m_firePosition - perpendicularFaceDirection;
					m_game->SpawnBullet(spawnPosition);
					m_canSpawnMachinegunBullet = false;
					g_theAudio->StartSound(bulletSound);
					g_theAudio->StopSound(bulletSound);
				}
				break;
		}
	}
}

void PlayerShip::UpdateFromController()
{
	XboxController ctrler = g_theInput->GetController(0);
	
	if (ctrler.IsConnected()==false)
	{
		return;
	}

	if (ctrler.IsButtonDown(XboxButtonID::XBOX_BUTTON_START))
	{
		if ( m_isDead && m_respawnTimes > 0)
		{
			m_respawnTimes -= 1;
			ResetPlayership();
			SoundID respawnSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerRespawn.wav");
			g_theAudio->StartSound(respawnSound);
			g_theAudio->StopSound(respawnSound);
			return;
		}

	}

	if (m_isDead) return;

	if (ctrler.GetLeftStick().GetMagnitude()!= 0.f) 
	{
		m_orientationDegrees = ctrler.GetLeftStick().GetOrientationDegrees();
		
		m_accleration = PLAYER_SHIP_ACCELERATION * ctrler.GetLeftStick().GetMagnitude();

		Vec2 acclerationDirectional = m_faceDirection * m_accleration;

		m_velocity += acclerationDirectional * m_game->m_gameClock->GetDeltaSeconds();
	}

	if (ctrler.IsButtonDown(XboxButtonID::XBOX_BUTTON_A) && !ctrler.WasButtonJustPressed(XboxButtonID::XBOX_BUTTON_A))
	{
		SoundID bulletSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerShootNormal.wav");
		SoundID missileSound = g_theAudio->CreateOrGetSound("Data/Audio/MissileSpawn.wav");
		switch (m_weapon)
		{
		case PlayerWeapon::WEAPON_NORMALBULLET:
			m_game->SpawnBullet(m_firePosition);
			g_theAudio->StartSound(bulletSound);
			break;

		case PlayerWeapon::WEAPON_MISSILE:
			if (m_currentMissileNumber >= 1)
			{
				m_game->SpawnMissile(m_firePosition);
				g_theAudio->StartSound(missileSound);
				m_currentMissileNumber -= 1;
			}
			break;
		}
	}
	if (ctrler.IsButtonDown(XboxButtonID::XBOX_BUTTON_A))
	{
		SoundID bulletSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerMachineGun.wav");
		Vec2	perpendicularFaceDirection = m_faceDirection.GetRotated90Degrees().GetNormalized();
		Vec2	spawnPosition = m_firePosition;
		switch (m_weapon)
		{
		case PlayerWeapon::WEAPON_MACHINEGUN:
			if (m_canSpawnMachinegunBullet)
			{
				spawnPosition = m_firePosition + perpendicularFaceDirection;
				m_game->SpawnBullet(spawnPosition);
				m_game->SpawnBullet(m_firePosition);
				spawnPosition = m_firePosition - perpendicularFaceDirection;
				m_game->SpawnBullet(spawnPosition);
				m_canSpawnMachinegunBullet = false;
				g_theAudio->StartSound(bulletSound);
				g_theAudio->StopSound(bulletSound);
			}
			break;
		}
	}
}



void PlayerShip::Render()
{
	if (m_isDead) return;
	RenderShipWeapon();
	RenderShip();
	RenderShipDecoration();
	RenderShipAnimation();
}

void PlayerShip::RenderShip() const
{

	Vertex_PCU shipVertsGlobal[]
	{
		Vertex_PCU(2.5f, -0.5f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(3.0, 0.f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(2.5f, 0.5f,		m_color.r, m_color.g, m_color.b,	m_transparency),
																		
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(2.5f, 0.5f,		m_color.r, m_color.g, m_color.b,	m_transparency),
																		
		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b,	m_transparency),
																		
		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(2.5f, -0.5f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b,	m_transparency),
																		
		Vertex_PCU(-1.8f, 1.75f,	128,128,128,						m_transparency),
		Vertex_PCU(0.f,1.25f,		128,128,128,						m_transparency),
		Vertex_PCU(-1.8f,3.f,		m_color.r, m_color.g, m_color.b,	m_transparency),
																		
		Vertex_PCU(-1.8f,-1.75f,	m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(-1.8f, 1.75f,	m_color.r, m_color.g, m_color.b,	m_transparency),
																		
		Vertex_PCU(-1.8f,-1.75f,	m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b,	m_transparency),
																		
		Vertex_PCU(-1.8f,-3.f,		m_color.r, m_color.g, m_color.b,	m_transparency),
		Vertex_PCU(0.f,-1.25f,		128,128,128,						m_transparency),
		Vertex_PCU(-1.8f,-1.75f,	128,128,128,						m_transparency),
																		
		Vertex_PCU(-2.2f,1.0f,		96,	96,	96,							m_transparency),
		Vertex_PCU(-1.8f,1.75f,		96,	96,	96,							m_transparency),
		Vertex_PCU(-2.2f,1.75f,		96,	96,	96,							m_transparency),
																		
		Vertex_PCU(-2.2f,1.0f,		96,	96,	96,							m_transparency),
		Vertex_PCU(-1.8f,1.f,		96,	96,	96,							m_transparency),
		Vertex_PCU(-1.8f,1.75f,		96,	96,	96,							m_transparency),
																		
		Vertex_PCU(-3.0f,-1.f,		64, 64, 64,							m_transparency),
		Vertex_PCU(-1.8f,1.f,		64, 64, 64,							m_transparency),
		Vertex_PCU(-3.0f,1.f,		64, 64, 64,							m_transparency),
																		
		Vertex_PCU(-3.0f,-1.f,		64, 64, 64,							m_transparency),
		Vertex_PCU(-1.8f,-1.f,		64, 64, 64,							m_transparency),
		Vertex_PCU(-3.0f,1.f,		64, 64, 64,							m_transparency),
																		
		Vertex_PCU(-2.2f,-1.75f,	96,	96,	96,							m_transparency),
		Vertex_PCU(-1.8f,-1.f,		96,	96,	96,							m_transparency),
		Vertex_PCU(-2.2f,-1.f,		96,	96,	96,							m_transparency),
																		
		Vertex_PCU(-2.2f,-1.75f,	96,	96,	96,							m_transparency),
		Vertex_PCU(-1.8f,-1.75f,	96,	96,	96,							m_transparency),
		Vertex_PCU(-1.8f,-1.f,		96,	96,	96,							m_transparency),
																		
		Vertex_PCU(2.3f, 0.6f,		64, 64, 64,							m_transparency),
		Vertex_PCU(2.5f, -0.5f,		64, 64, 64,							m_transparency),
		Vertex_PCU(2.5f, 0.5f,		64, 64, 64,							m_transparency),
																		
		Vertex_PCU(2.3f,-0.6f,		64, 64, 64,							m_transparency),
		Vertex_PCU(2.5f, -0.5f,		64, 64, 64,							m_transparency),
		Vertex_PCU(2.3f,0.6f,		64, 64, 64,							m_transparency),
																		
		Vertex_PCU(1.0f,-0.5f,		51, 255, 255,						m_transparency),
		Vertex_PCU(2.0f,0.0f,		51, 255, 255,						m_transparency),
		Vertex_PCU(1.0f,0.5f,		51, 255, 255,						m_transparency),
																		
		Vertex_PCU(0.25f,1.0f,		255, 255, 255,						m_transparency),
		Vertex_PCU(1.0f,-0.5f,		255, 255, 255,						m_transparency),
		Vertex_PCU(1.0f,0.5f,		255, 255, 255,						m_transparency),
																		
		Vertex_PCU(0.25f,-1.0f,		255, 255, 255,						m_transparency),
		Vertex_PCU(1.0f,-0.5f,		255, 255, 255,						m_transparency),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255,						m_transparency),
																		
		Vertex_PCU(-1.8f,-1.5f,		255, 255, 255,						m_transparency),
		Vertex_PCU(0.25f,-1.0f,		255, 255, 255,						m_transparency),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255,						m_transparency),
																		
		Vertex_PCU(-1.8f,-1.5f,		255, 255, 255,						m_transparency),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255,						m_transparency),
		Vertex_PCU(-1.8f,1.5f,		255, 255, 255,						m_transparency),
																		
		Vertex_PCU(-1.8f,-2.f,		96, 96, 96,							m_transparency),
		Vertex_PCU(-1.8f,-2.5f,		96, 96, 96,							m_transparency),
		Vertex_PCU(-0.5f,-2.f,		96, 96, 96,							m_transparency),
																		
		Vertex_PCU(-1.8f,2.f,		96, 96, 96,							m_transparency),
		Vertex_PCU(-0.5f,2.f,		96, 96, 96,							m_transparency),
		Vertex_PCU(-1.8f,2.5f,		96, 96, 96,							m_transparency),
	};
	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
}

void PlayerShip::RenderShipDecoration() const
{
	Vertex_PCU shipVertsGlobal[]
	{
		Vertex_PCU(2.3f, 0.6f,		64, 64, 64, m_transparency),
		Vertex_PCU(2.5f, 0.5f,		64, 64, 64, m_transparency),
		Vertex_PCU(2.5f, -0.5f,		64, 64, 64, m_transparency),
				   
		Vertex_PCU(2.3f,0.6f,		64, 64, 64, m_transparency),
		Vertex_PCU(2.3f,-0.6f,		64, 64, 64, m_transparency),
		Vertex_PCU(2.5f, -0.5f,		64, 64, 64, m_transparency),

		Vertex_PCU(1.0f,0.5f,		51, 255, 255, m_transparency),
		Vertex_PCU(2.0f,0.0f,		51, 255, 255, m_transparency),
		Vertex_PCU(1.0f,-0.5f,		51, 255, 255, m_transparency),

		Vertex_PCU(1.0f,0.5f,		255, 255, 255, m_transparency),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255, m_transparency),
		Vertex_PCU(1.0f,-0.5f,		255, 255, 255, m_transparency),
									
		Vertex_PCU(1.0f,-0.5f,		255, 255, 255, m_transparency),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255, m_transparency),
		Vertex_PCU(0.25f,-1.0f,		255, 255, 255, m_transparency),

		Vertex_PCU(-1.8f,-1.5f,		255, 255, 255, m_transparency),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255, m_transparency),
		Vertex_PCU(0.25f,-1.0f,		255, 255, 255, m_transparency),

		Vertex_PCU(-1.8f,-1.5f,		255, 255, 255, m_transparency),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255, m_transparency),
		Vertex_PCU(-1.8f,1.5f,		255, 255, 255, m_transparency),

		Vertex_PCU(-1.8f,-2.f,		96, 96, 96, m_transparency),
		Vertex_PCU(-0.5f,-2.f,		96, 96, 96, m_transparency),
		Vertex_PCU(-1.8f,-2.5f,		96, 96, 96, m_transparency),

		Vertex_PCU(-1.8f,2.f,		96, 96, 96, m_transparency),
		Vertex_PCU(-0.5f,2.f,		96, 96, 96, m_transparency),
		Vertex_PCU(-1.8f,2.5f,		96, 96, 96, m_transparency),
	};
	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
}

void PlayerShip::RenderShipWeapon() const
{
	Vec3 startPosition(-1.5f, 2.5f, 0.0f);

	switch (m_weapon)
	{
		case PlayerWeapon::WEAPON_NORMALBULLET:

			break;

		case PlayerWeapon::WEAPON_MISSILE:

			for (int missileIndex = 0 ; missileIndex < m_currentMissileNumber ; missileIndex++)
			{

				Vertex_PCU MissileVertsGlobal[]
				{
					Vertex_PCU(4.f, 0.f,	255, 0, 0, m_transparency),
					Vertex_PCU(3.f, 0.6f,	255, 0, 0, m_transparency),
					Vertex_PCU(3.f, -0.6f,	255, 0, 0, m_transparency),

					Vertex_PCU(-2.f, 0.6f,	192,192,192, m_transparency),
					Vertex_PCU(3.f, 0.6f,	192,192,192, m_transparency),
					Vertex_PCU(3.f, -0.6f,	192,192,192, m_transparency),

					Vertex_PCU(-2.f,0.6f,	192,192,192, m_transparency),
					Vertex_PCU(-2.f, -0.6f,	192,192,192, m_transparency),
					Vertex_PCU(3.f, -0.6f,	192,192,192, m_transparency),

					Vertex_PCU(1.f, 0.6f,	96,96,96,	m_transparency),
					Vertex_PCU(-1.f, 0.6f,	96,96,96,	m_transparency),
					Vertex_PCU(-1.f, 1.f,	96,96,96,	m_transparency),

					Vertex_PCU(1.f, -0.6f,	96,96,96, m_transparency),
					Vertex_PCU(-1.f, -0.6f,	96,96,96, m_transparency),
					Vertex_PCU(-1.f, -1.f,	96,96,96, m_transparency),

				};

				int vertNum = sizeof(MissileVertsGlobal) / sizeof(MissileVertsGlobal[0]);

				for (int i = 0; i < vertNum; i++)
				{
					Vec3 vectorToOrigin = MissileVertsGlobal[i].m_position;
					vectorToOrigin += startPosition;
					vectorToOrigin.GetRotatedAboutZDegrees(m_orientationDegrees);
					MissileVertsGlobal[i].m_position = vectorToOrigin;
				}
				startPosition -= Vec3(0.0f, 5.f, 0.0f);
				TransformVertexArrayXY3D(vertNum, MissileVertsGlobal, 0.5f, m_orientationDegrees, m_position);
				g_theRenderer->DrawVertexArray(vertNum, MissileVertsGlobal);

			}

			break;

		case PlayerWeapon::WEAPON_MACHINEGUN:

			for (int machineIndex = 0; machineIndex < 2; machineIndex++)
			{

				Vertex_PCU MissileVertsGlobal[]
				{
					Vertex_PCU(-2.f, 1.0f,	192,192,192, m_transparency),
					Vertex_PCU(3.f, 1.0f,	192,192,192, m_transparency),
					Vertex_PCU(3.f, -1.0f,	192,192,192, m_transparency),

					Vertex_PCU(-2.f,1.0f,	192,192,192, m_transparency),
					Vertex_PCU(-2.f, -1.0f,	192,192,192, m_transparency),
					Vertex_PCU(3.f, -1.0f,	192,192,192, m_transparency),

				};

				int vertNum = sizeof(MissileVertsGlobal) / sizeof(MissileVertsGlobal[0]);

				for (int i = 0; i < vertNum; i++)
				{
					Vec3 vectorToOrigin = MissileVertsGlobal[i].m_position;
					vectorToOrigin += startPosition;
					vectorToOrigin.GetRotatedAboutZDegrees(m_orientationDegrees);
					MissileVertsGlobal[i].m_position = vectorToOrigin;
				}
				startPosition -= Vec3(0.0f, 5.f, 0.0f);
				TransformVertexArrayXY3D(vertNum, MissileVertsGlobal, 0.5f, m_orientationDegrees, m_position);
				g_theRenderer->DrawVertexArray(vertNum, MissileVertsGlobal);
			}
			return;
	}
}

void PlayerShip::RenderShipAnimation() const
{
	float thrustInZeroOneRange = m_accleration / PLAYER_SHIP_ACCELERATION;
	float randomNumber = m_rng->RollRandomFloatInRange(0.8f, 1.0f);
	thrustInZeroOneRange *= randomNumber;
	Vertex_PCU shipVertsGlobal[]
	{
		Vertex_PCU(-3.0f, 1.0f,														m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, m_transparency),
		Vertex_PCU((-3.0f - PLAYER_FLAME_PARAMETER * thrustInZeroOneRange), 0.f,		m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, 0),
		Vertex_PCU(-3.0f, -1.0f,													m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, m_transparency),

		Vertex_PCU(-2.2f, 1.75f,													m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, m_transparency),
		Vertex_PCU((-2.2f - PLAYER_FLAME_PARAMETER * thrustInZeroOneRange), 1.375f,	m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, 0),
		Vertex_PCU(-2.2f, 1.0f,														m_thrustColor.r, m_thrustColor.g, m_thrustColor.b,m_transparency),

		Vertex_PCU(-2.2f, -1.0f,													m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, m_transparency),
		Vertex_PCU((-2.2f - PLAYER_FLAME_PARAMETER * thrustInZeroOneRange), -1.375f,m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, 0),
		Vertex_PCU(-2.2f, -1.75f,													m_thrustColor.r, m_thrustColor.g, m_thrustColor.b, m_transparency),
	};


	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
}

void PlayerShip::Die()
{
	m_isDead = true;
	m_health = 0;
	int debrisNum = m_rng->RollRandomIntRange(5, 30);
	m_game->SpawnMultipleDebrisForEntity(m_position, debrisNum, m_color, DEBRIS_COSMETIC_RADIUS, m_velocity);
	SoundID deadSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerDied.wav");
	g_theAudio->StartSound(deadSound);
}



void PlayerShip::GetDamage(int damageValue)
{
	if (m_invulnerable)
	{
		return;
	}
	m_health -= damageValue;
	SoundID hurtSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerHurt.wav");
	g_theAudio->StartSound(hurtSound);
	m_invulnerable = true;
	if (m_health <= 0)
	{
		Die();
	}
}

Rgba8 PlayerShip::GetMainColor() const
{
	return m_color;
}

Rgba8 PlayerShip::GetThrustColor() const
{
	return m_thrustColor;
}

bool PlayerShip::GetIsInvulnerable() const
{
	return m_invulnerable;
}

void PlayerShip::SetIsInvunlnerable(const bool isInvunlnerable)
{
	m_invulnerable = isInvunlnerable;
}

void PlayerShip::ResetPlayership()
{
	m_isDead = false;
	m_position = Vec2(100.f, 50.f);
	m_invulnerable = false;
	m_invulnerableTimer = 0.0f;
	m_orientationDegrees = 0.f;
	m_faceDirection = Vec2(1.f, 0.f);
	SwitchWeapon(0);
	m_health = PLAYER_HP;
	m_velocity = Vec2(0.f,0.f);
	m_speed = 0.f;
	m_physicsRadius = PLAYER_SHIP_PHYSICS_RADIUS;
	m_cosmeticRadius = PLAYER_SHIP_COSMETIC_RADIUS;
	m_game->ResetEndTimer();
}


void PlayerShip::SwitchWeapon(int weaponNum)
{
	if (weaponNum ==0) 
	{
		m_weapon = PlayerWeapon::WEAPON_NORMALBULLET;
		m_hasWeapon = false;
		return;
	}

	if (m_weapon == PlayerWeapon::WEAPON_NORMALBULLET)
	{

		if (weaponNum ==1)
		{
			m_weapon = PlayerWeapon::WEAPON_MISSILE;
			m_hasWeapon = true;
			m_weaponTimer = WEAPON_MAX_CONTAING_TIME;
		}
		else 
		{
			m_weapon = PlayerWeapon::WEAPON_MACHINEGUN;
			m_hasWeapon = true;
			m_weaponTimer = WEAPON_MAX_CONTAING_TIME;
		}
	}
}

