#ifndef _UNIT_H_
#define _UNIT_H_
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

//Forward declare
class Scene;
class Vector;
class Building;

enum eUnitType
{
	eCitizen = 0
};

enum eCurrentTask
{
	eIdle = 0,
	eGathering,
	eDelivering,
	eBuilding
};

#include "Actor.h"

class Unit : public Actor
{
public:
	Unit(Scene* a_pkScene);
	~Unit();
    
    void SetUnitNumber(int a_iNewNumber);

	bool Update(float a_fDeltaTime);
    
    void CheckForEmptyJobs();
    
    void CheckForEmptyBuilding();
    
    Building* FindBuildingWithResource(int a_iResourceType);
    
    void CheckForEmptyResource();

	void SetSelected(bool a_bIsSelected = true);

	void SetDestination(Vector a_vNewDestination, bool a_bReplicate = false);
    
    void SetLocation(Vector a_vNewLocation, bool a_bReplicate = false);

	void SetHardLocation(double a_x, double a_y, double a_z, bool a_bReplicate = false);
    void SetHardLocation(Vector a_vNewDestination, bool a_bReplicate = false);
    
    void NetworkUpdateLocationAndDestination();
    
    int m_iUnitNumber; //Used to self reference in network packets and the like

	int GetUnitType();
	std::string GetUnitTypeString();

	int GetCurrentTask();
	std::string GetCurrentTaskString();

	stStoredResources GetCurrentResources();
	bool HasResources();

	void HarvestComplete(Resource* a_pkResource);

	void DeliverResources(); //Delivers resources into m_pkTarget (it should always be a building according to the state machine)

private:
	int m_iUnitType; //Holds the unit type

	int m_iCurrentTask; //Holds the enum of the current task
    
	bool m_bIsSelected;

	Vector* m_pDestination;

	Object* m_pkTarget;

	int m_iSpeed;

	int m_uiSelectedShaderNumber;

	stStoredResources m_stCurrentResources;
};

#endif
