#include "GameScene.h"
#include "Actor.h"
#include "Unit.h"
#include "HUD.h"
#include "Camera.h"
#include "Cursor.h"
#include "GameInfo.h"
#include "SceneManager.h"
#include "UnitManager.h"

#include <cstdlib>

GameScene::GameScene() : Scene()
{
    std::cout<<"GameScene created. Pointer: "<<this<<std::endl;

    //m_pkPlayer = new Player(this);

    m_pkHUD = new HUD(this);
    m_pkGameInfo = new GameInfo(this);

	m_pkCamera = new Camera(this);

	SceneManager::GetInputManager()->SetCameraObject(m_pkCamera);

	m_pkCursor = new Cursor(this);

	m_pkCursor->SetCamera(m_pkCamera);

	//m_pkCamera->SetLocation(960, 540, 0);

	m_pkCamera->SetLocation(0,0,0);
}

GameScene::~GameScene()
{
    std::cout<<"GameScene Destroyed. Pointer: "<<this<<std::endl;

	delete m_pkCursor;

    delete m_pkHUD;

	//Make sure that the camera reference in the input manager is handled before destroying it
	SceneManager::GetInputManager()->SetCameraObject(NULL);

	delete m_pkCamera;

    delete m_pkGameInfo;
}

bool GameScene::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"GameScene Tick: "<<m_iCounter<<std::endl;
    #endif

    m_pkHUD->Update(a_fDeltaTime);
	m_pkCamera->Update(a_fDeltaTime);
	m_pkCursor->Update(a_fDeltaTime);
	m_pkGameInfo->Update(a_fDeltaTime);

    return true;
}

void GameScene::ReloadAllObjectScripts()
{

}

SceneManager* GameScene::GetSceneManager()
{
    return m_pkSceneManager;
}

GameInfo* GameScene::GetGameInfo()
{
    return m_pkGameInfo;
}

