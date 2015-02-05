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

	m_vCurrentSpeed.x = 0;
	m_vCurrentSpeed.y = 0;
	m_vMaxSpeed.x = 400;
	m_vMaxSpeed.y = 600;

	m_iAcceleration = 45;

	m_iJumpSpeed = 400;
	m_iFallSpeed = 600;
	m_bJumpLatch = false;

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

    Object::Update(a_fDeltaTime);

	//TODO: Remove this
	//Escape for if there are no controllers connected.
	if(!SceneManager::GetInputManager()->GetIsControllerConnected(0))
	{
		return true;
	}

	if(SceneManager::GetInputManager()->GetControllerState(0).fAxis1X == 0)
	{
		if(m_vCurrentSpeed.x < -100)
		{
			m_vCurrentSpeed.x += (m_iAcceleration * 0.5);
		}
		else if(m_vCurrentSpeed.x > 100)
		{
			m_vCurrentSpeed.x -= (m_iAcceleration * 0.5);
		}
		else //Deceleration
		{
			m_vCurrentSpeed.x = 0;
		}
	}
	else
	{
		m_vCurrentSpeed.x += (m_iAcceleration * SceneManager::GetInputManager()->GetControllerState(0).fAxis1X);
	}

	//Speed cap

	if(m_vCurrentSpeed.x > m_vMaxSpeed.x)
		m_vCurrentSpeed.x = m_vMaxSpeed.x;

	if(m_vCurrentSpeed.x < -m_vMaxSpeed.x)
		m_vCurrentSpeed.x = -m_vMaxSpeed.x;

	//TODO: Work on physics.

	//Jump detection
	if(SceneManager::GetInputManager()->GetControllerState(0).bJumpPressed && !m_bJumpLatch)
	{
		Jump();
	}

	//Gravity alpha
	if(GetLocation()->y < 0)
	{
		m_vCurrentSpeed.y += m_iFallSpeed * a_fDeltaTime;

		//Fall speed cap

		if(m_vCurrentSpeed.y > m_vMaxSpeed.y)
		m_vCurrentSpeed.y = m_vMaxSpeed.y;
	}

	//Ground plane alpha
	if(GetLocation()->y > 0 && m_vCurrentSpeed.y != 0.0f)
	{
		SetLocation(GetLocation()->x, 0.0f, GetLocation()->z);
		m_vCurrentSpeed.y = 0.0f;
		m_bJumpLatch = false;
	}

	//std::cout<<"Location: "<<*GetLocation()<<std::endl;
	//std::cout<<"Velocity: "<<m_vCurrentSpeed<<std::endl;

	SetLocation(GetLocation()->x + (m_vCurrentSpeed.x * a_fDeltaTime), GetLocation()->y + (m_vCurrentSpeed.y * a_fDeltaTime), GetLocation()->z);

    return true;
}

void Player::Jump()
{
	m_vCurrentSpeed.y -= m_iJumpSpeed;

	m_bJumpLatch = true;
}