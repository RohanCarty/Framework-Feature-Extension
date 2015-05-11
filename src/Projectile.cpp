#include "Projectile.h"
#include "Particle.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "HUD.h"
#include "Vector.h"
#include "InputManager.h"
#include "AnimatedTexture.h"

Projectile::Projectile(Scene* a_pkScene) : Actor(a_pkScene)
{
    //std::cout<<"Unit created. Pointer: "<<this<<std::endl;

	//Test sprite for now, will load actual one once we have it.
	m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/Burst.png", SceneManager::GetDisplayManager());

	//Set some defaults and initialise some variables.

	m_iAttackPower = 50;

	m_bIsGravityOn = false;

	SetScale(0.5f);
	SetSize(GetSize() * GetScale());
    
    m_iObjectType = eProjectile;

	m_bInvincible = false;
}

Projectile::~Projectile()
{
    std::cout<<"Player Destroyed. Pointer: "<<this<<std::endl;
}

bool Projectile::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Actor Tick: "<<this<<std::endl;
    #endif

	//SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/playerswing.ogg");

	Actor::Update(a_fDeltaTime);

	//Create a temp vector, this will be modified based on direction then added to the current location to get the attack point
	Vector vTemp;

	vTemp = *GetLocation() + (*m_pVelocity * a_fDeltaTime);
    
	//Ensure checking collision like this doesn't cause any issues.

	//Clear collision list
	while(m_apkIsCollidingWithNextFame.size() > 0)
	{
		m_apkIsCollidingWithNextFame.pop_back();
	}

	if(IsCollidingWithActorNextFrame(a_fDeltaTime, &vTemp))
	{
		for(unsigned int uiDx = 0; uiDx < m_apkIsCollidingWithNextFame.size(); uiDx++)
		{
			if(m_apkIsCollidingWithNextFame[uiDx]->m_iObjectType == eUnit)
			{
				//TODO: Proper damage
				if(!((Actor*)m_apkIsCollidingWithNextFame[uiDx])->GetIsInvincible()) //Not invinciible
				{
					SceneManager::GetSoundManager()->PlaySoundFile("Sounds/SFX/playerhit.ogg");
					((Actor*)m_apkIsCollidingWithNextFame[uiDx])->SetHealth(((Actor*)m_apkIsCollidingWithNextFame[uiDx])->GetHealth() - m_iAttackPower);
					SceneManager::GetParticleManager()->SpawnFloatingText(Vector(GetLocation()->x,GetLocation()->y - 20,0), m_iAttackPower);

					return false; // Kill self on hit.
				}
			}
		}
	}

	if(IsCollidingWithTileNextFrame(a_fDeltaTime, &vTemp))
	{
		return false; //Hit a wall, despawn
	}

	return true;
}