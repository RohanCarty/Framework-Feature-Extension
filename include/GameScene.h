#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_
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

#include "Scene.h"

//Forward declare the Object class so as to use it's pointer.
class Actor;
class Unit;
class HUD;
class GameInfo;
class Camera;
class Cursor;

//Forward declare GameSceneManager
class SceneManager;

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();

    bool Update(float a_fDeltaTime);

	void ReloadAllObjectScripts();

    SceneManager* GetSceneManager();

    GameInfo* GetGameInfo();

//private:
    SceneManager* m_pkSceneManager;
    HUD* m_pkHUD;
	Camera* m_pkCamera;
	Cursor* m_pkCursor;
    GameInfo* m_pkGameInfo;
};

#endif
