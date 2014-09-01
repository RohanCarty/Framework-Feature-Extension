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

#include "Object.h"

class Tile : public Object
{
public:
    Tile(Scene* a_pkScene);
    virtual ~Tile();
    
    bool Update(float a_fDeltaTime);
    
    void SetIsOccupied(Object* a_pkOccupiedBy);
    bool GetIsOccupied();
    
    Object* GetIsOccupiedBy();
    
    void SetCost(float a_fNewCost);
    float GetCost();
    
private:
    float m_fCost; //Cost for pathfinding (make them prefer roads heavily)
    Object* m_pkOccupiedBy;
};

#endif
