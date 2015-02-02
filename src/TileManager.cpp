#include "TileManager.h"
#include "Tile.h"
#include "SceneManager.h"
#include "Resource.h"

TileManager::TileManager()
{
    //SpawnTile(Vector(0,0,0));

    m_iTileListWidth = 7;
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
    //Modulus is in here in order to "centre" odd number map sizes.
	/*for(int iDx = m_iTileListWidth * -1 + m_iTileListWidth / 2 + m_iTileListWidth % 2; iDx < m_iTileListWidth / 2 + m_iTileListWidth % 2; iDx++)
	{
		for(int iDy = m_iTileListWidth * -1 + m_iTileListWidth / 2 + m_iTileListWidth % 2; iDy < m_iTileListWidth / 2 + m_iTileListWidth % 2; iDy++)
		{
			SpawnTile(Vector(iDx * 256,iDy * 256,0));

            GetTileList()[GetTileList().size() -1]->SetCoordinate(Vector(iDx, iDy, 0));
		}
	}*/

	for(int iDx = m_iTileListWidth * -1 + m_iTileListWidth / 2 + m_iTileListWidth % 2; iDx < m_iTileListWidth / 2 + m_iTileListWidth % 2; iDx++)
	{
			SpawnTile(Vector(iDx * 64, 0 * 64,0));

            GetTileList()[GetTileList().size() -1]->SetCoordinate(Vector(iDx, 0, 0));
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

void TileManager::SetHighlightsForBuildingFootprint(Vector a_vPosition, Vector a_vSize)
{
    //std::cout<<"Tile Position: "<<a_vPosition<<std::endl;
    
    for(unsigned long long iDx = a_vPosition.x; iDx < a_vPosition.x + a_vSize.x;iDx++)
    {
        for(unsigned long long iDy = a_vPosition.y; iDy < a_vPosition.y + a_vSize.y; iDy++)
        {
    if(GetTileAt(Vector(a_vPosition.x, a_vPosition.y, 0))->GetIsOccupied())
    {
        GetTileAt(Vector(a_vPosition.x, a_vPosition.y, 0))->SetRedZone();
    }
    else
    {
        GetTileAt(Vector(a_vPosition.x, a_vPosition.y, 0))->SetGreenZone();
    }
        }
    }
}

bool TileManager::GetIsOccupied(Vector a_vPosition, Vector a_vSize)
{
    for(unsigned long long iDx = a_vPosition.x; iDx < a_vPosition.x + a_vSize.x;iDx++)
    {
        for(unsigned long long iDy = a_vPosition.y; iDy < a_vPosition.y + a_vSize.y; iDy++)
        {
            if(GetTileAt(Vector(iDx, iDy, 0))->GetIsOccupied())
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<Tile*> TileManager::GetTileList()
{
    return m_apkTiles;
}

std::vector<Resource*> TileManager::GetResourceList()
{
	return m_apkResources;
}

Tile* TileManager::GetTileAt(Vector a_vCoordinates)
{
    a_vCoordinates.x += m_iTileListWidth / 2;
    a_vCoordinates.y += m_iTileListWidth / 2;
    return GetTileList()[((int)a_vCoordinates.x * m_iTileListWidth) + (int)a_vCoordinates.y];
}