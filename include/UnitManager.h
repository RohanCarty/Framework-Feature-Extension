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
class CollectibleSoul;
class Scene;
class Projectile;

class UnitManager
{
public:
    UnitManager();
    virtual ~UnitManager();
    
    virtual void StartGame(); //Actually spawns in the units

	virtual bool Update(float a_fDeltaTime);
    
	virtual int SpawnPlayer();

	virtual Projectile* SpawnProjectile(); //Spawns a projectile and returns a pointer to it

    virtual int SpawnNewUnit(int a_iType = 0);
    virtual void SpawnNewUnitOverNetwork(Unit* a_pkUnit);

	virtual int SpawnNewCollectibleSoul(Vector* a_pLocation);
    
	std::vector<Unit*>& GetUnitList();
	std::vector<Player*>& GetPlayerList();
	std::vector<Projectile*>& GetProjectileList();
    std::vector<Actor*>& GetActorList();

	void ForceActorListUpdate();

private:
	std::vector<Player*> m_apkPlayers;
	std::vector<Projectile*> m_apkProjectiles;
	std::vector<Unit*> m_apkUnits;
	std::vector<CollectibleSoul*> m_apkCollectibleSouls;
    std::vector<Actor*> m_apkActors; //combined list of all other types
};

#endif
