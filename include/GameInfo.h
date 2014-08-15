#ifndef _GAMEINFO_H_
#define _GAMEINFO_H_
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

class Scene;

class GameInfo
{
public:
    GameInfo(Scene* a_pkScene);
    ~GameInfo();

    bool Update(float a_fDeltaTime);

	int GetCurrentPopulation();
	int GetMaxPopulation();

    int GetScore();
    void ChangeScore(int a_iScore);

private:
    int m_iScore;
    float m_fVillagerSpawnCooldown;
};

#endif
