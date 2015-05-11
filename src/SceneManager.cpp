#include "SceneManager.h"
#include "Scene.h"

#include <iostream>
#include "GameScene.h"
#include "MainMenuScene.h"

#ifdef _WIN32
#include "msvcunistd.h"
#else
#include <unistd.h>
#endif
//Declare static variables
char** SceneManager::m_argv;
int SceneManager::m_argc;

std::stack<Scene*> SceneManager::m_stScenes;
Scene* SceneManager::m_pkTempScene;

DisplayManager* SceneManager::m_pkDisplayManager;
InputManager* SceneManager::m_pkInputManager;
ObjectManager* SceneManager::m_pkObjectManager;
SoundManager* SceneManager::m_pkSoundManager;
NetworkManager* SceneManager::m_pkNetworkManager;
UnitManager* SceneManager::m_pkUnitManager;
TileManager* SceneManager::m_pkTileManager;
ParticleManager* SceneManager::m_pkParticleManager;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::InitialiseSceneManager(int argc, char **argv)
{
	std::cout<<"SceneManager created."<<std::endl;

	m_argc = argc;
	m_argv = argv;
    
    //Log Arguments
    std::cout<<"Arguments: ";
    for(int iDx = 0; iDx < argc; iDx++)
    {
        std::cout<<argv[iDx]<<", ";
    }
    std::cout<<std::endl;
    char result[ 1024 ];
    std::cout<<"Current working directory: "<<getcwd( result, sizeof( result ))<<std::endl;
	
	m_pkDisplayManager = NULL;

	//look for opengl1 in the arguments
	for(int iDx = 0; iDx < argc; iDx++)
    {
		if(strcmp(argv[iDx], "opengl") == 0)
		{
			m_pkDisplayManager = new OpenGLDisplayManager(m_argc, m_argv);
		}

		if(strcmp(argv[iDx], "opengl1") == 0)
		{
			m_pkDisplayManager = new OpenGL1DisplayManager(m_argc, m_argv);
		}
#ifdef _WIN32
		if(strcmp(argv[iDx], "d3d11") == 0)
		{
			m_pkDisplayManager = new D3D11DisplayManager(m_argc, m_argv);
		}
#endif
	}

	if(m_pkDisplayManager == NULL)
	{
#ifdef _WIN32
		m_pkDisplayManager = new D3D11DisplayManager(m_argc, m_argv);
#else
		m_pkDisplayManager = new OpenGLDisplayManager(m_argc, m_argv);
#endif //_WIN32
	}
	
	m_pkInputManager = new InputManager;

	m_pkObjectManager = new ObjectManager;

	m_pkSoundManager = new SoundManager;

	m_pkNetworkManager = new NetworkManager;
    
    //AddNewScene(new GameScene());
	AddNewScene(new MainMenuScene());
    
    ProcessAddNewScene();

	//AddNewScene(new GameScene());
    
    //ProcessAddNewScene();
    
	m_pkUnitManager = new UnitManager;
    
    m_pkTileManager = new TileManager;

	m_pkParticleManager = new ParticleManager;

	for(int iDx = 0; iDx < argc; iDx++)
    {
        if(strcmp(argv[iDx], "server") == 0)
		{
			if(argc > iDx)
			{
				std::cout<<"Starting server on "<<argv[iDx + 1]<<std::endl;
				m_pkNetworkManager->StartServer(atoi(argv[iDx + 1]));
			}
		}
		else if(strcmp(argv[iDx], "client") == 0)
		{
			if(argc > iDx + 1)
			{
				std::cout<<"Connecting to client at: "<<argv[iDx + 1]<<" on "<<argv[iDx + 2]<<std::endl;
				m_pkNetworkManager->ConnectToServer(argv[iDx + 1], atoi(argv[iDx + 2]));
			}
		}
		
    }
    
    //Arguments done, time to start the game
//    m_pkUnitManager->StartGame();

	m_pkTempScene = NULL;
}

void SceneManager::CleanupSceneManager()
{
	std::cout<<"SceneManager Destroyed."<<std::endl;

	while(m_stScenes.size() > 0)
	{
		RemoveCurrentScene();
	}

    delete m_pkUnitManager;
    delete m_pkTileManager;
	delete m_pkParticleManager;
	delete m_pkInputManager;
	delete m_pkObjectManager;
	delete m_pkSoundManager;
	delete m_pkDisplayManager;
	delete m_pkNetworkManager;
}

void SceneManager::AddNewScene(Scene* a_pkNewScene)
{
    std::cout<<"Adding New Scene"<<std::endl;

    m_pkTempScene = a_pkNewScene;
}

void SceneManager::ProcessAddNewScene()
{
    if(m_pkTempScene != NULL)
    {
        m_stScenes.push(m_pkTempScene);

        m_pkTempScene = NULL;
    }
}

void SceneManager::RemoveCurrentScene()
{
    std::cout<<"Removing Current Scene"<<std::endl;

    delete m_stScenes.top();
    m_stScenes.pop();
}

bool SceneManager::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"SceneManager Tick."<<std::endl;
    #endif

    ProcessAddNewScene();

    if(m_stScenes.size() == 0)
    {
        std::cout<<"Out of Scenes."<<std::endl;
        return false;
    }
    else
    {
        if(!m_pkInputManager->Update(a_fDeltaTime))
        {
            return false;
        }

#ifdef _WIN32
		if(m_pkInputManager->GetIsKeyDown(SDL_SCANCODE_F10))//Dump current memory leaks
        {
			_CrtDumpMemoryLeaks();
        }
        
        if(m_pkInputManager->GetIsKeyDown(SDL_SCANCODE_F12))//Toggle Wireframe Mode
        {
            m_pkDisplayManager->ToggleWireframeMode();
        }
#endif //_WIN32
        
		//TODO: Fix this hack, either move these into the gamescene or have them react to the menu properly
		if(m_stScenes.top()->GetSceneType() == eGameScene)
		{
			m_pkTileManager->Update(a_fDeltaTime);
			m_pkParticleManager->Update(a_fDeltaTime);
			m_pkUnitManager->Update(a_fDeltaTime);
		}

        if(m_stScenes.top()->Update(a_fDeltaTime))
        {
			m_pkNetworkManager->Update(a_fDeltaTime);

            m_pkDisplayManager->Update(a_fDeltaTime);
            
            return true;
        }
        else
        {
            RemoveCurrentScene();
            if(m_stScenes.size() == 0)
            {
                std::cout<<"Out of Scenes."<<std::endl;
                return false;
            }
            else
            {
                return true;
            }
        }

    }

    //Something's Wrong.
    std::cout<<"SceneManager Stack Error."<<std::endl;
    return false;
}

DisplayManager* SceneManager::GetDisplayManager()
{
    return m_pkDisplayManager;
}

InputManager* SceneManager::GetInputManager()
{
    return m_pkInputManager;
}

ObjectManager* SceneManager::GetObjectManager()
{
	return m_pkObjectManager;
}

SoundManager* SceneManager::GetSoundManager()
{
	return m_pkSoundManager;
}

NetworkManager* SceneManager::GetNetworkManager()
{
	return m_pkNetworkManager;
}

Scene* SceneManager::GetCurrentScene()
{
    return m_stScenes.top();
}

UnitManager* SceneManager::GetUnitManager()
{
    return m_pkUnitManager;
}

TileManager* SceneManager::GetTileManager()
{
	return m_pkTileManager;
}

ParticleManager* SceneManager::GetParticleManager()
{
	return m_pkParticleManager;
}