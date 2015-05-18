#include "TileManager.h"
#include "Tile.h"
#include "SceneManager.h"
#include "Resource.h"
#include "tinyxml2.h"

TileManager::TileManager()
{
    //SpawnTile(Vector(0,0,0));

    m_iTileListWidth = 25;
	GenerateMap(0);
	//GenerateFromXML("lvl1.xml");
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
	//TODO: Allow maps to be loaded.

	//Load a map from either the stack or a file or something and blot it out.

	//Handle something for the background. // Do this first so it draws underneath everything

	for(int iDx = -24; iDx <= 24; iDx++)
	{
		for(int iDy = -7; iDy <= 7; iDy++)
		{
			SpawnTileAt(Vector(iDx, iDy, 0), eBackground);
		}
	}

	//Generate all the floors

	for(int iDx = -24; iDx <= 24; iDx++)
	{
		SpawnTileAt(Vector(iDx, 8, 0), eFloor2);
	}

	for(int iDx = -24; iDx <= -16; iDx++)
	{
		SpawnTileAt(Vector(iDx, 7, 0), eFloor2);
	}

	for(int iDx = -5; iDx <= 16; iDx++)
	{
		SpawnTileAt(Vector(iDx, 7, 0), eFloor2);
	}

	for(int iDx = -3; iDx <= 14; iDx++)
	{
		SpawnTileAt(Vector(iDx, 6, 0), eFloor2);
	}

	for(int iDx = -20; iDx <= -4; iDx++)
	{
		SpawnTileAt(Vector(iDx, 3, 0), eFloor);
	}

	for(int iDx = 14; iDx <= 23; iDx++)
	{
		SpawnTileAt(Vector(iDx, 3, 0), eFloor);
	}

	for(int iDx = -3; iDx <= 13; iDx++)
	{
		SpawnTileAt(Vector(iDx, 0, 0), eFloor);
	}

	for(int iDx = -20; iDx <= -4; iDx++)
	{
		SpawnTileAt(Vector(iDx, -3, 0), eFloor);
	}

	for(int iDx = -24; iDx <= 24; iDx++)
	{
		SpawnTileAt(Vector(iDx, -8, 0), eFloor2);
	}

	//Generate all the walls

	for(int iDy = -7; iDy <= 6; iDy++)
	{
		SpawnTileAt(Vector(-24, iDy, 0), eWall);
	}

	for(int iDy = -7; iDy <= 7; iDy++)
	{
		SpawnTileAt(Vector(24, iDy, 0), eWall);
	}

    return;
}

int TileManager::GenerateFromXML(std::string a_sFilename)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(a_sFilename.c_str());

	int error = doc.ErrorID();
	if (error) //file didnt load
	{
		std::cout << "Error " << error << std::endl;
		return error;
	}

	int iTileID = 0;

	tinyxml2::XMLElement* level = doc.FirstChildElement("level");


	//Load background
	iTileID = GetTileType(level->FirstChildElement("background")->GetText());

	for (int iDx = -24; iDx <= 24; iDx++)
	{
		for (int iDy = -7; iDy <= 7; iDy++)
		{
			SpawnTileAt(Vector(iDx, iDy, 0), iTileID);
		}
	}

	//Load tiles
	tinyxml2::XMLElement* tile = level->FirstChildElement("tile");
	tinyxml2::XMLElement* pos = 0;

	int iDx = 0, iDy = 0, iDz = 0;

	while (tile)
	{
		// tile type
		iTileID = GetTileType(tile->FirstChildElement("type")->GetText());

		// load tile locations
		pos = tile->FirstChildElement("locations")->FirstChildElement("pos");
		while (pos)
		{
			pos->QueryAttribute("x", &iDx);
			pos->QueryAttribute("y", &iDy);
			pos->QueryAttribute("z", &iDz);
			SpawnTileAt(Vector(iDx, iDy, iDz), iTileID);
			pos = pos->NextSiblingElement("pos"); // next pos
		}

		tile = tile->NextSiblingElement("tile"); // next tile
	}


}

int TileManager::GetTileType(std::string a_sTileID)
{
	if (a_sTileID == "background")
		return eBackground;
	if (a_sTileID == "floor")
		return eFloor;
	if (a_sTileID == "floor2")
		return eFloor2;
	if (a_sTileID == "wall")
		return eWall;

	return -1; //file is broke
}

void TileManager::SpawnTileAt(Vector a_vDestination, int a_iTileType)//Spawn at adjusted coordinates (Adjusted being *64'd)
{
    //TODO: Everything

	Vector vTemp;

	vTemp = a_vDestination;

	vTemp *= 64;
    
    m_apkTiles.push_back(new Tile(SceneManager::GetCurrentScene()));
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkTiles[m_apkTiles.size() - 1]->SetLocation(vTemp);

	m_apkTiles[m_apkTiles.size() - 1]->ChangeTileType(a_iTileType);

    return;
}

void TileManager::SpawnTile(Vector a_vDestination, int a_iTileType)
{
    //TODO: Everything
    
    m_apkTiles.push_back(new Tile(SceneManager::GetCurrentScene()));
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkTiles[m_apkTiles.size() - 1]->SetLocation(a_vDestination);

	m_apkTiles[m_apkTiles.size() - 1]->ChangeTileType(a_iTileType);

    return;
}

std::vector<Tile*>& TileManager::GetTileList()
{
    return m_apkTiles;
}

Tile* TileManager::GetTileAt(Vector a_vCoordinates)
{
    a_vCoordinates.x += m_iTileListWidth / 2;
    a_vCoordinates.y += m_iTileListWidth / 2;
    return GetTileList()[((int)a_vCoordinates.x * m_iTileListWidth) + (int)a_vCoordinates.y];
}