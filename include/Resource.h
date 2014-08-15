#ifndef _RESOURCE_H_
#define _RESOURCE_H_
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

enum eResourceType
{
	eWood = 0
};

struct stStoredResources
{
	int iWood;
};

//Forward declare
class Scene;
class Unit;

//Buildings will for now represent houses.

#include "Object.h"

class Resource : public Object
{
public:
    Resource(Scene* a_pkScene);
    virtual ~Resource();
    
    bool Update(float a_fDeltaTime);

	void SetResourceType(int a_iResourceType);
	int GetResourceType();
	std::string GetResourceTypeString();
    
    int GetResourceAmount();

	void Harvest();

	float GetHarvestTime();

	bool GetHasWorkerAssigned();
	bool SetHasWorkerAssigned(Unit* a_pkUnit);
    
    int m_iUnitNumber; //Used to self reference in network packets and the like
private:
	int m_iResourceType;
	int m_iResourceAmount;
	float m_fHarvestTime;
	Unit* m_pkWorker;
	bool m_bIsBeingHarvested;
};

#endif
