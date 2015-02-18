#include "Actor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Tile.h"
#include "Unit.h"
#include "Player.h"

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
    
    m_iHealth = 100;

	//Collision stuff
	m_bIsCollidingTileNextFrame = false;
    m_bIsCollidingActorNextFrame = false;
	m_bIsCollidingTileNextFrameSet = false;
    m_bIsCollidingActorNextFrameSet = false;
    
    m_iObjectType = eActor;
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
    
    if(m_iHealth <= 0)
    {
		Death();
    }

	//Reset Collision Latch
	m_bIsCollidingTileNextFrameSet = false;
    m_bIsCollidingActorNextFrameSet = false;
    
    //Clear collision list
    while(m_apkIsCollidingWithNextFame.size() > 0)
    {
        m_apkIsCollidingWithNextFame.pop_back();
    }

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
	if(m_bIsCollidingTileNextFrameSet)
		return m_bIsCollidingTileNextFrame;

	m_bIsCollidingTileNextFrameSet = true;
    m_bIsCollidingTileNextFrame = false;
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
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}

		vTemp = vCalculatedPosition;
		vTemp.x -= (GetSize().x / 2);


		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}

		vTemp = vCalculatedPosition;
		vTemp.y += (GetSize().y / 2);


		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}

		vTemp = vCalculatedPosition;
		vTemp.y -= (GetSize().y / 2);


		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}
        
        if(m_bIsCollidingTileNextFrame)
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
    
    bool m_bIsCollidingNextFrame2 = m_bIsCollidingTileNextFrame;
    m_bIsCollidingTileNextFrame = false;
    
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
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.x -= (GetSize().x / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y += (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y -= (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetTileManager()->GetTileList()[uiDx]->GetLocation(), SceneManager::GetTileManager()->GetTileList()[uiDx]->GetSize()))
		{
			m_bIsCollidingTileNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetTileManager()->GetTileList()[uiDx]);
		}
        
        if(m_bIsCollidingTileNextFrame)
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

    if(m_bIsCollidingTileNextFrame || m_bIsCollidingNextFrame2)
    {
        m_bIsCollidingTileNextFrame = true;
        return true;
    }
    
	m_bIsCollidingTileNextFrame = false;
	return false;
}

bool Actor::IsCollidingWithActorNextFrame(float a_fDeltaTime)
{
	if(m_bIsCollidingActorNextFrameSet)
		return m_bIsCollidingActorNextFrame;
    
	m_bIsCollidingActorNextFrameSet = true;
    m_bIsCollidingActorNextFrame = false;
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
    
	for(unsigned int uiDx = 0; uiDx < SceneManager::GetUnitManager()->GetActorList().size(); uiDx++)
	{
		vTemp = vCalculatedPosition;
		vTemp.x += (GetSize().x / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.x -= (GetSize().x / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y += (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y -= (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
		}
        
        if(m_bIsCollidingActorNextFrame)
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
    
    bool m_bIsCollidingNextFrame2 = m_bIsCollidingActorNextFrame;
    m_bIsCollidingActorNextFrame = false;
    
    //X Collision
    //Remove Y component, so can extrapolate direction
    fVelTemp = m_pVelocity->y;
    m_pVelocity->y = 0.0f;
    vCalculatedPosition = (*GetLocation() + (*m_pVelocity * a_fDeltaTime));
    m_pVelocity->y = fVelTemp;
    
	//Check all four corners against all tiles twice, once for X velocity, once for Y velocity.
	//TODO: See if this is way too slow.
    
	for(unsigned int uiDx = 0; uiDx < SceneManager::GetUnitManager()->GetActorList().size(); uiDx++)
	{
		vTemp = vCalculatedPosition;
		vTemp.x += (GetSize().x / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.x -= (GetSize().x / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y += (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
		}
        
		vTemp = vCalculatedPosition;
		vTemp.y -= (GetSize().y / 2);
        
        
		if(vTemp.WithinBox2D(*SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetLocation(), SceneManager::GetUnitManager()->GetActorList()[uiDx]->GetSize()))
		{
			m_bIsCollidingActorNextFrame = true;
			m_apkIsCollidingWithNextFame.push_back(SceneManager::GetUnitManager()->GetActorList()[uiDx]);
        }
        
        if(m_bIsCollidingActorNextFrame)
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
    
    if(m_bIsCollidingActorNextFrame || m_bIsCollidingNextFrame2)
    {
        m_bIsCollidingActorNextFrame = true;
        return true;
    }
    
	m_bIsCollidingActorNextFrame = false;
	return false;
}

void Actor::SetHealth(int a_iNewHealth)
{
    m_iHealth = a_iNewHealth;
    
    //std::cout<<"Setting new health: "<<m_iHealth<<std::endl;
}

int Actor::GetHealth()
{
    return m_iHealth;
}

void Actor::Death()
{
	return;
}

Vector Actor::GetCollisionVector()
{
    return m_vCollisionVector;
}