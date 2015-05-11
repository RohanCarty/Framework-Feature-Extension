#include "MainMenuScene.h"
#include "Actor.h"
#include "Button.h"
#include "Unit.h"
#include "HUD.h"
#include "Camera.h"
#include "Cursor.h"
#include "GameInfo.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "UnitManager.h"
#include "UIElement.h"

#include <cstdlib>

MainMenuScene::MainMenuScene() : Scene()
{
    std::cout<<"MainMenuScene created. Pointer: "<<this<<std::endl;

	m_iSceneType = eMenuScene;

	m_pkCamera = new Camera(this);

	SceneManager::GetInputManager()->SetCameraObject(m_pkCamera);

	m_pkCursor = new Cursor(this);

	m_pkCursor->SetCamera(m_pkCamera);

	m_pkNewGameButton = new Button(this);
	m_pkNewGameButton->SetLocation(256, 256, 0);
	m_pkNewGameButton->SetSize(Vector(128, 64, 0));

	m_pkQuitButton  = new Button(this);
	m_pkQuitButton->SetLocation(256, 512, 0);
	m_pkQuitButton->SetSize(Vector(128, 64, 0));
}

MainMenuScene::~MainMenuScene()
{
    std::cout<<"MainMenuScene Destroyed. Pointer: "<<this<<std::endl;

	delete m_pkCursor;

	//Make sure that the camera reference in the input manager is handled before destroying it
	SceneManager::GetInputManager()->SetCameraObject(NULL);

	delete m_pkCamera;
}

bool MainMenuScene::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"MainMenuScene Tick: "<<m_iCounter<<std::endl;
    #endif

	m_pkCursor->Update(a_fDeltaTime);

	m_pkNewGameButton->Update(a_fDeltaTime);

	if(m_pkNewGameButton->GetIsToggled()) // Load Game Scene
	{
		SceneManager::AddNewScene(new GameScene);
	}

	m_pkQuitButton->Update(a_fDeltaTime);
	
	if(m_pkQuitButton->GetIsToggled()) // Quit
	{
		std::cout<<"Quitting"<<std::endl;
		return false;
	}

    return true;
}

SceneManager* MainMenuScene::GetSceneManager()
{
    return m_pkSceneManager;
}
