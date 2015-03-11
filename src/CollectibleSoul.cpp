#include "CollectibleSoul.h"
#include "Actor.h"
#include "AnimatedTexture.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Mesh.h"
#include "Object.h"

CollectibleSoul::CollectibleSoul(Scene* a_pkScene) : Actor(a_pkScene)
{
    //std::cout<<"CollectibleSoul created. Pointer: "<<this<<std::endl;

	//Set a texture; //TODO: Get a real texture for this.
	m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/RedZone.png", SceneManager::GetDisplayManager());

	m_pVelocity->y = -500.0;

	m_iSoulLevelContained = 5;

	m_bHasBeenCollected = false;

	m_iObjectType = eSoul;

	SetScale(0.4f);
	SetSize(GetSize() * GetScale());

	m_bIsGravityOn = true;
}

CollectibleSoul::~CollectibleSoul()
{
    //std::cout<<"Particle Destroyed. Pointer: "<<this<<std::endl;
}

bool CollectibleSoul::Update(float a_fDeltaTime)
{
#ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Particle Tick: "<<this<<std::endl;
#endif

	if(!Actor::Update(a_fDeltaTime) || m_bHasBeenCollected )
	{
		return false;
	}

    return true;
}

int CollectibleSoul::GetSoulLevelContained()
{
	return m_iSoulLevelContained;
}

void CollectibleSoul::SetSoulCollected()
{
	m_bHasBeenCollected = true;
}