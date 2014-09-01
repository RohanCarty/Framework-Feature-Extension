#include "TileManager.h"
#include "Tile.h"
#include "SceneManager.h"
#include "Resource.h"

TileManager::TileManager()
{
    //SpawnTile(Vector(0,0,0));
	GenerateMap(0);
}

TileManager::~TileManager()
{
}

bool TileManager::Update(float a_fDeltaTime)
{
    //TODO: Everything
    //Update all the tiles
	for( unsigned int iDx = 0; iDx < m_apkTiles.size(); iDx++ )
    {
        if(!m_apkTiles[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkTiles[iDx];
            m_apkTiles.erase(m_apkTiles.begin() + iDx);
            iDx--;
        }
	}

	//Update all the resources
	for( unsigned int iDx = 0; iDx < m_apkResources.size(); iDx++ )
    {
        if(!m_apkResources[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkResources[iDx];
            m_apkResources.erase(m_apkResources.begin() + iDx);
            iDx--;
        }
	}
    return true;
}

void TileManager::GenerateMap(int a_iSeed)
{
    //TODO: Everything
	for(int iDx = -16; iDx < 16; iDx++)
	{
		for(int iDy = -16; iDy < 16; iDy++)
		{
			SpawnTile(Vector(iDx * 256,iDy * 256,0));
		}
	}

	for(int iDx = -1; iDx <= 1; iDx++)
	{
		for(int iDy = -1; iDy <= 1; iDy++)
		{
			if(iDx == 0 && iDy == 0)
			{
				continue;
			}
			SpawnResource(Vector(512 * iDx, 512 * iDy, 0));
		}
	}

    return;
}

void TileManager::SpawnTile(Vector a_vDestination)
{
    //TODO: Everything
    
    m_apkTiles.push_back(new Tile(SceneManager::GetCurrentScene()));
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkTiles[m_apkTiles.size() - 1]->SetLocation(a_vDestination);

	m_apkTiles[m_apkTiles.size() - 1]->SetRotation((rand() % 3 + 1) * 3.1415926535f / 2.0f);

    return;
}

void TileManager::SpawnResource(Vector a_vDestination)
{
	m_apkResources.push_back(new Resource(SceneManager::GetCurrentScene()));

	m_apkResources[m_apkResources.size() - 1]->SetLocation(a_vDestination);
	m_apkResources[m_apkResources.size() - 1]->SetResourceType(eWood);

	return;
}

std::vector<Tile*> TileManager::GetTileList()
{
    return m_apkTiles;
}

std::vector<Resource*> TileManager::GetResourceList()
{
	return m_apkResources;
}