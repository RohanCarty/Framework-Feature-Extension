#include "Building.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Unit.h"

#include "AnimatedTexture.h"

Building::Building(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Building created. Pointer: "<<this<<std::endl;
    //m_pkTexture->LoadTexture("Resources/Textures/GroundTexture.bmp", SceneManager::GetDisplayManager());
    SetScale(2.0f);

	m_iBuildingType = 0;
	m_iMaxPopulation = 0;

	m_stMaxResources.iWood = 0;

	m_stCurrentResources.iWood = 0;
    
    m_fBuildTime = 0.0f;
    
    m_bIsBuilt = false;
    
    m_pkWorker = NULL;
    m_bWorkerPresent = false;
}

Building::~Building()
{
    std::cout<<"Building Destroyed. Pointer: "<<this<<std::endl;
}

bool Building::Update(float a_fDeltaTime)
{
#ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Building Tick: "<<this<<std::endl;
#endif
    
    Object::Update(a_fDeltaTime);
    
    if(!m_bIsBuilt && !m_bWorkerPresent)
    {
        if(m_pkWorker != NULL)
        {
            if(m_pkWorker->GetWorldLocation()->CheckDistance(*GetWorldLocation()) < 10.0f)
            {
                m_bWorkerPresent = true;
            }
        }
    }
    
    if(!m_bIsBuilt && m_bWorkerPresent && !GetNeedsResources())
    {
        if(m_pkWorker != NULL)
        {
            if(m_pkWorker->GetWorldLocation()->CheckDistance(*GetWorldLocation()) > 10.0f)
            {
                m_bWorkerPresent = false;
            }
        }
        m_fBuildTime -= a_fDeltaTime;
        
        if(m_fBuildTime < 0.0f)
        {
            BuildBuilding();
        }
    }
    
    return true;
}

void Building::BuildBuilding()
{
    m_bIsBuilt = true;
    
    //Clear current resources
    m_stCurrentResources.iWood = 0;
    
    switch(m_iBuildingType)
    {
        case eGatheringHall:
            //Gathering Hall auto builds when placed.
            m_stMaxResources.iWood = 30;
            break;
        case eHouse:
        {
            int iRand = rand() % 3 + 1;
            
            //std::cout<<"House"<<std::endl;
            
            m_stMaxResources.iWood = 0;
            
            if(iRand == 1)
            {
                m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/house1.png", SceneManager::GetDisplayManager());
            }
            else if(iRand == 2)
            {
                m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/house2.png", SceneManager::GetDisplayManager());
            }
            else if(iRand == 3)
            {
                m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/house3.png", SceneManager::GetDisplayManager());
            }
            m_iMaxPopulation = 3;
            break;
        }
        default:
            std::cout<<"Default"<<std::endl;
            break;
    }

}

void Building::SetBuildingType(int a_iBuildingType)
{
	m_iBuildingType = a_iBuildingType;

	if(m_iBuildingType == eGatheringHall)
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/gatheringhall.png", SceneManager::GetDisplayManager());

		SetScale(GetScale() * 2.0f);
		SetSize(Vector(GetSize().x * GetScale() / 2, GetSize().y * GetScale(), GetSize().z));
		
		m_iMaxPopulation = 1;

		m_stMaxResources.iWood = 5;
        
        m_fBuildTime = 0.0f;
        m_bIsBuilt = true;
	}

	if(m_iBuildingType == eHouse)
	{
		m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/Construction_Site.png", SceneManager::GetDisplayManager());

        m_fBuildTime = 10.0f;
        
        m_stMaxResources.iWood = 1;
	}
}

int Building::GetBuildingType()
{
	return m_iBuildingType;
}

std::string Building::GetBuildingTypeString()
{
	switch(m_iBuildingType)
	{
		case eGatheringHall:
			return "Gathering Hall";
			break;
		case eHouse:
			return "House";
			break;
		default:
			return "Generic Building";
			break;
	}
}

int Building::GetPopulationCapacity()
{
	return m_iMaxPopulation;
}

int Building::GetResourceAmount(int a_iResourceType)
{
    switch(a_iResourceType)
    {
        case eWood:
            return m_stCurrentResources.iWood;
            break;
        default:
            return 0;
    }
}

bool Building::TakeResource(int a_iResourceType) // returns true if successful
{
    switch(a_iResourceType)
    {
        case eWood:
            if(m_stCurrentResources.iWood > 0)
            {
                m_stCurrentResources.iWood--;
                return true;
            }
            else
                return false;
            break;
        default:
            return false;
    }
}

bool Building::GetHasWorkerAssigned()
{
	if(m_pkWorker != NULL)
	{
		return true;
	}
	return false;
}

bool Building::SetHasWorkerAssigned(Unit* a_pkUnit)
{
    if(m_pkWorker == a_pkUnit)
    {
        return true; // see if already assigned
    }
	if(m_pkWorker != NULL)
	{
        std::cout<<"Building already has worker assigned"<<std::endl;
		return false;
	}
    
	m_pkWorker = a_pkUnit;
    
    std::cout<<"Assigning worker to building: "<<m_pkWorker<<std::endl;
    
	return true;
}

bool Building::GetIsBuilt()
{
    return m_bIsBuilt;
}

bool Building::GetNeedsResources()
{
    if(m_stCurrentResources.iWood < m_stMaxResources.iWood)
    {
        return true;
    }
    
    return false;
}