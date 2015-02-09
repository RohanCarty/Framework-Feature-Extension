#include "Resource.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Unit.h"

#include "AnimatedTexture.h"

Resource::Resource(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Building created. Pointer: "<<this<<std::endl;
    //m_pkTexture->LoadTexture("resources/Textures/Tree1.png", SceneManager::GetDisplayManager());

    SetScale(2.0f);
	SetSize(GetSize() * GetScale());

	m_iResourceType = eWood;
	m_iResourceAmount = 1; //Perhaps I'll make each thing able to have multiple resources
	m_fHarvestTime = 0.0f; //Time in seconds of how long it takes to harvest this resource
	m_pkWorker = NULL;
	m_bIsBeingHarvested = false;
}

Resource::~Resource()
{
    //std::cout<<"Resource Destroyed. Pointer: "<<this<<std::endl;
}

bool Resource::Update(float a_fDeltaTime)
{
#ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Building Tick: "<<this<<std::endl;
#endif
    
    Object::Update(a_fDeltaTime);

	if(m_bIsBeingHarvested)
	{
		m_fHarvestTime -= a_fDeltaTime;

		if(m_fHarvestTime < 0.0f)
		{
			return false;
		}
	}
    
    return true;
}

void Resource::SetResourceType(int a_iResourceType)
{
	m_iResourceType = a_iResourceType;

	if(m_iResourceType == eWood)
	{
		switch(rand() % 2 + 1)
		{
			case 1:
				m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/Tree1.png", SceneManager::GetDisplayManager());
				break;
			case 2:
				m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/Tree2.png", SceneManager::GetDisplayManager());
				break;
			default:
				break;
		}

		m_fHarvestTime = 1.0f;

		/*SetScale(GetScale() * 2.0f);
		SetSize(GetSize() * GetScale());*/
		
		//m_iResourceAmount = 5;
	}
}

int Resource::GetResourceType()
{
	return m_iResourceType;
}

std::string Resource::GetResourceTypeString()
{
	switch(m_iResourceType)
	{
		case eWood:
			return "Wood";
			break;
		default:
			return "Generic Unit";
			break;
	}
}

int Resource::GetResourceAmount()
{
	return m_iResourceAmount;
}

void Resource::Harvest()
{
	m_bIsBeingHarvested = true;
}

float Resource::GetHarvestTime()
{
	return m_fHarvestTime;
}

bool Resource::GetHasWorkerAssigned()
{
	if(m_pkWorker != NULL)
	{
		return true;
	}
	return false;
}

bool Resource::SetHasWorkerAssigned(Unit* a_pkUnit)
{
	if(m_pkWorker != NULL)
	{
		return false;
	}

	m_pkWorker = a_pkUnit;

	return true;
}