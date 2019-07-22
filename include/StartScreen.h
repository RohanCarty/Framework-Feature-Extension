#ifndef _STARTSCREEN_H_
#define _STARTSCREEN_H_
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

#include "Scene.h"

#include <iostream>
#include <vector>

//Forward declare SceneManager
class SceneManager;

class StartScreen : public Scene
{
public:
    StartScreen();
    ~StartScreen();

    bool Update(float a_fDeltaTime);
};

#endif
