#ifndef _TILEMANAGER_H_
#define _TILEMANAGER_H_
//Dem debug things
#ifdef _WIN32
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <malloc.h>
		#include <crtdbg.h>
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
	   #endif
	#endif
#endif

//Tile Manager deals with generating/decoding the level and spawning the tiles and resources

#include <vector>

#include "Vector.h"
//Forward Declare
class Object;
class Tile;
class Resource;

class TileManager
{
public:
    TileManager();
    virtual ~TileManager();
    
    bool Update(float a_fDeltaTime);
    void GenerateMap(int a_iSeed);
    void SpawnTileAt(Vector a_vDestination, int a_iTileType);
    void SpawnTile(Vector a_vDestination, int a_iTileType);

    std::vector<Tile*>& GetTileList();
    
    Tile* GetTileAt(Vector a_vCoordinates);
private:
    std::vector<Tile*> m_apkTiles;
    
    int m_iTileListWidth; // When getting a tile by X and Y, this is how wide an X row is, (this * Y) + X is the iterator
};

#endif
