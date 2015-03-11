#ifndef _COLLECTIBLESOUL_H_
#define _COLLECTIBLESOUL_H_
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
class TextLibrary;

//Floating Text for damage and the like

#include "Actor.h"

class CollectibleSoul : public Actor
{
public:
    CollectibleSoul(Scene* a_pkScene);
    virtual ~CollectibleSoul();

    virtual bool Update(float a_fDeltaTime);

	int GetSoulLevelContained();

	void SetSoulCollected();

private:
    Scene* m_pkScene;
    int m_iSoulLevelContained;
	bool m_bHasBeenCollected;
};

#endif
