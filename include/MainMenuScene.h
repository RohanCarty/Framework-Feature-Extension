#ifndef _MAINMENUSCENE_H_
#define _MAINMENUSCENE_H_
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
class Camera;
class Cursor;
class Button;

//Forward declare GameSceneManager
class SceneManager;

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
    ~MainMenuScene();

    bool Update(float a_fDeltaTime);

    SceneManager* GetSceneManager();

private:
    SceneManager* m_pkSceneManager;
	Camera* m_pkCamera;
	Cursor* m_pkCursor;

	Button* m_pkNewGameButton;
	Button* m_pkQuitButton;
};

#endif
