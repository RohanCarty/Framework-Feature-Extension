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

	if(rand() % 2 + 1 == 1)
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/elusive.animated", SceneManager::GetDisplayManager());
	}
	else
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/applejack.animated", SceneManager::GetDisplayManager());
	}

	
	m_bJumpLatch = false;

	BindToController();

	m_bIsGravityOn = true;

	SetScale(0.6f);
	SetSize(GetSize() * GetScale());
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

    Actor::Update(a_fDeltaTime);

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

	//Jump detection
	if(SceneManager::GetInputManager()->GetControllerState(m_iControllerNumberBoundTo).bJumpPressed && !m_bJumpLatch)
	{
		Jump();
	}

	if(IsCollidingWithTileNextFrame(a_fDeltaTime))
	{
		m_bJumpLatch = false;
	}

    return true;
}

void Player::Jump()
{
	m_pVelocity->y -= m_iJumpSpeed;

	m_bJumpLatch = true;
}

void Player::BindToController()
{
	int iTemp = SceneManager::GetInputManager()->GetControllerForPlayer();

	std::cout<<"Got a controller: "<<iTemp<<std::endl;

	m_iControllerNumberBoundTo = iTemp;
}