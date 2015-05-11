#include "UnitManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameInfo.h"
#include "GameScene.h"
#include "Unit.h"
#include "Building.h"
#include "Player.h"
#include "CollectibleSoul.h"
#include "Vector.h"
#include "Tile.h"
#include "Projectile.h"


UnitManager::UnitManager()
{
	srand(0);
}

UnitManager::~UnitManager()
{
    while(m_apkUnits.size() > 0)
	{
		delete m_apkUnits.back();
		m_apkUnits.pop_back();
	}

	while(m_apkPlayers.size() > 0)
	{
		delete m_apkPlayers.back();
		m_apkPlayers.pop_back();
	}

	while(m_apkProjectiles.size() > 0)
	{
		delete m_apkProjectiles.back();
		m_apkProjectiles.pop_back();
	}
}

//Actually starts the game, in order for us to be able to have a unit manager before networking is started, but we need to make sure we don't make copies on our client
void UnitManager::StartGame()
{
    //Spawn the test units
    if(!SceneManager::GetNetworkManager()->m_bIsClient)
    {
		for(int iDx = 0; iDx < SceneManager::GetInputManager()->GetNumConnectedControllers(); iDx++)
		{
			SpawnPlayer();
		}
    }
}

//Handles the updating of all the actors.
bool UnitManager::Update(float a_fDeltaTime)
{
	//Update all the units
	for( unsigned int iDx = 0; iDx < m_apkUnits.size(); iDx++ )
    {
        if(!m_apkUnits[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkUnits[iDx];
            m_apkUnits.erase(m_apkUnits.begin() + iDx);
            iDx--;

			ForceActorListUpdate(); // force an actor list update every time one gets removed.
        }
	}

	//Limit of units based on the current haunting level.
	if(m_apkUnits.size() < (unsigned int)((GameScene*)SceneManager::GetCurrentScene())->GetGameInfo()->GetHauntingLevel() * 2)
	{
		SpawnNewUnit();
	}

	

	//Update all the players
	for( unsigned int iDx = 0; iDx < m_apkPlayers.size(); iDx++ )
    {
        if(!m_apkPlayers[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkPlayers[iDx];
            m_apkPlayers.erase(m_apkPlayers.begin() + iDx);
            iDx--;
        }
	}

	//Update all the projectiles
	for( unsigned int iDx = 0; iDx < m_apkProjectiles.size(); iDx++ )
    {
        if(!m_apkProjectiles[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkProjectiles[iDx];
            m_apkProjectiles.erase(m_apkProjectiles.begin() + iDx);
            iDx--;
        }
	}

	//Update all the Collectible Souls
	for( unsigned int iDx = 0; iDx < m_apkCollectibleSouls.size(); iDx++ )
    {
        if(!m_apkCollectibleSouls[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkCollectibleSouls[iDx];
            m_apkCollectibleSouls.erase(m_apkCollectibleSouls.begin() + iDx);
            iDx--;
        }
	}

	return true;
}

int UnitManager::SpawnPlayer()
{
	m_apkPlayers.push_back(new Player(SceneManager::GetCurrentScene()));

	m_apkPlayers.back()->SetLocation(0, -192, 0);

	return 0;
}

Projectile* UnitManager::SpawnProjectile()
{
	m_apkProjectiles.push_back(new Projectile(SceneManager::GetCurrentScene()));

	return m_apkProjectiles[m_apkActors.size() - 1];
}

int UnitManager::SpawnNewUnit(int a_iType)
{
    m_apkUnits.push_back(new Unit(SceneManager::GetCurrentScene()));
    
    m_apkUnits[m_apkUnits.size() - 1]->SetUnitNumber((int)m_apkUnits.size() - 1);
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkUnits[m_apkUnits.size() - 1]->SetHardLocation(Vector(rand()%1280 - 640,-360,0));
    
    SpawnNewUnitOverNetwork(m_apkUnits[m_apkUnits.size() - 1]);
    
    return 0;
}

//TODO: Network Function
void UnitManager::SpawnNewUnitOverNetwork(Unit* a_pkUnit)
{
    stCommandPacket stTempCommand;
    
    stTempCommand.m_iOrder = eOrderSpawnUnit;
    stTempCommand.m_iUnit = a_pkUnit->m_iUnitNumber;
    stTempCommand.m_iClient = 0;
    stTempCommand.m_vFirstVector = *a_pkUnit->GetLocation();
    stTempCommand.m_vSecondVector = *a_pkUnit->GetLocation(); // Fill the entire command fuckhead
    SceneManager::GetNetworkManager()->AddCommand(&stTempCommand);
}

int UnitManager::SpawnNewCollectibleSoul(Vector* a_pLocation)
{
	m_apkCollectibleSouls.push_back(new CollectibleSoul(SceneManager::GetCurrentScene()));
        
	m_apkCollectibleSouls[m_apkCollectibleSouls.size() - 1]->SetLocation(*a_pLocation);
    
    return 0;
}

std::vector<Unit*>& UnitManager::GetUnitList()
{
	return m_apkUnits;
}

std::vector<Player*>& UnitManager::GetPlayerList()
{
	return m_apkPlayers;
}

std::vector<Projectile*>& UnitManager::GetProjectileList()
{
	return m_apkProjectiles;
}

std::vector<Actor*>& UnitManager::GetActorList() // checks to see if actor list is the same size as the other lists combined
{
    //if size isn't the same as the other two then rebuild list before returning it.
	if(m_apkActors.size() != m_apkPlayers.size() + m_apkUnits.size() + m_apkCollectibleSouls.size())
    {
        ForceActorListUpdate();
    }
    
    return m_apkActors;
}

void UnitManager::ForceActorListUpdate()
{
	while(m_apkActors.size() > 0)
	{
		m_apkActors.pop_back();
	}

	for(unsigned int uiDx = 0; uiDx < m_apkPlayers.size(); uiDx++)
	{
		m_apkActors.push_back(m_apkPlayers[uiDx]);
	}
	
	for(unsigned int uiDx = 0; uiDx < m_apkProjectiles.size(); uiDx++)
	{
		m_apkActors.push_back(m_apkProjectiles[uiDx]);
	}

	for(unsigned int uiDx = 0; uiDx < m_apkUnits.size(); uiDx++)
	{
		m_apkActors.push_back(m_apkUnits[uiDx]);
	}

	for(unsigned int uiDx = 0; uiDx < m_apkCollectibleSouls.size(); uiDx++)
	{
		m_apkActors.push_back(m_apkCollectibleSouls[uiDx]);
	}
}