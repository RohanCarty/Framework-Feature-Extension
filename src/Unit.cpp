#include "Unit.h"
#include "Building.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "Vector.h"
#include "AnimatedTexture.h"
#include "InputManager.h"
#include "Camera.h"
#include "TileManager.h"
#include "Resource.h"

#include <fstream>

Unit::Unit(Scene* a_pkScene) : Actor(a_pkScene)
{
    //std::cout<<"Unit created. Pointer: "<<this<<std::endl;

	m_iUnitType = eCitizen;

    //Pick a random direction.
	m_iCurrentDirection = rand()% 2;
    
    if(m_iCurrentDirection == 0)
    {
        m_iCurrentDirection = -1;
    }
    
	if(rand() % 2 + 1 == 1)
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/pinkiepie.animated", SceneManager::GetDisplayManager());
	}
	else
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/applejack.animated", SceneManager::GetDisplayManager());
	}

    m_vMaxSpeed.x = 400 + (rand() % 50 - 25);
	m_vMaxSpeed.y = 600;
    
	m_iAcceleration = 25 + (rand() % 20 - 10);
    
    m_iUnitNumber = 0;
    
    m_pDestination = new Vector();

	m_pkTarget = NULL;

	SetScale(0.6f);
	SetSize(GetSize() * GetScale());
    
    m_apkRenderables[0].m_pkTexture->SwitchAnimation("Standing");
    
    m_bIsGravityOn = true;

	m_bInvincible = false;

	m_iHealth = 20;

	switch (rand() % 12)
	{
	case 0:
		m_svName = "Gundars Kalve";
		break;
	case 1:
		m_svName = "Haralds Sirmais";
		break;
	case 2:
		m_svName = "Stefans Igaunis";
		break;
	case 3:
		m_svName = "Vukan Letvin";
		break;
	case 4:
		m_svName = "Ivan Fontikov";
		break;
	case 5:
		m_svName = "Gorzyslaw Zhukov";
		break;
	case 6:
		m_svName = "Lasma Aplocins";
		break;
	case 7:
		m_svName = "Tekla Kurzemnieks";
		break;
	case 8:
		m_svName = "Amalija Zilite";
		break;
	case 9:
		m_svName = "Veselina Zajczyk";
		break;
	case 10:
		m_svName = "Mieczyslawa Dworkin";
		break;
	case 11:
		m_svName = "Tanya Kaffka";
		break;
	default:
		m_svName = "RNG is broken, wut";
		break;
	}
    
    m_iObjectType = eUnit;
}

Unit::~Unit()
{
    std::cout<<"Unit Destroyed. Pointer: "<<this<<std::endl;

    delete m_pDestination;
}

void Unit::SetUnitNumber(int a_iNewNumber)
{
    m_iUnitNumber = a_iNewNumber;
}

bool Unit::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Player Tick: "<<this<<std::endl;
    #endif

	//Check to make sure we're not already dead
	if(GetHealth() <= 0)
	{
		return false;
	}

	if(!m_bControlsLocked)
	{
		//Go in randomly picked direction.
    
		//If Velocity on the x is zero then assume that we've hit a wall, change direction.
		if(m_pVelocity->x == 0.0)
		{
			m_iCurrentDirection *= -1;
		}

		m_pVelocity->x += (m_iAcceleration * m_iCurrentDirection);
    
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
			m_apkRenderables[0].m_pkTexture->FlipTexture(true);
			m_iCurrentDirection = -1;
		}
		if(m_pVelocity->x > 0.0f)
		{
			//SwitchAnimation to running
			m_apkRenderables[0].m_pkTexture->SwitchAnimation("Running");
			m_apkRenderables[0].m_pkTexture->FlipTexture(false);
			m_iCurrentDirection = 1;
		}
	}
    
    Actor::Update(a_fDeltaTime);
    
	if(IsCollidingWithActorNextFrame(a_fDeltaTime, m_pLocation) && !m_bControlsLocked)
    {
        for(unsigned int uiDx = 0; uiDx < m_apkIsCollidingWithNextFame.size(); uiDx++)
        {
            if(m_apkIsCollidingWithNextFame[uiDx]->m_iObjectType == ePlayer)
            {
                //TODO: Proper damage
				if(!((Actor*)m_apkIsCollidingWithNextFame[uiDx])->GetIsInvincible()) //Not invinciible
				{
					((Actor*)m_apkIsCollidingWithNextFame[uiDx])->SetHealth(((Actor*)m_apkIsCollidingWithNextFame[uiDx])->GetHealth() - 10);
					SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), 10);
				}
            }
        }
    }

	//kill enemy if it's fallen through the level.
	if(GetLocation()->y > 800)
	{
		return false;
	}

	if(m_pVelocity->y == 0.0f && m_bControlsLocked)
	{
		m_bControlsLocked = false;
		m_bInvincible = false;
	}

    return true;
}

void Unit::SetDestination(Vector a_vNewDestination, bool a_bReplicate)
{
    m_pDestination->x = a_vNewDestination.x;
    m_pDestination->y = a_vNewDestination.y;
    m_pDestination->z = a_vNewDestination.z;
    
    //if connected to or is a server
    if(a_bReplicate)
    {
        NetworkUpdateLocationAndDestination();
    }
}

void Unit::SetLocation(Vector a_vNewLocation, bool a_bReplicate)
{
    Actor::SetLocation(a_vNewLocation);
    if(a_bReplicate)
    {
        NetworkUpdateLocationAndDestination();
    }
}

void Unit::SetHardLocation(double a_x, double a_y, double a_z, bool a_bReplicate)
{
	m_pLocation->x = a_x;
    m_pLocation->y = a_y;
	m_pLocation->z = a_z;
	m_pDestination->x = a_x;
    m_pDestination->y = a_y;
	m_pDestination->z = a_z;
    
    if(a_bReplicate)
    {
        NetworkUpdateLocationAndDestination();
    }
}

void Unit::SetHardLocation(Vector a_vNewDestination, bool a_bReplicate)
{
    *m_pLocation = a_vNewDestination;
    *m_pDestination = a_vNewDestination;
    
    if(a_bReplicate)
    {
        NetworkUpdateLocationAndDestination();
    }
}

void Unit::NetworkUpdateLocationAndDestination()
{
    //Build a command packet and send it
    //Check to see if connected, first of all
    if(!SceneManager::GetNetworkManager()->GetIsConnected())
    {
        //Not conneceted, so jump out
        return;
    }
    
    stCommandPacket stTempCommand;
    
    //Send Current Location;
    stTempCommand.m_iOrder = eOrderSetLocation;
    stTempCommand.m_iUnit = m_iUnitNumber;
    stTempCommand.m_iClient = 0;
    stTempCommand.m_vFirstVector = *m_pLocation;
    stTempCommand.m_vSecondVector = *m_pLocation; // Fill the entire command fuckhead
    SceneManager::GetNetworkManager()->AddCommand(&stTempCommand);
}

int Unit::GetUnitType()
{
	return m_iUnitType;
}

std::string Unit::GetUnitTypeString()
{
	switch(m_iUnitType)
	{
		case eCitizen:
			return "Citizen";
			break;
		default:
			return "Generic Unit";
			break;
	}
}

std::string Unit::GetName()
{
	return m_svName;
}