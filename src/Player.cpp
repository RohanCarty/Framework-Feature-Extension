#include "Player.h"
#include "Particle.h"
#include "Projectile.h"
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

	//Test sprite for now, will load actual one once we have it.
	m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/CharacterTemplate2.animated", SceneManager::GetDisplayManager());

	//Set some defaults and initialise some variables.

	m_iSpecial1Ability = eHeal;

	m_iSpecial2Ability = eBurst;

	m_bIsUsingAbility = false;

	m_fAbilityCurrentCastTime = 0.0f;

	m_fAbilityCastTime = 1.0f;

	m_iAbilityBeingCast = eNone;

	m_bIsAlive = true;

	m_iJumpSpeed = 525;

	m_bJumpLatch = false;

	m_iCurrentSoulPowerLevel = 0;

	m_vCurrentMaxSpeed = m_vMaxSpeed; //Copy defaults in on in there.

	m_iAttackPower = 50;

	m_iCurrentAttackPower = m_iAttackPower; //More defaults.

	m_fAttackCooldown = 1.0f;
	m_fCurrentAttackCooldown = 0.0f;

	BindToController();

	m_bIsGravityOn = true;
    
    m_apkRenderables[0].m_pkTexture->SwitchAnimation("Idle");

	SetScale(0.8f);
	SetSize(GetSize() * GetScale());
    
    m_iObjectType = ePlayer;

	m_bInvincible = false;

	((GameScene*)m_pkScene)->m_pkHUD->PopulatePlayerInfos();
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

	//Updating status of abilities
	if(m_bIsUsingAbility)
	{
		UpdateAbilityStatus(a_fDeltaTime);

		m_bControlsLocked = true; // keep controls locked while ability is running.

		SetVelocity(0,0,0);

		//If using ability, check if button has been released.
		/*if(m_iAbilityBeingCast == m_iSpecial1Ability)
		{
			if(!SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bSpecial1Pressed)
			{
				StopCastingAbility();
			}
		}

		if(m_iAbilityBeingCast == m_iSpecial2Ability)
		{
			if(!SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bSpecial2Pressed)
			{
				StopCastingAbility();
			}
		}*/
	}

	//Testing progress bar and showing attack cooldowns while there is still no matching animation.
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
		//SwitchAnimation to Idle
		m_apkRenderables[0].m_pkTexture->SwitchAnimation("Idle");
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

		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bSpecial1Pressed && m_pVelocity->y == 0.0f)
		{
			if(m_fCurrentAttackCooldown <= 0.0f)
			{
				BeginCastingAbility(m_iSpecial1Ability);
			}
		}

		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bSpecial2Pressed && m_pVelocity->y == 0.0f)
		{
			if(m_fCurrentAttackCooldown <= 0.0f)
			{
				BeginCastingAbility(m_iSpecial2Ability);
			}
		}
		
		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bViewPressed && m_pVelocity->y == 0.0f)
		{
			if(m_fCurrentAttackCooldown <= 0.0f)
			{
				BeginCastingAbility(eRevive);
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

	if(m_pVelocity->y == 0.0f && m_vCollisionVector.y > 0 && m_bControlsLocked && m_bIsAlive)
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

	StopCastingAbility();

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
	//If alive, call death functions.
	if(m_bIsAlive)
	{
		SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/playerdeath.ogg");
		//Move to a death state.
		m_bInvincible = true;
		m_apkRenderables[0].m_bIsHidden = true;
		m_bControlsLocked = true;
		m_bIsAlive = false;
		SetVelocity(0,0,0);
		SetHealth(0);
		//Respawn();
	}
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

	m_iCurrentAttackPower = FloatLerp((float)(m_iAttackPower / 5), (float)m_iAttackPower, 1.0f - (float)a_iNewSoulPowerLevel / 100.0f);

	m_vCurrentMaxSpeed.x = FloatLerp((float)m_vMaxSpeed.x / 2, (float)m_vMaxSpeed.x, 1.0f - (float)a_iNewSoulPowerLevel / 100.0f); //a_iNewSoulPowerLevel will be between 0 and 100;

	m_iCurrentSoulPowerLevel = a_iNewSoulPowerLevel;
}

bool Player::GetIsAlive()
{
	return m_bIsAlive;
}

void Player::BeginCastingAbility(int a_iAbilityBeingCast)
{
	switch(a_iAbilityBeingCast)
	{
	case eHeal:
		if(GetHealth() == 100)
		{
			//TODO: Figure a way for this not to be spammed.
			//SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Health Already Full");
		}
		else if(m_iCurrentSoulPowerLevel >= 25)
		{
			m_bIsUsingAbility = true; // Is used to enable checking if a special button has been released.
			m_bControlsLocked = true; // Stops the player from moving.
			m_fAbilityCurrentCastTime = 0.0f;
			m_fAbilityCastTime = 1.0f; // Set this to the time it'll take for this ability to be cast.
			m_iAbilityBeingCast = a_iAbilityBeingCast;
		}
		else
		{
			SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Not enough power for Heal");
		}
		break;
	case eHealParty:
		if(m_iCurrentSoulPowerLevel >= 50)
		{
			m_bIsUsingAbility = true; // Is used to enable checking if a special button has been released.
			m_bControlsLocked = true; // Stops the player from moving.
			m_fAbilityCurrentCastTime = 0.0f;
			m_fAbilityCastTime = 3.0f; // Set this to the time it'll take for this ability to be cast.
			m_iAbilityBeingCast = a_iAbilityBeingCast;
		}
		else
		{
			SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Not enough power for Heal Party");
		}
		break;
	case eRevive:
		//TODO: Check to see if all players are already alive.
		if(m_iCurrentSoulPowerLevel >= 100)
		{
			m_bIsUsingAbility = true; // Is used to enable checking if a special button has been released.
			m_bControlsLocked = true; // Stops the player from moving.
			m_fAbilityCurrentCastTime = 0.0f;
			m_fAbilityCastTime = 5.0f; // Set this to the time it'll take for this ability to be cast.
			m_iAbilityBeingCast = a_iAbilityBeingCast;
		}
		else
		{
			SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Not enough power for Revive");
		}
		break;
	case eBurst:
		if(m_iCurrentSoulPowerLevel >= 5)
		{
			m_bIsUsingAbility = true; // Is used to enable checking if a special button has been released.
			m_bControlsLocked = true; // Stops the player from moving.
			m_fAbilityCurrentCastTime = 0.0f;
			m_fAbilityCastTime = 0.5f; // Set this to the time it'll take for this ability to be cast.
			m_iAbilityBeingCast = a_iAbilityBeingCast;
		}
		else
		{
			SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), "Not enough power for Burst");
		}
		break;
	default:
		std::cout<<"BeginCastingAbility switch default on Player: "<<this<<std::endl;
	}
}

void Player::StopCastingAbility()
{
	m_bIsUsingAbility = false; // Is used to enable checking if a special button has been released.
	m_bControlsLocked = false; // Stops the player from moving.
	m_fAbilityCurrentCastTime = 0.0f;
	m_fAbilityCastTime = 1.0f; // Set this to the time it'll take for this ability to be cast.
	m_iAbilityBeingCast = eNone;
}

void Player::UpdateAbilityStatus(float a_fDeltaTime)
{
	m_fAbilityCurrentCastTime += a_fDeltaTime;

	Vector vTempPosition = *GetLocation();
	vTempPosition.y -= GetSize().y;
	((GameScene*)m_pkScene)->m_pkHUD->DrawProgressBar(vTempPosition,Vector(128,32,0), m_fAbilityCurrentCastTime / m_fAbilityCastTime);

	vTempPosition.y -= GetSize().y / 3;

	Particle* pTempParticle = SceneManager::GetParticleManager()->SpawnFloatingText(vTempPosition, GetStringOfNameOfAbility());

	// Show floating text for only one frame.

	pTempParticle->SetLifetime(0.0f);

	pTempParticle->SetVelocity(Vector(0,0,0));

	if(m_fAbilityCurrentCastTime > m_fAbilityCastTime)
	{
		ApplyAbility();
	}
}

void Player::ApplyAbility()
{
	//Uniform cast sound for now.
	SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/healing.ogg");

	//Position of floating text once ability is cast.
	Vector vTempPosition = *GetLocation();

	vTempPosition.y -= GetSize().y;

	vTempPosition.y -= GetSize().y / 3;

	switch(m_iAbilityBeingCast)
	{
	case eHeal:
		SceneManager::GetParticleManager()->SpawnFloatingText(vTempPosition, "Heal");

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

		break;
	case eHealParty:
		SceneManager::GetParticleManager()->SpawnFloatingText(vTempPosition, "Heal Party");
		//Heal entire party.
		for(unsigned int uiDx = 0; uiDx < SceneManager::GetUnitManager()->GetPlayerList().size(); uiDx++)
		{
			//Healing is for 50 points, hardcoded
			if(SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetHealth() > 50)
			{
				SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->SetHealth(100);
			}
			else
			{
				SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->SetHealth(GetHealth() + 50);
			}

			SetCurrentSoulPowerLevel(GetCurrentSoulPowerLevel() - 50);
		}

		break;
	case eRevive:
		SceneManager::GetParticleManager()->SpawnFloatingText(vTempPosition, "Revive");

		for(unsigned int uiDx = 0; uiDx < SceneManager::GetUnitManager()->GetPlayerList().size(); uiDx++)
		{
			//Revive any players who're dead.
			if(!SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetIsAlive())
			{
				SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->Respawn();

				SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->SetLocation(*GetLocation());

				SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->SetHealth(100);
			}
		}
		
		SetCurrentSoulPowerLevel(GetCurrentSoulPowerLevel() - 100);

		break;
	case eBurst:
		SceneManager::GetParticleManager()->SpawnFloatingText(vTempPosition, "Burst");

		//Spawn and set the velocity of a projectile
		SceneManager::GetUnitManager()->SpawnProjectile();

		if(m_apkRenderables[0].m_pkTexture->GetIsTextureFlipped())
		{
			SceneManager::GetUnitManager()->GetProjectileList().back()->SetVelocity(Vector(-1500,0,0));
			SceneManager::GetUnitManager()->GetProjectileList().back()->GetRenderables()[0].m_pkTexture->FlipTexture(true);
		}
		else
		{
			SceneManager::GetUnitManager()->GetProjectileList().back()->SetVelocity(Vector(1500,0,0));
		}

		SceneManager::GetUnitManager()->GetProjectileList().back()->SetLocation(*GetLocation());

		SceneManager::GetUnitManager()->GetProjectileList().back()->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/Burst.png", SceneManager::GetDisplayManager());

		SceneManager::GetUnitManager()->GetProjectileList().back()->SetDamage(50);

		SetCurrentSoulPowerLevel(GetCurrentSoulPowerLevel() - 5);

		break;
	default:
		std::cout<<"ApplyAbility switch default on Player: "<<this<<std::endl;
	}

	//Reset self;
	StopCastingAbility();
}

std::string Player::GetStringOfNameOfAbility()
{
	switch(m_iAbilityBeingCast)
	{
	case eHeal:
		return std::string("Heal");
		break;
	case eHealParty:
		return std::string("Heal Party");
		break;
	case eRevive:
		return std::string("Revive");
		break;
	case eBurst:
		return std::string("Burst");
		break;
	default:
		std::cout<<"ApplyAbility switch default on Player: "<<this<<std::endl;
		return std::string("Default");
	}
}