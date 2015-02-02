#ifndef _PLAYER_H_
#define _PLAYER_H_
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
class Vector;

#include "Actor.h"

class Player : public Actor
{
public:
	Player(Scene* a_pkScene);
	~Player();

	bool Update(float a_fDeltaTime);

private:
	int m_iCurrentSpeed;
	int m_iMaxSpeed;
	int m_iAcceleration;
};

#endif