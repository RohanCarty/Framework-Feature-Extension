#ifndef _UNITMANAGER_H_
#define _UNITMANAGER_H_
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

//Object Manager keeps track of I.D.'s of objects,

#include <vector>
#include "Vector.h"

//Forward Declare
class Building;
class Unit;
class Actor;
class Player;
class Scene;

class UnitManager
{
public:
    UnitManager();
    virtual ~UnitManager();
    
    virtual void StartGame(); //Actually spawns in the units

	virtual bool Update(float a_fDeltaTime);
    
	virtual int SpawnPlayer();

    virtual int SpawnNewUnit(int a_iType = 0);
    virtual void SpawnNewUnitOverNetwork(Unit* a_pkUnit);

	virtual void SortUnitByY();
    
	std::vector<Unit*> GetUnitList();
    std::vector<Actor*> GetActorList();
private:
	std::vector<Unit*> m_apkUnits;
	std::vector<Player*> m_apkPlayers;
    std::vector<Actor*> m_apkActors; //combined list of both other types
};

#endif
