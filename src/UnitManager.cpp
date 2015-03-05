#include "UnitManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "Unit.h"
#include "Building.h"
#include "Player.h"
#include "Vector.h"
#include "Tile.h"


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
        for(int iDx = 0; iDx < 2; iDx++)
        {
            SpawnNewUnit();
        }
    }
}

//Handles all input related stuff when it comes to placing buildings and selecting units.

// Also handles the updating of all the units and buildings.
bool UnitManager::Update(float a_fDeltaTime)
{
	//Update all the units
	SortUnitByY(); // will probably remove this
	for( unsigned int iDx = 0; iDx < m_apkUnits.size(); iDx++ )
    {
        if(!m_apkUnits[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkUnits[iDx];
            m_apkUnits.erase(m_apkUnits.begin() + iDx);
            iDx--;
        }
	}

	//Check for lack of players
	if((int)m_apkPlayers.size() < SceneManager::GetInputManager()->GetNumConnectedControllers())
	{
		SpawnPlayer();
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

	return true;
}

int UnitManager::SpawnPlayer()
{
	m_apkPlayers.push_back(new Player(SceneManager::GetCurrentScene()));

	return 0;
}

int UnitManager::SpawnNewUnit(int a_iType)
{
    m_apkUnits.push_back(new Unit(SceneManager::GetCurrentScene()));
    
    m_apkUnits[m_apkUnits.size() - 1]->SetUnitNumber((int)m_apkUnits.size() - 1);
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkUnits[m_apkUnits.size() - 1]->SetHardLocation(Vector(-640,-360,0));
    
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

    //Resorts the list of units so that units that have a lower Y are drawn last. (isometric drawing thingo)
//TODO: get rid of this, will not scale nicely and it'll be fully topdown anyway
void UnitManager::SortUnitByY()
{
	//Selection Sort
	unsigned int uiLowestUnit = 0; //assume first element is the smallest
	Unit* pTempUnit = NULL;

	for(unsigned int iDx = 0; iDx < m_apkUnits.size(); iDx++)
	{
		uiLowestUnit = iDx;

		for(unsigned int iDy = iDx + 1; iDy < m_apkUnits.size(); iDy++)
		{
			if(m_apkUnits[iDy]->GetWorldLocation()->y < m_apkUnits[uiLowestUnit]->GetWorldLocation()->y)
			{
				//Set lowest
				uiLowestUnit = iDy;
			}
		}

		if(uiLowestUnit != iDx)
		{
			//Swap
			pTempUnit = m_apkUnits[iDx];
			m_apkUnits[iDx] = m_apkUnits[uiLowestUnit];
			m_apkUnits[uiLowestUnit] = pTempUnit;
		}
	}
}

std::vector<Unit*> UnitManager::GetUnitList()
{
	return m_apkUnits;
}

std::vector<Actor*> UnitManager::GetActorList() // checks to see if actor list is the same size as the other lists combined
{
    //if size isn't the same as the other two then rebuild list before returning it.
    if(m_apkActors.size() != m_apkPlayers.size() + m_apkUnits.size())
    {
        while(m_apkActors.size() > 0)
        {
            m_apkActors.pop_back();
        }
        
        for(unsigned int uiDx = 0; uiDx < m_apkPlayers.size(); uiDx++)
        {
            m_apkActors.push_back(m_apkPlayers[uiDx]);
        }
        
        for(unsigned int uiDx = 0; uiDx < m_apkUnits.size(); uiDx++)
        {
            m_apkActors.push_back(m_apkUnits[uiDx]);
        }
    }
    
    return m_apkActors;
}
