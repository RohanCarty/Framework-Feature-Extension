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

	m_iCurrentTask = eIdle;

	if(rand() % 2 + 1 == 1)
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/elusive.animated", SceneManager::GetDisplayManager());
	}
	else
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/applejack.animated", SceneManager::GetDisplayManager());
	}

	m_uiSelectedShaderNumber = SceneManager::GetDisplayManager()->LoadShaderProgram("Resources/Shaders/System/texturevert.glsl","Resources/Shaders/greyscale.frag");

    m_iUnitNumber = 0;
    
    m_pDestination = new Vector();

	m_pkTarget = NULL;
    
    m_iSpeed = 80;

	m_bIsSelected = false;

	// Init held resources
	m_stCurrentResources.iWood = 0;

	SetScale(0.4f);
	SetSize(GetSize() * GetScale());

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

    Actor::Update(a_fDeltaTime);

    Vector TempVector;

    TempVector = (*GetLocation() - *m_pDestination);

	TempVector = TempVector.Unitise();

    TempVector *= m_iSpeed;

    if(GetLocation()->CheckDistance(*m_pDestination) > 10.0f)
    {
        SetVelocity(TempVector);
        switch(m_iCurrentTask)
		{
            case eGathering:
                break;
            case eDelivering:
                break;
            case eBuilding:
                break;
            default:
                break;
		}
    }
    else
    {
		switch(m_iCurrentTask)
		{
		case eGathering:
			SetVelocity(0,0,0);
			((Resource*)m_pkTarget)->Harvest();
			break;
		case eDelivering:
			SetVelocity(0,0,0);
			DeliverResources();
            m_iCurrentTask = eIdle;
            break;
        case eBuilding:
            SetVelocity(0,0,0);
            if(((Building*)m_pkTarget)->GetNeedsResources())
            {
                //If at a destination that isn't our target, it probably means we're at the building that had the resource we need.
                if(m_pDestination->CheckDistance(*m_pkTarget->GetWorldLocation()) > 10.0f)
                {
                    //std::cout<<"At destination that isn't target"<<std::endl;
                    if(FindBuildingWithResource(eWood)->TakeResource(eWood))
                    {
                        m_stCurrentResources.iWood++;
                        SetDestination(*m_pkTarget->GetWorldLocation());
                        //TODO: what happens if no resources?
                    }
                    //Else is waiting
                }
                else //At our Building
                {
                    //std::cout<<"At building that needs resources"<<std::endl;
                    DeliverResources();
                    //Check if it still needs resources
                    if(((Building*)m_pkTarget)->GetNeedsResources())
                    {
                        //std::cout<<"Finding Building With Resource"<<std::endl;
                        FindBuildingWithResource(eWood);
                    }
                }
            }
            if(((Building*)m_pkTarget)->GetIsBuilt())
            {
                //std::cout<<"Building is finished"<<std::endl;
                m_iCurrentTask = eIdle;
                m_pkTarget = NULL;
            }
            break;
		default:
			SetVelocity(0,0,0);
			//m_iCurrentTask = eIdle;
			m_pkTarget = NULL;
			break;
		}
    }
    
    CheckForEmptyJobs();
    
    NetworkUpdateLocationAndDestination();

    return true;
}

void Unit::CheckForEmptyJobs()// Used to encapsulate all the job searching
{
    CheckForEmptyBuilding();
    CheckForEmptyResource();
}

void Unit::CheckForEmptyBuilding() // "Empty" as in no worker or no resources
{
    //Check to see if any buildings need help being constructed first.
    if(SceneManager::GetUnitManager()->GetBuildingList().size() > 0 && m_iCurrentTask == eIdle)
    {
        m_iCurrentTask = eBuilding;
        int iTempIndex = -1;
        
        for(unsigned int iDx = 0; iDx < SceneManager::GetUnitManager()->GetBuildingList().size(); iDx++)
        {
            if(SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetHasWorkerAssigned() || SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetIsBuilt())
            {
                continue;
            }
            else
            {
                iTempIndex = iDx;
            }
        }
        
        if (iTempIndex == -1)
        {
            m_iCurrentTask = eIdle;
        }
        else
        {
            Vector vDestination = *SceneManager::GetUnitManager()->GetBuildingList()[iTempIndex]->GetWorldLocation();
            
			if(SceneManager::GetUnitManager()->GetBuildingList()[iTempIndex]->SetHasWorkerAssigned(this))
			{
				m_pkTarget = SceneManager::GetUnitManager()->GetBuildingList()[iTempIndex];
                
				SetDestination(vDestination);
                
                if(((Building*)m_pkTarget)->GetNeedsResources())
                {
                    FindBuildingWithResource(eWood);
                }
			}
			else
			{
				m_iCurrentTask = eIdle;
			}
        }
    }
}

Building* Unit::FindBuildingWithResource(int a_iResourceType) // Used to ferry resources from a stockpile to a building project/work building, sets destination as new location
{
    m_iCurrentTask = eBuilding;
    int iTempIndex = -1;
    
    for(unsigned int iDx = 0; iDx < SceneManager::GetUnitManager()->GetBuildingList().size(); iDx++)
    {
        if(SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetBuildingType() == eGatheringHall)
        {
            iTempIndex = iDx;
        }
        else
        {
            continue;
        }
    }
    
    if(iTempIndex == -1)
    {
        std::cout<<"No buildings with required resource"<<std::endl;
        return NULL;
    }

    Vector vDestination = *SceneManager::GetUnitManager()->GetBuildingList()[iTempIndex]->GetWorldLocation();
    
    SetDestination(vDestination);
    
    return SceneManager::GetUnitManager()->GetBuildingList()[iTempIndex];
}

void Unit::CheckForEmptyResource()
{
    // See if there are any resources to gather
	
	if(SceneManager::GetTileManager()->GetResourceList().size() > 0 && m_iCurrentTask == eIdle)
	{
		m_iCurrentTask = eGathering;
		
		int iTempIndex = -1;
        
		for(unsigned int iDx = 0; iDx < SceneManager::GetTileManager()->GetResourceList().size(); iDx++)
		{
			if(SceneManager::GetTileManager()->GetResourceList()[iDx]->GetHasWorkerAssigned())
			{
				continue;
			}
			else
			{
				iTempIndex = iDx;
			}
		}
        
		if(iTempIndex != -1)
		{
			iTempIndex = 0;
            
			for(unsigned int iDx = 0; iDx < SceneManager::GetTileManager()->GetResourceList().size(); iDx++)
			{
				if(SceneManager::GetTileManager()->GetResourceList()[iDx]->GetHasWorkerAssigned())
				{
					continue;
				}
				if(SceneManager::GetTileManager()->GetResourceList()[iDx]->GetWorldLocation()->CheckDistance(*GetWorldLocation()) < SceneManager::GetTileManager()->GetResourceList()[iTempIndex]->GetWorldLocation()->CheckDistance(*GetWorldLocation()))
				{
					iTempIndex = iDx;
				}
			}
            
            
			Vector vDestination = *SceneManager::GetTileManager()->GetResourceList()[iTempIndex]->GetWorldLocation();
            
			if(SceneManager::GetTileManager()->GetResourceList()[iTempIndex]->SetHasWorkerAssigned(this))
			{
				m_pkTarget = SceneManager::GetTileManager()->GetResourceList()[iTempIndex];
                
                
				SetDestination(vDestination);
			}
			else
			{
				m_iCurrentTask = eIdle;
			}
		}
		else
		{
			m_iCurrentTask = eIdle;
		}
	}
}

void Unit::SetSelected(bool a_bIsSelected)
{
	//std::cout<<"Unit Selection Status Changed To: "<<m_bIsSelected<<" Pointer: "<<this<<std::endl;
	if(a_bIsSelected == false)
	{
		//Change shader back to default
		m_uiShaderNumber = SceneManager::GetDisplayManager()->GetDefaultShader();
	}
	else
	{
		//Change Shader to black and white
		m_uiShaderNumber = m_uiSelectedShaderNumber;
		SceneManager::GetSoundManager()->PlaySound("Resources/Sound/Grunt.wav");
	}
	m_bIsSelected = a_bIsSelected;
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

int Unit::GetCurrentTask()
{
	return m_iCurrentTask;
}

std::string Unit::GetCurrentTaskString()
{
	switch(m_iCurrentTask)
	{
		case eIdle:
			return "Idle";
			break;
		case eGathering:
			return "Gathering";
			break;
		case eDelivering:
			return "Delivering";
			break;
		case eBuilding:
			return "Building";
			break;
		default:
			return "Generic Unit";
			break;
	}
}

stStoredResources Unit::GetCurrentResources()
{
	return m_stCurrentResources;
}

bool Unit::HasResources()
{
	if(m_stCurrentResources.iWood > 0)
	{
		return true;
	}

	return false;
}

void Unit::HarvestComplete(Resource* a_pkResource)
{
	switch(a_pkResource->GetResourceType())
	{
		case eWood:
			m_stCurrentResources.iWood += a_pkResource->GetResourceAmount();
			m_iCurrentTask = eIdle;
			break;
		default:
			break;
	}
	
	//Time to head back to closest store house
	if(SceneManager::GetUnitManager()->GetBuildingList().size() > 0 && m_iCurrentTask == eIdle)
	{
		m_iCurrentTask = eDelivering;

		int iTempIndex = 0;

		for(unsigned int iDx = 0; iDx < SceneManager::GetUnitManager()->GetBuildingList().size(); iDx++)
		{
			if(SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetBuildingType() == eGatheringHall) // Will always go to the first gathering hall.
			{
				iTempIndex = iDx;
                break;
			}
		}

		m_pkTarget = SceneManager::GetUnitManager()->GetBuildingList()[iTempIndex];
		Vector vDestination = *SceneManager::GetUnitManager()->GetBuildingList()[iTempIndex]->GetWorldLocation();
        
        SetDestination(vDestination);
	}
}

void Unit::DeliverResources()
{
	if(m_pkTarget == NULL) // Time to assume target is a building, lol crashes
	{
		return;
	}

	((Building*)m_pkTarget)->m_stCurrentResources.iWood += m_stCurrentResources.iWood;
    
    m_stCurrentResources.iWood = 0;

	return;
}

std::string Unit::GetName()
{
	return m_svName;
}