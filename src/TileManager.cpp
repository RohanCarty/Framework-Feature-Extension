#include "TileManager.h"
#include "Tile.h"
#include "SceneManager.h"
#include "Resource.h"

TileManager::TileManager()
{
    //SpawnTile(Vector(0,0,0));

    m_iTileListWidth = 25;
	GenerateMap(0);
}

TileManager::~TileManager()
{
	while(m_apkTiles.size() > 0)
	{
		delete m_apkTiles[m_apkTiles.size() - 1];
		m_apkTiles.pop_back();
	}
}

bool TileManager::Update(float a_fDeltaTime)
{
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

    return true;
}

void TileManager::GenerateMap(int a_iSeed)
{
	//TODO: Everything

	//Load a map from either the stack or a file or something and blot it out.

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
			SpawnTile(Vector(iDx * 64, 128, 0));

            GetTileList()[GetTileList().size() -1]->SetCoordinate(Vector(iDx, 0, 0));
	}
    SpawnTile(Vector(-768, -128, 0));
    SpawnTile(Vector(-768, -64, 0));
    SpawnTile(Vector(-768, 0, 0));
    SpawnTile(Vector(-768, 64, 0));
    SpawnTile(Vector(768, -128, 0));
    SpawnTile(Vector(768, -64, 0));
    SpawnTile(Vector(768, 0, 0));
    SpawnTile(Vector(768, 64, 0));

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

std::vector<Tile*> TileManager::GetTileList()
{
    return m_apkTiles;
}

Tile* TileManager::GetTileAt(Vector a_vCoordinates)
{
    a_vCoordinates.x += m_iTileListWidth / 2;
    a_vCoordinates.y += m_iTileListWidth / 2;
    return GetTileList()[((int)a_vCoordinates.x * m_iTileListWidth) + (int)a_vCoordinates.y];
}