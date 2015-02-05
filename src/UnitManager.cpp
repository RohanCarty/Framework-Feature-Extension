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
        for(int iDx = 0; iDx < 1; iDx++)
        {
            //SpawnNewUnit();
        }
    }
}

//Handles all input related stuff when it comes to placing buildings and selecting units.

// Also handles the updating of all the units and buildings.
bool UnitManager::Update(float a_fDeltaTime)
{
	//Now all the buildings
	for( unsigned int iDx = 0; iDx < m_apkBuildings.size(); iDx++ )
    {
        if(!m_apkBuildings[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkBuildings[iDx];
            m_apkBuildings.erase(m_apkBuildings.begin() + iDx);
            iDx--;
        }
	}

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
    
    m_apkUnits[m_apkUnits.size() - 1]->SetHardLocation(Vector(0,0,0));
    
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

int UnitManager::SpawnNewBuilding(Vector a_vWorldPosition, int a_iBuildingType)
{
    //Need to get the location of the tile it got put on, centre it on that tile and set an occupied flag.
    /*unsigned int uiIndexOfTile = -1;
    
    //TODO: could be optimised with distance checks, but don't worry about it for now.
    for(unsigned int iDx = 0; iDx < SceneManager::GetTileManager()->GetTileList().size(); iDx++)
	{
		if(a_vWorldPosition.WithinBox(*SceneManager::GetTileManager()->GetTileList()[iDx]->GetWorldLocation(), SceneManager::GetTileManager()->GetTileList()[iDx]->GetSize()))
		{
            if(SceneManager::GetTileManager()->GetTileList()[iDx]->GetIsOccupied())
            {
                std::cout<<"Attempted build space is already occupied by"<<((Building*)SceneManager::GetTileManager()->GetTileList()[iDx]->GetIsOccupiedBy())->GetBuildingTypeString()<<", cancelling building"<<std::endl;
                return 0;
            }
			a_vWorldPosition = *SceneManager::GetTileManager()->GetTileList()[iDx]->GetWorldLocation();
            
            uiIndexOfTile = iDx;
            
            break;
		}
	}
    
    m_apkBuildings.push_back(new Building(SceneManager::GetCurrentScene()));
    
    m_apkBuildings[m_apkBuildings.size() - 1]->SetLocation(a_vWorldPosition);

	m_apkBuildings[m_apkBuildings.size() - 1]->SetBuildingType(a_iBuildingType);

	m_apkBuildings[m_apkBuildings.size() - 1]->Update(0.0f);
    
    SceneManager::GetTileManager()->GetTileList()[uiIndexOfTile]->SetIsOccupied(m_apkBuildings[m_apkBuildings.size() - 1]);

	SortBuildingByY();
    
    SpawnNewBuildingOverNetwork(m_apkBuildings[m_apkBuildings.size() - 1]);
    */
    return 0;
}

int UnitManager::SpawnNewBuildingAtTile(Vector a_vTileCoordinates, int a_iBuildingType)
{
    /*if(SceneManager::GetTileManager()->GetTileAt(a_vTileCoordinates)->GetIsOccupied())
    {
        std::cout<<"Attempted build space is already occupied by"<<((Building*)SceneManager::GetTileManager()->GetTileAt(a_vTileCoordinates)->GetIsOccupiedBy())->GetBuildingTypeString()<<", cancelling building"<<std::endl;
        return 0;
    }
    Vector vWorldPosition = *SceneManager::GetTileManager()->GetTileAt(a_vTileCoordinates)->GetWorldLocation();
    
    m_apkBuildings.push_back(new Building(SceneManager::GetCurrentScene()));
    
    m_apkBuildings[m_apkBuildings.size() - 1]->SetLocation(vWorldPosition);
    
	m_apkBuildings[m_apkBuildings.size() - 1]->SetBuildingType(a_iBuildingType);
    
	m_apkBuildings[m_apkBuildings.size() - 1]->Update(0.0f);
    
    SceneManager::GetTileManager()->GetTileAt(a_vTileCoordinates)->SetIsOccupied(m_apkBuildings[m_apkBuildings.size() - 1]);
    
	SortBuildingByY();
    
    SpawnNewBuildingOverNetwork(m_apkBuildings[m_apkBuildings.size() - 1]);
    */
    return 0;
}

//TODO: Network Function
void UnitManager::SpawnNewBuildingOverNetwork(Building* a_pkBuilding)
{
    stCommandPacket stTempCommand;
    
    stTempCommand.m_iOrder = eOrderSpawnBuilding;
    stTempCommand.m_iUnit = a_pkBuilding->m_iUnitNumber;
    stTempCommand.m_iClient = 0;
    stTempCommand.m_vFirstVector = *a_pkBuilding->GetLocation();
    stTempCommand.m_vSecondVector = *a_pkBuilding->GetLocation(); // Fill the entire command fuckhead
    stTempCommand.m_vSecondVector.x = a_pkBuilding->GetBuildingType(); // Set the building Type
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

//Resorts the list of buildings so that buildings that have a lower Y are drawn last. (isometric drawing thingo)
void UnitManager::SortBuildingByY()
{
	//Selection Sort
	unsigned int uiLowestBuilding = 0; //assume first element is the smallest
	Building* pTempBuilding = NULL;

	for(unsigned int iDx = 0; iDx < m_apkBuildings.size(); iDx++)
	{
		uiLowestBuilding = iDx;

		for(unsigned int iDy = iDx + 1; iDy < m_apkBuildings.size(); iDy++)
		{
			if(m_apkBuildings[iDy]->GetWorldLocation()->y < m_apkBuildings[uiLowestBuilding]->GetWorldLocation()->y)
			{
				//Set lowest
				uiLowestBuilding = iDy;
			}
		}

		if(uiLowestBuilding != iDx)
		{
			//Swap
			pTempBuilding = m_apkBuildings[iDx];
			m_apkBuildings[iDx] = m_apkBuildings[uiLowestBuilding];
			m_apkBuildings[uiLowestBuilding] = pTempBuilding;
		}
	}
}

std::vector<Unit*> UnitManager::GetUnitList()
{
	return m_apkUnits;
}

std::vector<Building*> UnitManager::GetBuildingList()
{
	return m_apkBuildings;
}