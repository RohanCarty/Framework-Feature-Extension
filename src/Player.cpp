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

	m_iCurrentSpeed = 0;
	m_iMaxSpeed = 400;
	m_iAcceleration = 15;

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

	if(SceneManager::GetInputManager()->GetControllerState(0).fAxis1X == 0)
	{
		if(m_iCurrentSpeed < -100)
		{
			m_iCurrentSpeed += (m_iAcceleration * 2);
		}
		else if(m_iCurrentSpeed > 100)
		{
			m_iCurrentSpeed -= (m_iAcceleration * 2);
		}
		else
		{
			m_iCurrentSpeed = 0;
		}
	}
	else
	{
		m_iCurrentSpeed += (m_iAcceleration * SceneManager::GetInputManager()->GetControllerState(0).fAxis1X);
	}

	//Speed cap

	if(m_iCurrentSpeed > m_iMaxSpeed)
		m_iCurrentSpeed = m_iMaxSpeed;

	if(m_iCurrentSpeed < -m_iMaxSpeed)
		m_iCurrentSpeed = -m_iMaxSpeed;

	SetLocation(GetLocation()->x + (m_iCurrentSpeed * a_fDeltaTime), GetLocation()->y, GetLocation()->z);

    return true;
}