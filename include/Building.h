#ifndef _BUILDING_H_
#define _BUILDING_H_
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
#include "Resource.h"

enum eBuildingType
{
	eGatheringHall = 0,
	eHouse,
	eFood,
	eTree
};

//Forward declare
class Scene;
class Unit;

//Buildings will for now represent houses.

#include "Object.h"

class Building : public Object
{
public:
    Building(Scene* a_pkScene);
    virtual ~Building();
    
    bool Update(float a_fDeltaTime);
    
    void BuildBuilding();

	void SetBuildingType(int a_iBuildingType);
	int GetBuildingType();
	std::string GetBuildingTypeString();

	int GetPopulationCapacity();
    
    int m_iUnitNumber; //Used to self reference in network packets and the like

	stStoredResources m_stCurrentResources;
	stStoredResources m_stMaxResources; // Resource requirements before it's built, capacity after it's built
    
    int GetResourceAmount(int a_iResourceType);
    bool TakeResource(int a_iResourceType);
    
    bool GetHasWorkerAssigned();
	bool SetHasWorkerAssigned(Unit* a_pkUnit);
    
    bool GetIsBuilt();
    
    bool GetNeedsResources();
    
private:
	int m_iBuildingType;
	int m_iMaxPopulation;
    float m_fBuildTime;
    bool m_bIsBuilt;
    Unit* m_pkWorker;
    bool m_bWorkerPresent;
};

#endif
