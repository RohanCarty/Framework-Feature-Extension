#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "Vector.h"
#include "InputManager.h"
#include "AnimatedTexture.h"

Player::Player(Scene* a_pkScene) : Actor(a_pkScene)
{
    //std::cout<<"Unit created. Pointer: "<<this<<std::endl;

	if(rand() % 2 + 1 == 2)
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/pinkiepie.animated", SceneManager::GetDisplayManager());
	}
	else
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/applejack.animated", SceneManager::GetDisplayManager());
	}

	m_iJumpSpeed = 525;

	m_bJumpLatch = false;

	BindToController();

	m_bIsGravityOn = true;
    
    m_apkRenderables[0].m_pkTexture->SwitchAnimation("Standing");

	SetScale(0.6f);
	SetSize(GetSize() * GetScale());
    
    m_iObjectType = ePlayer;
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
		//Jump detection
		if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bJumpPressed && !m_bJumpLatch)
		{
			Jump();
		}

		//TODO: Remove this
		//Escape for if there are no controllers connected.
		if(m_iControllerNumberBoundTo == -1)
		{
			std::cout<<"No Controller"<<std::endl;
			return true;
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

	if(IsCollidingWithTileNextFrame(a_fDeltaTime))
	{
		if(IsCollidingWithTileNextFrame(a_fDeltaTime))
		{
			if(GetCollisionVector().y > 0)
			{
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

	//std::cout<<"Jump Latched"<<std::endl;
}

void Player::BindToController()
{
	int iTemp = SceneManager::GetInputManager()->GetControllerForPlayer();

	std::cout<<"Got a controller: "<<iTemp<<std::endl;

	m_iControllerNumberBoundTo = iTemp;
}

void Player::Death()
{
	Respawn();
}

void Player::Respawn()
{
	SetLocation(0,-128,0);
	SetVelocity(0,0,0);
	SetHealth(100);
}