#include "Player.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "HUD.h"
#include "Vector.h"
#include "InputManager.h"
#include "AnimatedTexture.h"
#include "CollectibleSoul.h"

Player::Player(Scene* a_pkScene) : Actor(a_pkScene)
{
    //std::cout<<"Unit created. Pointer: "<<this<<std::endl;

	//Pick a random character sprite for now, allow players to choose one later.
	if(rand() % 2 + 1 == 2)
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/pinkiepie.animated", SceneManager::GetDisplayManager());
	}
	else
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/applejack.animated", SceneManager::GetDisplayManager());
	}

	//Set some defaults and initialise some variables.
	
	m_bIsAlive = true;

	m_iJumpSpeed = 525;

	m_bJumpLatch = false;

	m_iCurrentSoulPowerLevel = 0;

	m_vCurrentMaxSpeed = m_vMaxSpeed; //Copy defaults in on in there.

	m_iAttackPower = 25;

	m_iCurrentAttackPower = m_iAttackPower; //More defaults.

	m_fAttackCooldown = 0.75f;
	m_fCurrentAttackCooldown = 0.0f;

	BindToController();

	m_bIsGravityOn = true;
    
    m_apkRenderables[0].m_pkTexture->SwitchAnimation("Standing");

	SetScale(0.8f);
	SetSize(GetSize() * GetScale());
    
    m_iObjectType = ePlayer;

	m_bInvincible = false;
}

Player::~Player()
{
    std::cout<<"Player Destroyed. Pointer: "<<this<<std::endl;
}

bool Player::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Actor Tick: "<<this<<std::endl;
    #endif

	//Testing progress bar
	if(m_fCurrentAttackCooldown > 0.0f)
	{
		Vector vTempPosition = *GetLocation();
		vTempPosition.y += GetSize().y;
		((GameScene*)m_pkScene)->m_pkHUD->DrawProgressBar(vTempPosition,Vector(128,32,0), m_fCurrentAttackCooldown);

		m_fCurrentAttackCooldown -= a_fDeltaTime;
	}

	//Changing animation based on changed direction
	if(m_pVelocity->x == 0.0f)
	{
		//SwitchAnimation to standing
		m_apkRenderables[0].m_pkTexture->SwitchAnimation("Standing");
		m_iCurrentDirection = 0;
	}
	if(m_pVelocity->x < 0.0f)
	{
		//SwitchAnimation to running
		m_apkRenderables[0].m_pkTexture->SwitchAnimation("Running");
		
		if(!m_bControlsLocked)
		{
			m_apkRenderables[0].m_pkTexture->FlipTexture(true);
		}

		m_iCurrentDirection = -1;
	}
	if(m_pVelocity->x > 0.0f)
	{
		//SwitchAnimation to running
		m_apkRenderables[0].m_pkTexture->SwitchAnimation("Running");
		
		if(!m_bControlsLocked)
		{
			m_apkRenderables[0].m_pkTexture->FlipTexture(false);
		}

		m_iCurrentDirection = 1;
	}

	if(!m_bControlsLocked)
	{
		//TODO: Remove this
		//Escape for if there are no controllers connected.
		if(m_iControllerNumberBoundTo == -1)
		{
			std::cout<<"No Controller"<<std::endl;
			return true;
		}

		//Jump detection
		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bJumpPressed && !m_bJumpLatch)
		{
			Jump();
		}

		//Attack detection
		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bAttackPressed)
		{
			if(m_fCurrentAttackCooldown <= 0.0f)
			{
				m_fCurrentAttackCooldown = m_fAttackCooldown;
				Attack(a_fDeltaTime);
			}
		}

		//Heal
		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bSpecialPressed)
		{
			if(GetHealth() == 100)
			{
				SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Health Already Full");
			}
			else if(m_iCurrentSoulPowerLevel >= 25)
			{
				SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Heal!");

				SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/healing.ogg");

				//Healing is for 25 points, hardcoded
				if(GetHealth() > 75)
				{
					SetHealth(100);
				}
				else
				{
					SetHealth(GetHealth() + 25);
				}

				SetCurrentSoulPowerLevel(GetCurrentSoulPowerLevel() - 25);
			}
			else
			{
				SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Not enough power for Heal");
			}
		}

		//Deceleration
		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).fAxis1X == 0)
		{
			if(m_pVelocity->x < -100)
			{
				m_pVelocity->x += (m_iAcceleration * 0.5);
			}
			else if(m_pVelocity->x > 100)
			{
				m_pVelocity->x -= (m_iAcceleration * 0.5);
			}
			else
			{
				m_pVelocity->x = 0;
			}
		}
		else
		{
			m_pVelocity->x += (m_iAcceleration * SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).fAxis1X);
		}

		//TODO: Work on physics.

		//See if the player ran into a soul shard.
		if(IsCollidingWithActorNextFrame(a_fDeltaTime, m_pLocation))
		{
			for(unsigned int uiDx = 0; uiDx < m_apkIsCollidingWithNextFame.size(); uiDx++)
			{
				if(m_apkIsCollidingWithNextFame[uiDx]->m_iObjectType == eSoul) //Still isn't strictly type-safe.
				{
					//TODO: Put this into a much better and easier function
					SetCurrentSoulPowerLevel(GetCurrentSoulPowerLevel() + ((CollectibleSoul*)m_apkIsCollidingWithNextFame[uiDx])->GetSoulLevelContained());
					((CollectibleSoul*)m_apkIsCollidingWithNextFame[uiDx])->SetSoulCollected();
				}
			}
		}
	}

	//Falling respawn
	if(GetLocation()->y > 800)
	{
		Respawn();
	}

	if(m_pVelocity->y == 0.0f && m_vCollisionVector.y > 0 && m_bControlsLocked)
	{
		m_bControlsLocked = false;
		m_bInvincible = false;
	}

	Actor::Update(a_fDeltaTime);

	//Speed cap for reduced stat characters
	if(m_pVelocity->x > m_vCurrentMaxSpeed.x)
		m_pVelocity->x = m_vCurrentMaxSpeed.x;

	if(m_pVelocity->x < -m_vCurrentMaxSpeed.x)
		m_pVelocity->x = -m_vCurrentMaxSpeed.x;

	if(IsCollidingWithTileNextFrame(a_fDeltaTime, m_pLocation))
	{
		if(GetCollisionVector().y > 0)
		{
			if(m_bJumpLatch == true)
			{
				SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/landing.ogg");
				m_bJumpLatch = false;
			}
		}
	}

    return true;
}

void Player::Jump()
{
	m_pVelocity->y -= m_iJumpSpeed;

	m_bJumpLatch = true;

	SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/jumping.ogg");

	//std::cout<<"Jump Latched"<<std::endl;
}

void Player::Hurt()
{
	SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/playerhurt.ogg");

	return;
}

void Player::Attack(float a_fDeltaTime)
{
	//Attack in front of the player, determine direction then do a check of all actors there and damage/knockback them.

	SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/playerswing.ogg");

	//Create a temp vector, this will be modified based on direction then added to the current location to get the attack point
	Vector vTemp;
	if(m_iCurrentDirection != 0)
	{
		vTemp.x = m_iCurrentDirection * (GetSize().x / 1);
	}
	else
	{
		vTemp.x = -((int)m_apkRenderables[0].m_pkTexture->GetIsTextureFlipped() * 2 - 1) * (GetSize().x / 1);
	}

	vTemp = *GetLocation() + vTemp;
    
	//Ensure checking collision like this doesn't cause any issues.

	//Clear collision list
	while(m_apkIsCollidingWithNextFame.size() > 0)
	{
		m_apkIsCollidingWithNextFame.pop_back();
	}

	if(IsCollidingWithActorNextFrame(a_fDeltaTime, &vTemp))
	{
		for(unsigned int uiDx = 0; uiDx < m_apkIsCollidingWithNextFame.size(); uiDx++)
		{
			if(m_apkIsCollidingWithNextFame[uiDx]->m_iObjectType == eUnit)
			{
				//TODO: Proper damage
				if(!((Actor*)m_apkIsCollidingWithNextFame[uiDx])->GetIsInvincible()) //Not invinciible
				{
					SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/playerhit.ogg");
					((Actor*)m_apkIsCollidingWithNextFame[uiDx])->SetHealth(((Actor*)m_apkIsCollidingWithNextFame[uiDx])->GetHealth() - m_iCurrentAttackPower);
					SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), m_iCurrentAttackPower);
				}
			}
		}
	}
}

void Player::BindToController()
{
	int iTemp = SceneManager::GetInputManager()->GetControllerForPlayer();

	std::cout<<"Got a controller: "<<iTemp<<std::endl;

	m_iControllerNumberBoundTo = iTemp;
}

void Player::Death()
{
	SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/playerdeath.ogg");
	//Move to a death state.
	m_bInvincible = true;
	m_apkRenderables[0].m_bIsHidden = true;
	m_bControlsLocked = true;
	m_bIsAlive = false;
	SetVelocity(0,0,0);
	//Respawn();
}

void Player::Respawn()
{
	SetLocation(0,-192,0);
	SetVelocity(0,0,0);
	SetHealth(100);
	m_bControlsLocked = false;
	m_bInvincible = false;
	m_bJumpLatch = false;
	m_apkRenderables[0].m_bIsHidden = false;
	m_bIsAlive = true;
}

int Player::GetCurrentSoulPowerLevel()
{
	return m_iCurrentSoulPowerLevel;
}

void Player::SetCurrentSoulPowerLevel(int a_iNewSoulPowerLevel)
{
	if(a_iNewSoulPowerLevel < 0)
	{
		a_iNewSoulPowerLevel = 0;
	}
	else if(a_iNewSoulPowerLevel > 100)
	{
		a_iNewSoulPowerLevel = 100;
	}

	m_iCurrentAttackPower = FloatLerp(m_iAttackPower / 5, m_iAttackPower, 1.0f - (float)a_iNewSoulPowerLevel / 100.0f);

	m_vCurrentMaxSpeed.x = FloatLerp(m_vMaxSpeed.x / 2, m_vMaxSpeed.x, 1.0f - (float)a_iNewSoulPowerLevel / 100.0f); //a_iNewSoulPowerLevel will be between 0 and 100;

	m_iCurrentSoulPowerLevel = a_iNewSoulPowerLevel;
}

bool Player::GetIsAlive()
{
	return m_bIsAlive;
}