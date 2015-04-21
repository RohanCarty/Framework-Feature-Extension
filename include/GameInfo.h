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
	
    int GetScore();
    void ChangeScore(int a_iScore);
	
    int GetHauntingLevel();
    void SetHauntingLevel(int a_iHauntingLevel);

    float GetHauntingIncreaseCooldown();
    void SetHauntingIncreaseCooldown(float a_fHauntingIncreaseCooldown);

private:
    int m_iScore;
	int m_iHauntingLevel;
    float m_fHauntingIncreaseCooldown;
};

#endif
