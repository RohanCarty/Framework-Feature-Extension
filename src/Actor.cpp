#include "Actor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Tile.h"

Actor::Actor(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Actor created. Pointer: "<<this<<std::endl;

	//Actors have velocity over an object, objects being static.
    m_pVelocity = new Vector();

	m_vMaxSpeed.x = 400;
	m_vMaxSpeed.y = 600;

	m_iAcceleration = 45;

	m_iJumpSpeed = 400;
	m_iFallSpeed = 600;
	
	m_bIsGravityOn = false;

	//Collision stuff
	m_bIsCollidingNextFrame = false;
	m_pkIsCollidingWithNextFame = NULL;
	m_bIsCollidingNextFrameSet = false;
}

Actor::~Actor()
{
    //std::cout<<"Actor Destroyed. Pointer: "<<this<<std::endl;

    delete m_pVelocity;
}

bool Actor::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Actor Tick: "<<this<<std::endl;
    #endif

	//Reset Collision Latch
	m_bIsCollidingNextFrameSet = false;

    Object::Update(a_fDeltaTime);

	//Speed cap
	if(m_pVelocity->x > m_vMaxSpeed.x)
		m_pVelocity->x = m_vMaxSpeed.x;

	if(m_pVelocity->x < -m_vMaxSpeed.x)
		m_pVelocity->x = -m_vMaxSpeed.x;

    //Check for ground
    
    if(m_bIsGravityOn)
	{
		ApplyGravity(a_fDeltaTime);
	}
    
    if(IsCollidingWithTileNextFrame(a_fDeltaTime))
	{
		//SetLocation(GetLocation()->x, 0.0f, GetLocation()->z);
        if(m_vCollisionVector.y > 0)
        {
            m_pVelocity->y = 0.0f;
        }
        if(m_vCollisionVector.y < 0)
        {
            m_pVelocity->y = 0.0f;
        }
        if(m_vCollisionVector.x != 0)
        {
            m_pVelocity->x = 0.0f;
        }
	}
    
    SetLocation(GetLocation()->x + (m_pVelocity->x * a_fDeltaTime), GetLocation()->y + (m_pVelocity->y * a_fDeltaTime), GetLocation()->z + (m_pVelocity->z * a_fDeltaTime));

    return true;
}


void Actor::SetVelocity(Vector a_NewVelocity)
{
    m_pVelocity->x = a_NewVelocity.x;
    m_pVelocity->y = a_NewVelocity.y;
	m_pVelocity->z = a_NewVelocity.z;
}

void Actor::SetVelocity(double a_x, double a_y, double a_z)
{
    m_pVelocity->x = a_x;
    m_pVelocity->y = a_y;
	m_pVelocity->z = a_z;
}


void Actor::ApplyGravity(float a_fDeltaTime)
{
	//Gravity alpha
	m_pVelocity->y += m_iFallSpeed * a_fDeltaTime;

	//Fall speed cap

	if(m_pVelocity->y > m_vMaxSpeed.y)
	m_pVelocity->y = m_vMaxSpeed.y;
}

bool Actor::IsCollidingWithTileNextFrame(float a_fDeltaTime)
{
	if(m_bIsCollidingNextFrameSet)
		return m_bIsCollidingNextFrame;

	m_bIsCollidingNextFrameSet = true;
    m_bIsCollidingNextFrame = false;
    m_vCollisionVector = Vector(0,0,0);

    //Y Collision
    //Remove X component, so can extrapolate direction
    double fVelTemp = m_pVelocity->x;
    m_pVelocity->x = 0.0f;
	Vector vCalculatedPosition = (*GetLocation() + (*m_pVelocity * a_fDeltaTime));
	Vector vTemp;
    m_pVelocity->x = fVelTemp;

	//Check all four corners against all tiles twice, once for X velocity, once for Y velocity.
	//TODO: See if this is way too slow.

	for(unsigned int uiDx = 0; uiDx < SceneManager::GetTileManager()->GetTileList().size(); uiDx++)
	{
		vTemp = vCalculatedPosition;
		vTemp.x += (GetSize().x / 2);


		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}

		vTemp = vCalculatedPosition;
		vTemp.x -= (GetSize().x / 2);


		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}

		vTemp = vCalculatedPosition;
		vTemp.y += (GetSize().y / 2);


		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}

		vTemp = vCalculatedPosition;
		vTemp.y -= (GetSize().y / 2);


		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}
        
        if(m_bIsCollidingNextFrame)
        {
            if(m_pVelocity->y > 0.0)
            {
                m_vCollisionVector.y = 1.0;
            }
            else if(m_pVelocity->y == 0.0)
            {
                m_vCollisionVector.y = 0.0;
            }
            else
            {
                m_vCollisionVector.y = -1.0;
            }
        }
	}
    
    bool m_bIsCollidingNextFrame2 = m_bIsCollidingNextFrame;
    m_bIsCollidingNextFrame = false;
    
    //X Collision
    //Remove Y component, so can extrapolate direction
    fVelTemp = m_pVelocity->y;
    m_pVelocity->y = 0.0f;
    vCalculatedPosition = (*GetLocation() + (*m_pVelocity * a_fDeltaTime));
    m_pVelocity->y = fVelTemp;
    
	//Check all four corners against all tiles twice, once for X velocity, once for Y velocity.
	//TODO: See if this is way too slow.
    
	for(unsigned int uiDx = 0; uiDx < SceneManager::GetTileManager()->GetTileList().size(); uiDx++)
	{
		vTemp = vCalculatedPosition;
		vTemp.x += (GetSize().x / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}
        
		vTemp = vCalculatedPosition;
		vTemp.x -= (GetSize().x / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y += (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y -= (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingNextFrame = true;
			m_pkIsCollidingWithNextFame = SceneManager::GetTileManager()->GetTileList()[uiDx];
		}
        
        if(m_bIsCollidingNextFrame)
        {
            if(m_pVelocity->x > 0.0)
            {
                m_vCollisionVector.x = 1.0;
            }
            else if(m_pVelocity->x == 0.0)
            {
                m_vCollisionVector.x = 0.0;
            }
            else
            {
                m_vCollisionVector.x = -1.0;
            }
        }
	}

    if(m_bIsCollidingNextFrame || m_bIsCollidingNextFrame2)
    {
        m_bIsCollidingNextFrame = true;
        return true;
    }
    
	m_bIsCollidingNextFrame = false;
	return false;

	//Ground plane alpha
	/*if((*GetLocation() + (*m_pVelocity * a_fDeltaTime)).y > 0 && m_pVelocity->y != 0.0f)
	{
		m_bIsCollidingNextFrame = true;
		return true;
	}
	else
		m_bIsCollidingNextFrame = false;
		return false;*/
}

Vector Actor::GetCollisionVector()
{
    return m_vCollisionVector;
}