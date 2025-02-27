#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
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
#include <stack>

#include "SettingsManager.h"
#ifdef _WIN32
#include "D3D11DisplayManager.h"
#endif //_WIN32
#include "OpenGL1DisplayManager.h"
#include "VulkanDisplayManager.h"
#include "InputManager.h"
#include "InputManagerSDL.h"
#ifdef __UWP__
#include "InputManagerUWP.h"
#endif //#ifdef __UWP__
#include "ObjectManager.h"
#include "SoundManager.h"
#include "NetworkManager.h"
#include "UnitManager.h"
#include "TileManager.h"
#include "ParticleManager.h"

//Forward declare Scene class.
class Scene;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

	static void InitialiseSceneManager(int argc, char **argv);
	static void CleanupSceneManager();

    static void AddNewScene(Scene* a_pkNewScene);
    static void ProcessAddNewScene();
    static void RemoveCurrentScene();

    static bool Update(float a_fDeltaTime);

    static DisplayManager* GetDisplayManager();
	static InputManager* GetInputManager();
	static ObjectManager* GetObjectManager();
	static SoundManager* GetSoundManager();
	static NetworkManager* GetNetworkManager();
    static UnitManager* GetUnitManager();
    static TileManager* GetTileManager();
	static ParticleManager* GetParticleManager();
    static SettingsManager* GetSettingsManager();
    
    static Scene* GetCurrentScene();
	
	static int m_argc;
	static char **m_argv;

private:
    static std::stack<Scene*> m_stScenes;

    static Scene* m_pkTempScene;

    static DisplayManager* m_pkDisplayManager;
	static InputManager* m_pkInputManager;
	static ObjectManager* m_pkObjectManager;
	static SoundManager* m_pkSoundManager;
	static NetworkManager* m_pkNetworkManager;
	static UnitManager* m_pkUnitManager;
    static TileManager* m_pkTileManager;
	static ParticleManager* m_pkParticleManager;
    static SettingsManager* m_pkSettingsManager;
};

#endif
