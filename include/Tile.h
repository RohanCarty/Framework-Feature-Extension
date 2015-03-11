#ifndef _TILE_H_
#define _TILE_H_
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

#include <iostream>

//Forward declare
class Scene;

//Tiles are the terrain underneath the units,

//enum to avoid string comparisons when processing tiles
enum eTileType
{
	eFloor = 0,
	eFloor2,
	eWall,
	eBackground
};

#include "Object.h"

class Tile : public Object
{
public:
    Tile(Scene* a_pkScene);
    virtual ~Tile();

    bool Update(float a_fDeltaTime);

    void SetCoordinate(Vector a_vCoordinates);
    Vector GetCoordinate();

	bool GetIsCollidable();

	void ChangeTileType(int a_iTileType);
	void RandomiseRotation();
private:
    Vector m_vCoordinates;
	bool m_bIsCollidable;
};

#endif
