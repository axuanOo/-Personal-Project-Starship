#include "Game/Game.hpp"
#include "App.hpp"


Game::Game(App* const& app)
{
	g_theApp = app;
}

Game::~Game()
{

}

PlayerShip* const Game::GetPlayerShip() const
{
	return m_Playership;
}


void Game::StartUp()
{
	m_rng = new RandomNumberGenerator();
	m_worldCamera = new Camera();
	m_screenCamera = new Camera();
	m_attractCamera = new Camera();
	m_worldCamera->SetOrthographicView(WORLD_CAMERA_BOTTOMLEFT, WORLD_CAMERA_TOPRIGHT);
	m_screenCamera->SetOrthographicView(SCREEN_CAMERA_BOTTOMLEFT,SCREEN_CAMERA_TOPRIGHT);
	m_attractCamera->SetOrthographicView(ATTRACT_CAMERA_BOTTOMLEFT, ATTRACT_CAMERA_TOPRIGHT);
	m_gameClock = new Clock();

	g_theEventSystem->SubscribeEventCallbackFunction("timescale", Game::ChangeTimeScale);

	SpawnAttractMode();

}

void Game::ShutDown()
{
	g_theEventSystem->UnsubscribeEventCallbackFunction("timescale", Game::ChangeTimeScale);
	g_theApp->ClearGameInstance();
	delete this;
}


void Game::SpawnAttractMode()
{
	Vec2 leftStartPosition = Vec2(50.0f, 50.f);
	Vec2 middlePosition = Vec2(100.f, 50.f);
	Vec2 rightStartPosition = Vec2(150.0f, 50.f);
	Rgba8 playerColor = Rgba8(102, 153, 204, 255);
	Rgba8 buttonColor = Rgba8(0, 255, 0, 255);

	SoundID attractSound = g_theAudio->CreateOrGetSound("Data/Audio/Welcome.mp3");
	g_theAudio->StartSound(attractSound);
	g_theAudio->StopSound(attractSound);

	UIElements* leftStarShip = new UIElements(leftStartPosition, 0.f, playerColor);
	m_attractMode[0] = leftStarShip;

	UIElements* rightStarShip = new UIElements(rightStartPosition, 180.f, playerColor);
	m_attractMode[1] = rightStarShip;

	UIElements* startButton = new UIElements(middlePosition, 0.f, buttonColor);
	m_attractMode[2] = startButton;

	SpawnAsteroidForAttractMode();
}

void Game::SpawnAsteroidForAttractMode()
{
	for (int attractAsterIndex = 0; attractAsterIndex < MAX_ASTEROIDS_ATTRACTMODE; ++attractAsterIndex)
	{
		if (m_asteroidsForAttractMode[attractAsterIndex]==nullptr) 
		{
			Asteroid* asteroid = new Asteroid(this, GeneratePositionOffscreen(ASTEROID_COSMETIC_RADIUS,true));
			asteroid->StartUp();
			asteroid->SetSpeed(3.0f);
			m_asteroidsForAttractMode[attractAsterIndex] = asteroid;
		}
	}
}

void Game::SpawnStarsPosition()
{
	float posX = 0.0f;
	float posY = 0.0f;

	for (int starPositionIndex = 0 ; starPositionIndex < MAX_STARS_NUMBER ; ++ starPositionIndex )
	{
		posX = m_rng->RollRandomFloatInRange(WORLD_CAMERA_BOTTOMLEFT.x, WORLD_CAMERA_TOPRIGHT.x);
		posY = m_rng->RollRandomFloatInRange(WORLD_CAMERA_BOTTOMLEFT.y, WORLD_CAMERA_TOPRIGHT.y);

		unsigned char r = static_cast<unsigned char>(m_rng->RollRandomIntRange(0, 255));
		unsigned char g = static_cast<unsigned char>(m_rng->RollRandomIntRange(0, 255));
		unsigned char b = static_cast<unsigned char>(m_rng->RollRandomIntRange(0, 255));
		unsigned char a = static_cast<unsigned char>(m_rng->RollRandomIntRange(0, 255));


		m_stars[starPositionIndex].m_position = Vec3(posX, posY, 0.0f);
		m_stars[starPositionIndex].m_color = Rgba8(r, g, b, a);
	}
}

void Game::SpawnPlayerShip()
{
	if (m_Playership != nullptr)
	{
		return;
	}
	else
	{
		/// <summary>
		/// Spawn a new player ship and give the variables default values
		/// </summary>
		Vec2 shipStartPos = Vec2(100.f, 50.f);
		m_Playership = new PlayerShip(this, shipStartPos);
		m_Playership->StartUp();
	}
}

void Game::SpawnPickUps(Vec2& spawnPosition)
{
	for (int pkIndex = 0 ; pkIndex < MAX_PICKUP ; ++pkIndex)
	{
		if (m_pickups[pkIndex] == nullptr)
		{
			Pickup* pickup = new Pickup(this, spawnPosition);
			pickup->StartUp();
			m_pickups[pkIndex] = pickup;
			return;
		}
	}
}

void Game::SpawnPlayerUI() 
{
	Vec2 startDrawPosition(5.f, 95.f);
	UIElements* playerUI = new UIElements(startDrawPosition, 90.f, m_Playership->GetMainColor());
	m_playerUiTool = playerUI;
}

void Game::SpawnAsteroidAtBeginning()
{
	for (int asteroidIndex = 0; asteroidIndex < NUM_STARTING_ASTEROIDS; asteroidIndex++)
	{
		SpawnOneAsteroid();
	}
}

void Game::SpawnOneAsteroid()
{
	Asteroid* asteroid = nullptr;

	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		if ((asteroidIndex == MAX_ASTEROIDS - 1) && m_asteroids[asteroidIndex] != nullptr)
		{
			ERROR_RECOVERABLE("Can't Spawn new asteroids");
			return;
		}

		if (m_asteroids[asteroidIndex] == nullptr)
		{
			asteroid = new Asteroid(this, Vec2(400.f, 400.0f));
			asteroid->StartUp();
			asteroid->m_position = GeneratePositionOffscreen(asteroid->m_cosmeticRadius,false);		// generate a new position for this asteroid and move it
			m_asteroids[asteroidIndex] = asteroid;
			break;
		}
	}

	for (int entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
	{
		if (m_entities[entityIndex] == nullptr)
		{
			m_entities[entityIndex] = asteroid;
			return;
		}
	}
}

void Game::SpawnBullet(Vec2& firePosition)
{
	if (m_Playership != nullptr)
	{
		Bullet* bullet = new Bullet(this, firePosition);
		bullet->m_orientationDegrees = m_Playership->m_orientationDegrees;
		bullet->StartUp();
		for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
		{
			if ((bulletIndex == MAX_BULLETS - 1) && m_bullets[bulletIndex] != nullptr)
			{
				ERROR_RECOVERABLE("Can't spawn more bullets for you!!!");
				return;
			}

			if (m_bullets[bulletIndex] == nullptr)
			{
				m_bullets[bulletIndex] = bullet;
				break;
			}
		}
	}
	else
	{
		return;
	}
}

void Game::SpawnMissile(Vec2& firePosition)
{
	if (m_Playership != nullptr)
	{
		Missile* missile = new Missile(this, firePosition);
		missile->StartUp();
		missile->m_orientationDegrees = m_Playership->m_orientationDegrees;
		missile->m_faceDirection = m_Playership->m_faceDirection;
		for (int missileIndex = 0; missileIndex < MAX_MISSILE; missileIndex++)
		{
			if ((missileIndex == MAX_MISSILE - 1) && m_missile[missileIndex] != nullptr)
			{
				ERROR_RECOVERABLE("Can't spawn more missiles for you!!!");
				return;
			}

			if (m_missile[missileIndex] == nullptr)
			{
				m_missile[missileIndex] = missile;
				return;
			}
		}
	}
}

void Game::SpawnBeetle()
{
	Beetle* btle = nullptr;

	for (int beetleIndex = 0 ; beetleIndex < MAX_BEETLES ; beetleIndex ++)
	{
		if ((beetleIndex == MAX_BEETLES - 1) && m_beetle[beetleIndex] != nullptr)
		{
			ERROR_RECOVERABLE("Can't Spawn new beetles");
			return;
		}

		if (m_beetle[beetleIndex] == nullptr)
		{
			btle = new Beetle(this, Vec2(500.f,500.f));
			btle->StartUp();
			btle->m_position = GeneratePositionOffscreen(btle->m_cosmeticRadius,false);
			m_beetle[beetleIndex] = btle;
			break;
		}

	}

	for (int entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
	{
		if (m_entities[entityIndex] == nullptr)
		{
			m_entities[entityIndex] = btle;
			return;
		}
	}
}

void Game::SpawnWasp()
{
	Wasp* wasp = nullptr;

	for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
	{
		if ((waspIndex == MAX_WASPS - 1) && m_wasps[waspIndex] != nullptr)
		{
			ERROR_RECOVERABLE("Can't Spawn new wasps");
			return;
		}

		if (m_wasps[waspIndex] == nullptr)
		{
			wasp = new Wasp(this, Vec2(500.f, 500.f));
			wasp->StartUp();
			wasp->m_position = GeneratePositionOffscreen(wasp->m_cosmeticRadius,false);
			m_wasps[waspIndex] = wasp;
			break;
		}
	}

	for (int entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
	{
		if (m_entities[entityIndex] == nullptr)
		{
			m_entities[entityIndex] = wasp;
			return;
		}

	}
}

void Game::SpawnMultipleDebrisForEntity(Vec2& spawnPosition, int spawnNumber, Rgba8& color,float cosmeticRadius , Vec2& initVelocity)
{
	for (int spawnIndex = 0; spawnIndex < spawnNumber; spawnIndex++)
	{
		for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++)
		{
			if ((debrisIndex == MAX_DEBRIS - 1) && m_debris[debrisIndex] != nullptr)
			{
				ERROR_RECOVERABLE("Can't Spawn new debris");
				return;
			}

			if (m_debris[debrisIndex] == nullptr)
			{
				Debris* debris = new Debris(this, spawnPosition);
				debris->m_cosmeticRadius = cosmeticRadius;
				debris->StartUp();
				debris->SetColor(color);
				debris->m_initialVelocity = initVelocity;
				m_debris[debrisIndex] = debris;
				break;
			}
		}
	}
}


void Game::GenerateWaves(int waveIndex)
{
	SoundID waveSound = g_theAudio->CreateOrGetSound("Data/Audio/Wavebegin.wav");
	g_theAudio->StartSound(waveSound);

	if (m_waveindex == 0)		// Beginning of the whole game
	{
		SpawnAsteroidAtBeginning();
		SpawnBeetle();
	}
	else
	{
 		int asteroidNum = 6 * m_waveindex + 5;
		int beetleNum = 3*waveIndex + 1;
		int waspNum = (waveIndex - 1) * 2;

		for (int i = 0; i < asteroidNum; i++)
		{
			SpawnOneAsteroid();
		}

		for (int i = 0; i < beetleNum; i++)
		{
			SpawnBeetle();
		}

		for (int i = 0; i < waspNum; i++)
		{
			SpawnWasp();
		}
	}

}


void Game::Update()
{
	XboxController ctrler = g_theInput->GetController(0);

	if (m_enterAttractMode)
	{
		UpdateAttractMode();
		return;
	}

	CheckCollision();

	if (g_theInput->IsKeyDown(KEYCODE_ESC)&& !g_theInput->WasKeyJustPressed(KEYCODE_ESC))
	{
		g_theApp->ReloadGame();
		return;
	}

	if (g_theInput->IsKeyDown('I') && !g_theInput->WasKeyJustPressed('I'))
	{
		SpawnOneAsteroid();
	}

	if (g_theInput->IsKeyDown(KEYCODE_F1) && !g_theInput->WasKeyJustPressed(KEYCODE_F1))
	{
		m_developerMode = !m_developerMode;
	}


	m_Playership->Update();

	UpdateBullets();

	UpdateMissile();

	UpdateAsteroids();

	UpdateEnemies();

	UpdateDebris();

	UpdatePickups();

	DeleteAllGarbage();

	UpdateEntities();

	if (m_invokeEndTimer)	// Invoke this function when player is already dead but still need to render animation
	{
		InvokeEndTimer();
	}

}

void Game::UpdateAttractMode()
{

	m_attractMode[0]->SetOrientationDegrees(m_attractMode[0]->GetOrientationDegrees() + ATTRACTMODE_ROTATE_SPEED * m_gameClock->GetDeltaSeconds());
	m_attractMode[1]->SetOrientationDegrees(m_attractMode[1]->GetOrientationDegrees() - ATTRACTMODE_ROTATE_SPEED * m_gameClock->GetDeltaSeconds());

	for (int asteroidIndex = 0 ; asteroidIndex < MAX_ASTEROIDS_ATTRACTMODE ; asteroidIndex++) 
	{
		if (m_asteroidsForAttractMode[asteroidIndex] != nullptr)
		{
			m_asteroidsForAttractMode[asteroidIndex]->Update();
		}
	}

	CheckAttractAsteroidCollisionWithEdges();

	if (g_theInput->IsKeyDown(KEYCODE_ESC)&&!g_theInput->WasKeyJustPressed(KEYCODE_ESC))
	{
		m_enterAttractMode=false;
		g_theApp->Shutdown();
		return;
	}
	if (g_theInput->IsKeyDown(KEYCODE_SPACE)) 
	{
		m_enterAttractMode=false;
		SpawnStarsPosition();
		SpawnPlayerShip();
		SpawnPlayerUI();
		GenerateWaves(m_waveindex);
		return;
	}
	if (g_theInput->IsKeyDown('N'))
	{
		m_enterAttractMode = false;
		SpawnStarsPosition();
		SpawnPlayerShip();
		SpawnPlayerUI();
		GenerateWaves(m_waveindex);
		return;
	}
	if (g_theInput->GetController(0).IsButtonDown(XboxButtonID::XBOX_BUTTON_A))
	{
		m_enterAttractMode = false;
		SpawnStarsPosition();
		SpawnPlayerShip();
		SpawnPlayerUI();
		GenerateWaves(m_waveindex);
		return;
	}
	if (g_theInput->GetController(0).IsButtonDown(XboxButtonID::XBOX_BUTTON_START))
	{
		m_enterAttractMode = false;
		SpawnStarsPosition();
		SpawnPlayerShip();
		SpawnPlayerUI();
		GenerateWaves(m_waveindex);
		return;
	}

}

void Game::UpdateBullets()
{

	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{
		if (m_bullets[bulletIndex] != nullptr)
		{
			m_bullets[bulletIndex]->Update();

		}
		else
		{
			continue;
		}
	}

}

void Game::UpdateMissile()
{
	for (int missileIndex = 0; missileIndex < MAX_MISSILE; missileIndex++)
	{
		if (m_missile[missileIndex] != nullptr)
		{
			UpdateMissileAim(*m_missile[missileIndex]);
			m_missile[missileIndex]->Update();

		}
		else
		{
			continue;
		}
	}
}

void Game::UpdateMissileAim(Missile& missile)
{
	Entity* insideEntities[MISSILE_MAX_DETECTING_NUM] = {};
	int insideIndex = 0;

	for (int entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
	{
		if (m_entities[entityIndex]==nullptr)
		{
			continue;
		}

		Vec2 distanceVector = missile.m_position - m_entities[entityIndex]->m_position;
		float distance = distanceVector.GetLength();

		if (distance<=MISSILE_MAX_DETECTING_DISTANCE) 
		{
			insideEntities[insideIndex] = m_entities[entityIndex];
			insideEntities[insideIndex]->m_score = 20.f* (MISSILE_MAX_DETECTING_DISTANCE-distance) /MISSILE_MAX_DETECTING_DISTANCE;
			insideIndex++;
		}
	}

	Entity*	highestScoreEntity =nullptr;
	float	highestScore =0.0f;

	for (int entityIndex = 0; entityIndex <= insideIndex; entityIndex++)
	{
		if (insideEntities[entityIndex] == nullptr)
		{
			continue;
		}

		Vec2  entityDirection = insideEntities[entityIndex]->m_position - missile.m_position;
		float dotValue = DotProduct2D(missile.m_faceDirection.GetNormalized(),entityDirection.GetNormalized());
		float absDotValue = dotValue >= CosDegrees(MISSILE_DETECTING_MAX_ANGLE) ? dotValue : 0.f;

		insideEntities[entityIndex]->m_score += absDotValue * 80.f;

		if (insideEntities[entityIndex]->m_score > highestScore)
		{
			highestScore = insideEntities[entityIndex]->m_score;
			highestScoreEntity = insideEntities[entityIndex];
		}
	}
	if (highestScore>MISSILE_MIN_DETECTING_SCORE)
	{
		missile.SetTarget(highestScoreEntity);
	}
}

void Game::UpdateAsteroids()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		if (m_asteroids[asteroidIndex] == nullptr) continue;
		m_asteroids[asteroidIndex]->Update();
	}
}

void Game::UpdateEnemies()
{
	UpdateBeetles();
	UpdateWasps();
}

void Game::UpdateDebris() 
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++)
	{
		if (m_debris[debrisIndex] == nullptr)
		{
			continue;
		}
		else
		{
			m_debris[debrisIndex]->Update();
		}
	}
}

void Game::UpdateBeetles()
{
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
	{
		if (m_beetle[beetleIndex] == nullptr)
		{
			continue;
		}
		else 
		{
			if (m_Playership->IsAlive() && !m_Playership->GetIsInvulnerable())
			{
				m_beetle[beetleIndex]->SetPlayerStatus(true);
				m_beetle[beetleIndex]->FollowPlayer(m_Playership->m_position);
			}
			else 
			{
				m_beetle[beetleIndex]->SetPlayerStatus(false);
			}
			m_beetle[beetleIndex]->Update();
		}

	}
}

void Game::UpdatePickups()
{
	for (int pkIndex = 0; pkIndex < MAX_PICKUP; ++pkIndex)
	{
		if (m_pickups[pkIndex] != nullptr)
		{
			m_pickups[pkIndex]->Update();
		}
	}
}

void Game::UpdateWasps()
{
	for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
	{
		if (m_wasps[waspIndex] == nullptr)
		{
			continue;
		}
		else
		{
			if (m_Playership->IsAlive())
			{
				m_wasps[waspIndex]->SetPlayerStatus(true);
				m_wasps[waspIndex]->FollowPlayer(m_Playership->m_position);
			}
			else
			{
				m_wasps[waspIndex]->SetPlayerStatus(false);
			}
			m_wasps[waspIndex]->Update();
		}

	}
}

void Game::UpdateEntities() 
{
	for (int entityIndex = 0 ; entityIndex< MAX_ENTITIES; entityIndex ++)
	{
		if (m_entities[entityIndex]!=nullptr) 
		{
			return;
		}
	}

	if (m_waveindex < 2)
	{
		m_waveindex +=1;
		GenerateWaves(m_waveindex);
	}
	else 
	{
		g_theApp->ReloadGame();
	}
}


void Game::CheckCollision()
{

	CheckStarShipCollisionWithEntities();
	CheckStarshipCollisionWithEdges();

	CheckBulletCollisionWithEdges();
	CheckBulletCollisionWithEntities();
	CheckMissileCollisionWithEntities();

	CheckAsteroidCollisionWithEdges();
}

void Game::CheckStarshipCollisionWithEdges()
{
	Vec2& pos = m_Playership->m_position;
	float& radius = m_Playership->m_physicsRadius;
	Vec2& vel = m_Playership->m_velocity;

	if (pos.x > WORLD_SIZE_X - radius)
	{
		pos.x = WORLD_SIZE_X - radius;
		vel.x = -vel.x;
		vel.y = vel.y;
	}
	if (pos.x < radius)
	{
		pos.x = radius;
		vel.x = -vel.x;
		vel.y = vel.y;
	}
	if (pos.y > WORLD_SIZE_Y - radius)
	{
		pos.y = WORLD_SIZE_Y - radius;
		vel.x = vel.x;
		vel.y = -vel.y;
	}
	if (pos.y < radius)
	{
		pos.y = radius;
		vel.x = vel.x;
		vel.y = -vel.y;
	}
}

void Game::CheckStarShipCollisionWithEntities()
{
	if (m_Playership->m_isDead)
	{
		return;
	}

	Vec2 pos = m_Playership->m_position;
	float r = m_Playership->m_physicsRadius;

	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		if (m_asteroids[asteroidIndex] == nullptr)
		{
			continue;
		}

		Vec2 a_pos = m_asteroids[asteroidIndex]->m_position;

		float a_r = m_asteroids[asteroidIndex]->m_physicsRadius;

		if (DoDiscsOverlap(pos, r, a_pos, a_r) && !m_Playership->GetIsInvulnerable())
		{
			m_Playership->GetDamage(ASTEROID_DAMAGE);
			m_asteroids[asteroidIndex]->Die();
			break;
		}

	}
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
	{
		if (m_beetle[beetleIndex] == nullptr)
		{
			continue;
		}

		Vec2 b_pos = m_beetle[beetleIndex]->m_position;

		float b_r =  m_beetle[beetleIndex]->m_physicsRadius;

		if (DoDiscsOverlap(pos, r, b_pos, b_r) && !m_Playership->GetIsInvulnerable())
		{
			m_Playership->GetDamage(BEETLE_DAMAGE);
			m_beetle[beetleIndex]->GetDamage(1);
			break;
		}

	}
	for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
	{
		if (m_wasps[waspIndex] == nullptr)
		{
			continue;
		}

		Vec2 a_pos = m_wasps[waspIndex]->m_position;

		float a_r = m_wasps[waspIndex]->m_physicsRadius;

		if (DoDiscsOverlap(pos, r, a_pos, a_r) && !m_Playership->GetIsInvulnerable())
		{
			m_Playership->GetDamage(WASP_DAMAGE);
			m_wasps[waspIndex]->GetDamage(1);
			break;
		}

	}

	for (int pickupIndex = 0; pickupIndex < MAX_PICKUP; pickupIndex++)
	{
		if (m_pickups[pickupIndex] == nullptr)
		{
			continue;
		}

		Vec2 a_pos = m_pickups[pickupIndex]->m_position;

		float a_r = m_pickups[pickupIndex]->m_physicsRadius;

		if (DoDiscsOverlap(m_Playership->m_position, m_Playership->m_physicsRadius, a_pos, a_r))
		{
			m_Playership->SwitchWeapon(m_rng->RollRandomIntRange(1, 2));
			m_pickups[pickupIndex]->Die();
			return;
		}

	}
}

void Game::CheckBulletCollisionWithEdges()
{
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{

		if (m_bullets[bulletIndex] != nullptr)
		{
			if (m_bullets[bulletIndex]->m_position.x + m_bullets[bulletIndex]->m_cosmeticRadius < 0.f)
			{
				m_bullets[bulletIndex]->Die();
				continue;
			}
			if (m_bullets[bulletIndex]->m_position.x - m_bullets[bulletIndex]->m_cosmeticRadius > WORLD_SIZE_X)
			{
				m_bullets[bulletIndex]->Die();
				continue;
			}
			if (m_bullets[bulletIndex]->m_position.y + m_bullets[bulletIndex]->m_cosmeticRadius < 0.f)
			{
				m_bullets[bulletIndex]->Die();
				continue;
			}
			if (m_bullets[bulletIndex]->m_position.y - m_bullets[bulletIndex]->m_cosmeticRadius > WORLD_SIZE_Y)
			{
				m_bullets[bulletIndex]->Die();
				continue;
			}
		}
		else
		{
			continue;
		}
	}
}

void Game::CheckMissileCollisionWithEdges()
{

}

void Game::CheckMissileCollisionWithEntities()
{
	for (int missileIndex = 0; missileIndex < MAX_MISSILE; missileIndex++)
	{
		if (m_missile[missileIndex] != nullptr)
		{
			for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
			{
				if (m_asteroids[asteroidIndex] == nullptr)
				{
					continue;
				}

				Vec2 a_pos = m_asteroids[asteroidIndex]->m_position;

				float a_r = m_asteroids[asteroidIndex]->m_physicsRadius;

				if (DoDiscsOverlap(m_missile[missileIndex]->m_position, m_missile[missileIndex]->m_physicsRadius, a_pos, a_r))
				{
					//int spawnNum = m_rng->RollRandomIntRange(1, 3);
					//Missile* missle = m_missile[missileIndex];
					//SpawnMultipleDebrisForEntity(missle->m_position, spawnNum, m_asteroids[asteroidIndex]->m_color, DEBRIS_COSMETIC_RADIUS * 0.2f, missle->m_velocity);
					m_missile[missileIndex]->GetDamage(1);
					m_asteroids[asteroidIndex]->GetDamage(MISSILE_DAMAGE);
					return;
				}

			}
			for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
			{
				if (m_beetle[beetleIndex] == nullptr)
				{
					continue;
				}

				Vec2 a_pos = m_beetle[beetleIndex]->m_position;

				float a_r = m_beetle[beetleIndex]->m_physicsRadius;

				if (DoDiscsOverlap(m_missile[missileIndex]->m_position, m_missile[missileIndex]->m_physicsRadius, a_pos, a_r))
				{
					m_missile[missileIndex]->GetDamage(1);
					m_beetle[beetleIndex]->GetDamage(MISSILE_DAMAGE);
					return;
				}

			}
			for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
			{
				if (m_wasps[waspIndex] == nullptr)
				{
					continue;
				}

				Vec2 a_pos = m_wasps[waspIndex]->m_position;

				float a_r = m_wasps[waspIndex]->m_physicsRadius;

				if (DoDiscsOverlap(m_missile[missileIndex]->m_position, m_missile[missileIndex]->m_physicsRadius, a_pos, a_r))
				{
					m_missile[missileIndex]->GetDamage(1);
					m_wasps[waspIndex]->GetDamage(MISSILE_DAMAGE);
					return;
				}

			}
		}
		else
		{
			continue;
		}
	}

}

void Game::CheckBulletCollisionWithEntities()
{
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{
		if (m_bullets[bulletIndex] != nullptr)
		{
			for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
			{
				if (m_asteroids[asteroidIndex] == nullptr)
				{
					continue;
				}

				Vec2 a_pos = m_asteroids[asteroidIndex]->m_position;

				float a_r = m_asteroids[asteroidIndex]->m_physicsRadius;

				if (DoDiscsOverlap(m_bullets[bulletIndex]->m_position, m_bullets[bulletIndex]->m_physicsRadius, a_pos, a_r))
				{
					int spawnNum = m_rng->RollRandomIntRange(1, 3);
					Bullet* blt = m_bullets[bulletIndex];
					SpawnMultipleDebrisForEntity(blt->m_position, spawnNum, m_asteroids[asteroidIndex]->m_color,DEBRIS_COSMETIC_RADIUS*0.2f,blt->m_velocity);
					m_bullets[bulletIndex]->GetDamage(1);
					m_asteroids[asteroidIndex]->GetDamage(BULLET_DAMAGE);
					return;
				}

			}
			for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
			{
				if (m_beetle[beetleIndex] == nullptr)
				{
					continue;
				}

				Vec2 a_pos = m_beetle[beetleIndex]->m_position;

				float a_r = m_beetle[beetleIndex]->m_physicsRadius;

				if (DoDiscsOverlap(m_bullets[bulletIndex]->m_position, m_bullets[bulletIndex]->m_physicsRadius, a_pos, a_r))
				{
					int spawnNum = m_rng->RollRandomIntRange(1, 3);
					Bullet* blt = m_bullets[bulletIndex];
					SpawnMultipleDebrisForEntity(blt->m_position, spawnNum,m_beetle[beetleIndex]->m_color, DEBRIS_COSMETIC_RADIUS * 0.2f,blt->m_velocity);
					m_bullets[bulletIndex]->GetDamage(1);
					m_beetle[beetleIndex]-> GetDamage(BULLET_DAMAGE);
					return;
				}

			}

			for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
			{
				if (m_wasps[waspIndex] == nullptr)
				{
					continue;
				}

				Vec2 a_pos = m_wasps[waspIndex]->m_position;

				float a_r = m_wasps[waspIndex]->m_physicsRadius;

				if (DoDiscsOverlap(m_bullets[bulletIndex]->m_position, m_bullets[bulletIndex]->m_physicsRadius, a_pos, a_r))
				{
					int spawnNum = m_rng->RollRandomIntRange(1, 3);
					Bullet* blt = m_bullets[bulletIndex];
					SpawnMultipleDebrisForEntity(blt->m_position, spawnNum, m_wasps[waspIndex]->m_color, DEBRIS_COSMETIC_RADIUS * 0.2f,blt->m_velocity);
					m_bullets[bulletIndex]->GetDamage(1);
					m_wasps[waspIndex]->GetDamage(BULLET_DAMAGE);
					return;
				}

			}


		}
		else
		{
			continue;
		}
	}
}

void Game::CheckAttractAsteroidCollisionWithEdges()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_ATTRACTMODE; asteroidIndex++)
	{
		if (m_asteroidsForAttractMode[asteroidIndex] != nullptr)
		{
			if (m_asteroidsForAttractMode[asteroidIndex]->m_position.x + m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius < 0.f)
			{
				m_asteroidsForAttractMode[asteroidIndex]->m_position.x = WORLD_SIZE_X + m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
			if (m_asteroidsForAttractMode[asteroidIndex]->m_position.x - m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius > WORLD_SIZE_X)
			{
				m_asteroidsForAttractMode[asteroidIndex]->m_position.x = -m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
			if (m_asteroidsForAttractMode[asteroidIndex]->m_position.y + m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius < 0.f)
			{
				m_asteroidsForAttractMode[asteroidIndex]->m_position.y = WORLD_SIZE_Y + m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
			if (m_asteroidsForAttractMode[asteroidIndex]->m_position.y - m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius > WORLD_SIZE_Y)
			{
				m_asteroidsForAttractMode[asteroidIndex]->m_position.y = -m_asteroidsForAttractMode[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
		}
		else
		{
			continue;
		}
	}
}

void Game::CheckAsteroidCollisionWithEdges()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		if (m_asteroids[asteroidIndex] != nullptr)
		{
			if (m_asteroids[asteroidIndex]->m_position.x + m_asteroids[asteroidIndex]->m_cosmeticRadius < 0.f)
			{
				m_asteroids[asteroidIndex]->m_position.x = WORLD_SIZE_X + m_asteroids[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
			if (m_asteroids[asteroidIndex]->m_position.x - m_asteroids[asteroidIndex]->m_cosmeticRadius > WORLD_SIZE_X)
			{
				m_asteroids[asteroidIndex]->m_position.x = -m_asteroids[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
			if (m_asteroids[asteroidIndex]->m_position.y + m_asteroids[asteroidIndex]->m_cosmeticRadius < 0.f)
			{
				m_asteroids[asteroidIndex]->m_position.y = WORLD_SIZE_Y + m_asteroids[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
			if (m_asteroids[asteroidIndex]->m_position.y - m_asteroids[asteroidIndex]->m_cosmeticRadius > WORLD_SIZE_Y)
			{
				m_asteroids[asteroidIndex]->m_position.y = -m_asteroids[asteroidIndex]->m_cosmeticRadius;
				continue;
			}
		}
		else
		{
			continue;
		}
	}
}


void Game::Render() const
{

	if (m_enterAttractMode)
	{
		g_theRenderer->BeginCamera(*m_attractCamera);
		g_theRenderer->ClearScreen(CLEAR_COLOR);
		RenderAttractMode();
		g_theRenderer->EndCamera(*m_attractCamera);
		return;
	}

	g_theRenderer->BeginCamera(*m_worldCamera);
	g_theRenderer->ClearScreen(CLEAR_COLOR);

	RenderBackGroundStars();
	RenderPlayerShip();
	RenderAsteroids();
	RenderBullets();
	RenderBeetles();
	RenderMissiles();
	RenderWasps();
	RenderDebris();
	RenderPickups();
	RenderDeveloperMode();

	g_theRenderer->EndCamera(*m_worldCamera);

	g_theRenderer->BeginCamera(*m_screenCamera);
	RenderPlayerUI();
	g_theRenderer->EndCamera(*m_screenCamera);
}

void Game::RenderAttractMode() const
{
	if (!m_enterAttractMode)
	{
		return;
	}

	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_ATTRACTMODE; asteroidIndex++)
	{
		if (m_asteroidsForAttractMode[asteroidIndex] != nullptr)
		{
			m_asteroidsForAttractMode[asteroidIndex]->RenderFlame();
			m_asteroidsForAttractMode[asteroidIndex]->Render();
		}
	}

	m_attractMode[0]->RenderText("Starship Gold",Vec2(55.f,75.f), 10.f, Rgba8(255,150,50,255));

	m_attractMode[0]->RenderAttractModeShip();
	m_attractMode[1]->RenderAttractModeShip();
	m_attractMode[2]->RenderAttractModePlayButton();

}

void Game::RenderBackGroundStars() const
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
	for (int starPositionIndex = 0; starPositionIndex < MAX_STARS_NUMBER; ++starPositionIndex)
	{
		r =	 static_cast<unsigned char>(m_stars[starPositionIndex].m_color.r);
		g =  static_cast<unsigned char>(m_stars[starPositionIndex].m_color.g);
		b =  static_cast<unsigned char>(m_stars[starPositionIndex].m_color.b);
		a =  static_cast<unsigned char>(m_stars[starPositionIndex].m_color.a);

		Vertex_PCU shipVertsGlobal[]
		{

			Vertex_PCU(-0.5f, 0.0f,	r,g,b,a),
			Vertex_PCU(0.5f,0.0f,	r,g,b,a),
			Vertex_PCU(0.0f,2.0f,	r,g,b,a),

			Vertex_PCU(-0.5f, 0.0f,	r,g,b,a),
			Vertex_PCU(0.5f,0.0f,	r,g,b,a),
			Vertex_PCU(0.0f,-2.0f,	r,g,b,a),

			Vertex_PCU(0.0f, 0.5f,	r,g,b,a),
			Vertex_PCU(0.0f,-0.5f,	r,g,b,a),
			Vertex_PCU(-2.0f, 0.0f,	r,g,b,a),

			Vertex_PCU(0.0f, 0.5f,	r,g,b,a),
			Vertex_PCU(0.0f,-0.5f,	r,g,b,a),
			Vertex_PCU(2.0f, 0.0f,	r,g,b,a),

		};

		int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
		Vec2 startPosition = Vec2(m_stars[starPositionIndex].m_position.x, m_stars[starPositionIndex].m_position.y);
		TransformVertexArrayXY3D(vertNum, shipVertsGlobal, 0.2f, 0.0f,startPosition );
		g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
	}
}

void Game::RenderPlayerShip() const
{
	m_Playership->Render();
}

void Game::RenderAsteroids() const
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		if (m_asteroids[asteroidIndex] == nullptr)
		{
			continue;
		}
		m_asteroids[asteroidIndex]->Render();
	}
}

void Game::RenderBullets() const
{
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{
		if (m_bullets[bulletIndex] != nullptr)
		{
			m_bullets[bulletIndex]->Render();
			DebugDrawGlow(m_bullets[bulletIndex]->m_position, m_bullets[bulletIndex]->m_cosmeticRadius + 1.0f, 1.0f, Rgba8(255, 51, 51));
		}
		else
		{
			continue;
		}
	}
}

void Game::RenderMissiles() const
{
	for (int missileIndex = 0; missileIndex < MAX_MISSILE; missileIndex++)
	{
		if (m_missile[missileIndex] != nullptr)
		{
			m_missile[missileIndex]->Render();
		}
		else
		{
			continue;
		}
	}

}

void Game::RenderDebris() const 
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++)
	{
		if (m_debris[debrisIndex] == nullptr)
		{
			continue;
		}
		m_debris[debrisIndex]->Render();
	}
}

void Game::RenderPickups() const
{
	for (int pkIndex = 0; pkIndex < MAX_PICKUP; ++pkIndex)
	{
		if (m_pickups[pkIndex] != nullptr)
		{
			m_pickups[pkIndex]->Render();
		}
	}
}

void Game::RenderBeetles() const
{
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
	{
		if (m_beetle[beetleIndex] == nullptr)
		{
			continue;
		}
		m_beetle[beetleIndex]->Render();

	}
}

void Game::RenderWasps() const
{
	for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
	{
		if (m_wasps[waspIndex] == nullptr)
		{
			continue;
		}
		m_wasps[waspIndex]->Render();

	}
}

void Game::RenderPlayerUI() const
{
	RenderPlayerLives();
	//RenderPlayerMissileCoolDown();
}


void Game::RenderPlayerLives() const
{
	Vec2 startDrawPosition(80.0f,700.0f);
	Vec2 heartPosition(40.f, 720.f);
	Vec2 barPosition(140.0f,730.f);
	if (m_playerUiTool != nullptr)
	{
		m_playerUiTool->RenderHeart(heartPosition,0.0f,10.f);
		m_playerUiTool->RenderHpBar(barPosition, 0.0f, 10.f,(m_Playership->m_health *13.0f/ PLAYER_HP) - 6.5f);
		for (int liveIndex = 0; liveIndex < m_Playership->m_respawnTimes; liveIndex++)
		{
			m_playerUiTool->RenderPlayership(startDrawPosition, 90.f, 5.f);
			startDrawPosition += Vec2(30.f, 0.f);
		}
	}
}

void Game::RenderPlayerMissileCoolDown() const
{	
	Vec2 centerPosition(135.f, 75.f);
	Vec2 startDrawPosition = centerPosition + Vec2(0.f, -5.0f);
	Vec2 box1DrawPosition  = centerPosition + Vec2(-60.f, 0.0f);
	Vec2 box2DrawPosition = centerPosition;
	Vec2 box3DrawPosition = centerPosition + Vec2(60.f, 0.0f);
	Vec2 titlePosition = centerPosition + Vec2(-45.0f, 55.0f);

	if (m_playerUiTool != nullptr)
	{
		m_playerUiTool->RenderCanvasBox(box1DrawPosition, 0.5f, 80.f);
		m_playerUiTool->RenderCanvasBox(box2DrawPosition, 0.5f, 80.f);
		m_playerUiTool->RenderCanvasBox(box3DrawPosition, 0.5f, 80.f);
		m_playerUiTool->RenderMissile(startDrawPosition, 90.0f, 10.0f, 50);
		m_playerUiTool->RenderText("Weapons", titlePosition, 20.f, Rgba8(255, 255, 255, 255));
	}
}

void Game::RenderDeveloperMode() const
{
	if (!m_developerMode) return;

	//	Draw Grey lines for player ship
	if (GetPlayerShip()->IsAlive()) 
	{
		Vec2 pos = m_Playership->m_position;
		Vec2 fwd = m_Playership->GetForwardNormal();
		Vec2 lft = fwd.GetRotated90Degrees();
		Vec2 vlc = m_Playership->m_velocity;
		float csr = m_Playership->m_cosmeticRadius;
		float psr = m_Playership->m_physicsRadius;

		DebugDrawRing(pos, csr, 0.2f, Rgba8(255, 0, 255, 255));
		DebugDrawRing(pos, psr, 0.2f, Rgba8(0, 255, 255, 255));
		DebugDrawLine(pos, pos + fwd * csr, 0.2f, Rgba8(255, 0, 0, 255));
		DebugDrawLine(pos, pos + lft * csr, 0.2f, Rgba8(0, 255, 0, 255));
		DebugDrawLine(pos, pos + vlc, 0.2f, Rgba8(255, 255, 0, 255));

	}
	// --------------------------------------------------------------------------

	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{
		if (m_bullets[bulletIndex] != nullptr)
		{
			Vec2 pos = m_bullets[bulletIndex]->m_position;
			Vec2 fwd = m_bullets[bulletIndex]->GetForwardNormal();
			Vec2 lft = fwd.GetRotated90Degrees();
			Vec2 vlc = m_bullets[bulletIndex]->m_velocity;
			float csr = m_bullets[bulletIndex]->m_cosmeticRadius;
			float psr = m_bullets[bulletIndex]->m_physicsRadius;

			DebugDrawRing(pos, csr, 0.2f, Rgba8(255, 0, 255, 255));
			DebugDrawRing(pos, psr, 0.2f, Rgba8(0, 255, 255, 255));
			DebugDrawLine(m_Playership->m_position, pos, 0.2f, Rgba8(50, 50, 50, 255));
			DebugDrawLine(pos, pos + fwd * csr, 0.2f, Rgba8(255, 0, 0, 255));
			DebugDrawLine(pos, pos + lft * csr, 0.2f, Rgba8(0, 255, 0, 255));
			DebugDrawLine(pos, pos + vlc, 0.2f, Rgba8(255, 255, 0, 255));
		}
		else
		{
			continue;
		}
	}

	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		if (m_asteroids[asteroidIndex] != nullptr)
		{
			Vec2 pos = m_asteroids[asteroidIndex]->m_position;
			Vec2 fwd = m_asteroids[asteroidIndex]->GetForwardNormal();
			Vec2 lft = fwd.GetRotated90Degrees();
			Vec2 vlc = m_asteroids[asteroidIndex]->m_velocity;
			float csr = m_asteroids[asteroidIndex]->m_cosmeticRadius;
			float psr = m_asteroids[asteroidIndex]->m_physicsRadius;

			DebugDrawRing(pos, csr, 0.2f, Rgba8(255, 0, 255, 255));
			DebugDrawRing(pos, psr, 0.2f, Rgba8(0, 255, 255, 255));
			DebugDrawLine(m_Playership->m_position, pos, 0.2f, Rgba8(50, 50, 50, 255));
			DebugDrawLine(pos, pos + fwd * csr, 0.2f, Rgba8(255, 0, 0, 255));
			DebugDrawLine(pos, pos + lft * csr, 0.2f, Rgba8(0, 255, 0, 255));
			DebugDrawLine(pos, pos + vlc, 0.2f, Rgba8(255, 255, 0, 255));
		}
		else
		{
			continue;
		}
	}

	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
	{
		if (m_beetle[beetleIndex] == nullptr)
		{
			continue;
		}
		Vec2 pos = m_beetle[beetleIndex]->m_position;
		Vec2 fwd = m_beetle[beetleIndex]->GetForwardNormal();
		Vec2 lft = fwd.GetRotated90Degrees();
		Vec2 vlc = m_beetle[beetleIndex]->m_velocity;
		float csr = m_beetle[beetleIndex]->m_cosmeticRadius;
		float psr = m_beetle[beetleIndex]->m_physicsRadius;

		DebugDrawRing(pos, csr, 0.2f, Rgba8(255, 0, 255, 255));
		DebugDrawRing(pos, psr, 0.2f, Rgba8(0, 255, 255, 255));
		DebugDrawLine(m_Playership->m_position, pos, 0.2f, Rgba8(50, 50, 50, 255));
		DebugDrawLine(pos, pos + fwd * csr, 0.2f, Rgba8(255, 0, 0, 255));
		DebugDrawLine(pos, pos + lft * csr, 0.2f, Rgba8(0, 255, 0, 255));
		DebugDrawLine(pos, pos + vlc, 0.2f, Rgba8(255, 255, 0, 255));

	}

	for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
	{
		if (m_wasps[waspIndex] == nullptr)
		{
			continue;
		}

		Vec2 pos = m_wasps[waspIndex]->m_position;
		Vec2 fwd = m_wasps[waspIndex]->GetForwardNormal();
		Vec2 lft = fwd.GetRotated90Degrees();
		Vec2 vlc = m_wasps[waspIndex]->m_velocity;
		float csr = m_wasps[waspIndex]->m_cosmeticRadius;
		float psr = m_wasps[waspIndex]->m_physicsRadius;

		DebugDrawRing(pos, csr, 0.2f, Rgba8(255, 0, 255, 255));
		DebugDrawRing(pos, psr, 0.2f, Rgba8(0, 255, 255, 255));
		DebugDrawLine(m_Playership->m_position, pos, 0.2f, Rgba8(50, 50, 50, 255));
		DebugDrawLine(pos, pos + fwd * csr, 0.2f, Rgba8(255, 0, 0, 255));
		DebugDrawLine(pos, pos + lft * csr, 0.2f, Rgba8(0, 255, 0, 255));
		DebugDrawLine(pos, pos + vlc, 0.2f, Rgba8(255, 255, 0, 255));

	}

	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++)
	{
		if (m_debris[debrisIndex] != nullptr)
		{
			Vec2 pos =m_debris[debrisIndex]->m_position;
			Vec2 fwd = m_debris[debrisIndex]->GetForwardNormal();
			Vec2 lft = fwd.GetRotated90Degrees();
			Vec2 vlc = m_debris[debrisIndex]->m_velocity;
			float csr = m_debris[debrisIndex]->m_cosmeticRadius;
			//float psr = m_debris[debrisIndex]->m_physicsRadius;

			DebugDrawRing(pos, csr, 0.2f, Rgba8(255, 0, 255, 255));
			DebugDrawLine(m_Playership->m_position, pos, 0.2f, Rgba8(50, 50, 50, 255));
			DebugDrawLine(pos, pos + fwd * csr, 0.2f, Rgba8(255, 0, 0, 255));
			DebugDrawLine(pos, pos + lft * csr, 0.2f, Rgba8(0, 255, 0, 255));
			DebugDrawLine(pos, pos + vlc, 0.2f, Rgba8(255, 255, 0, 255));
		}
		else 
		{
			continue;
		}

	}
	//	-----------------------------------------------------------------------------------
}


void Game::DeleteAllGarbage()
{
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{
		if (m_bullets[bulletIndex] == nullptr) continue;
		if (m_bullets[bulletIndex]->m_isGarbage)
		{
			delete m_bullets[bulletIndex];
			m_bullets[bulletIndex] = nullptr;
		}
	}

	for (int missileIndex = 0; missileIndex < MAX_MISSILE; missileIndex++)
	{
		if (m_missile[missileIndex] == nullptr) continue;
		if (m_missile[missileIndex]->m_isGarbage)
		{
			delete m_missile[missileIndex];
			m_missile[missileIndex] = nullptr;
		}
	}

	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		if (m_asteroids[asteroidIndex] == nullptr) continue;
		if (m_asteroids[asteroidIndex]->m_isGarbage)
		{
   			delete m_asteroids[asteroidIndex];
			m_asteroids[asteroidIndex] = nullptr;
		}
	}

	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
	{
		if (m_beetle[beetleIndex] == nullptr)
		{
			continue;
		}

		if (m_beetle[beetleIndex]->m_isGarbage)
		{
			delete m_beetle[beetleIndex];
			m_beetle[beetleIndex] = nullptr;
		}

	}

	for (int waspIndex = 0; waspIndex < MAX_WASPS; waspIndex++)
	{
		if (m_wasps[waspIndex] == nullptr)
		{
			continue;
		}

		if (m_wasps[waspIndex]->m_isGarbage)
		{
			delete m_wasps[waspIndex];
			m_wasps[waspIndex] = nullptr;
		}

	}

	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++)
	{
		if (m_debris[debrisIndex] == nullptr)
		{
			continue;
		}
		if (m_debris[debrisIndex]->m_isGarbage)
		{
			delete m_debris[debrisIndex];
			m_debris[debrisIndex] = nullptr;
		}
	}

	for (int pickupIndex = 0; pickupIndex < MAX_PICKUP; pickupIndex++)
	{
		if (m_pickups[pickupIndex] == nullptr)
		{
			continue;
		}
		if (m_pickups[pickupIndex]->m_isGarbage)
		{
			delete m_pickups[pickupIndex];
			m_pickups[pickupIndex] = nullptr;
		}
	}

	for (int entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
	{
		if (m_entities[entityIndex] != nullptr)
		{
			if (m_entities[entityIndex]->m_isGarbage)
			{
 				m_entities[entityIndex] =nullptr;
			}
		}
	}
}


Vec2 Game::GeneratePositionOffscreen(float const& cosmeticRadius,bool onlyTopRight)
{

	int i = 0;
	if (onlyTopRight) 
	{
		i = m_rng->RollRandomIntRange(1,2);;
	}
	else 
	{
		i = m_rng->RollRandomIntLessThan(4);
	}

	float randomX = 0.f;
	float randomY = 0.f;

	switch (i)
	{
		case 0:
			randomX = -cosmeticRadius;
			randomY = m_rng->RollRandomFloatInRange(-cosmeticRadius, WORLD_SIZE_Y + cosmeticRadius);
			break;
		case 1:
			randomX = WORLD_SIZE_X + cosmeticRadius;
			randomY = m_rng->RollRandomFloatInRange(-cosmeticRadius, WORLD_SIZE_Y + cosmeticRadius);
			break;
		case 2:
			randomX = m_rng->RollRandomFloatInRange(cosmeticRadius, WORLD_SIZE_X - cosmeticRadius);
			randomY = WORLD_SIZE_Y + cosmeticRadius;
			break;
		case 3:
			randomX = m_rng->RollRandomFloatInRange(cosmeticRadius, WORLD_SIZE_X - cosmeticRadius);
			randomY = -cosmeticRadius;
			break;
	}

	return Vec2(randomX, randomY);
}

void Game::InvokeEndTimer()
{
	if (m_endTimer < PLAYER_END_MAX_TIMER)
	{
		m_endTimer += m_gameClock->GetDeltaSeconds();
		m_worldCamera->SetOrthographicView(WORLD_CAMERA_BOTTOMLEFT, WORLD_CAMERA_TOPRIGHT);
		m_screenCamera->SetOrthographicView(SCREEN_CAMERA_BOTTOMLEFT, SCREEN_CAMERA_TOPRIGHT);
		m_attractCamera->SetOrthographicView(ATTRACT_CAMERA_BOTTOMLEFT, ATTRACT_CAMERA_TOPRIGHT);
		float xoffset = m_rng->RollRandomFloatInRange(0.f, 3.0f);
		float yoffset = m_rng->RollRandomFloatInRange(0.f, 3.0f);
		m_worldCamera->Translate2D(Vec2(xoffset, yoffset));
	}
	else 
	{
		m_worldCamera->SetOrthographicView(WORLD_CAMERA_BOTTOMLEFT, WORLD_CAMERA_TOPRIGHT);
		m_screenCamera->SetOrthographicView(SCREEN_CAMERA_BOTTOMLEFT, SCREEN_CAMERA_TOPRIGHT);
		m_attractCamera->SetOrthographicView(ATTRACT_CAMERA_BOTTOMLEFT, ATTRACT_CAMERA_TOPRIGHT);
		if (m_Playership->m_respawnTimes==0 &&!m_Playership->IsAlive())
		{
			g_theApp->ReloadGame();
		}
	}
}

void Game::ResetEndTimer()
{
	m_endTimer =0.f;
	m_invokeEndTimer = false;
}

bool Game::ChangeTimeScale(EventArgs const& args)
{
	float timescale = args.GetValue("value", -1.0f);
	if (timescale > 0.0f)
	{
		g_theApp->m_theGame->m_gameClock->SetTimeScale(timescale);
		g_theConsole->AddLine(g_theConsole->INFO_MINOR, Stringf("timescale set correctly to %f!", timescale));
		return true;
	}

	return false;
}
