#ifndef _SCENE_H_
#define _SCENE_H_
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
#include <vector>

//Forward declare the Object class so as to use it's pointer.
class Actor;
class Cloud;
class HUD;
class GameInfo;

enum eSceneType
{
	eDefaultScene = 0,
	eGameScene,
	eMenuScene
};

//Forward declare SceneManager
class SceneManager;

class Scene
{
public:
    Scene();
    virtual ~Scene();

    virtual bool Update(float a_fDeltaTime);

	virtual int GetSceneType(); //returns the enum of what type of scene this is.

protected:
	int m_iSceneType;
};

#endif
