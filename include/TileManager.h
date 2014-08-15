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
    void SpawnTile(Vector a_vDestination);
	void SpawnResource(Vector a_vDestination);

	std::vector<Resource*> GetResourceList();
private:
    std::vector<Tile*> m_apkTiles;
	std::vector<Resource*> m_apkResources;
};

#endif
