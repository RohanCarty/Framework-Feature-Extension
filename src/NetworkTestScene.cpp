#include "NetworkTestScene.h"
#include "Actor.h"
#include "Unit.h"
#include "HUD.h"
#include "Camera.h"
#include "Cursor.h"
#include "GameInfo.h"
#include "SceneManager.h"
#include "UnitManager.h"

#include <cstdlib>

NetworkTestScene::NetworkTestScene() : Scene()
{
    std::cout<<"NetworkTestScene created. Pointer: "<<this<<std::endl;

    m_iSceneType = eGameScene;

    m_pkHUD = new HUD(this);
    m_pkGameInfo = new GameInfo(this);
	m_pkGameInfo->SetHauntingLevel(0);
	m_pkGameInfo->SetHauntingIncreaseCooldown(999);

	m_pkCamera = new Camera(this);

	SceneManager::GetInputManager()->SetCameraObject(m_pkCamera);

	m_pkCursor = new Cursor(this);

	m_pkCursor->SetCamera(m_pkCamera);

	//m_pkCamera->SetLocation(960, 540, 0);

	//m_pkCamera->SetLocation(0,0,0);
}

NetworkTestScene::~NetworkTestScene()
{
    std::cout<<"GameScene Destroyed. Pointer: "<<this<<std::endl;

	delete m_pkCursor;

    delete m_pkHUD;

	//Make sure that the camera reference in the input manager is handled before destroying it
	SceneManager::GetInputManager()->SetCameraObject(NULL);

	delete m_pkCamera;

    delete m_pkGameInfo;
}

bool NetworkTestScene::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"NetworkTestScene Tick: "<<m_iCounter<<std::endl;
    #endif

    m_pkHUD->Update(a_fDeltaTime);
	m_pkCamera->Update(a_fDeltaTime);
	m_pkCursor->Update(a_fDeltaTime);

	std::string szTempString("UNDEFINED STRING");
	std::stringstream szTempSStream;

	if (SceneManager::GetNetworkManager()->m_bNetworkEnabled)
	{
		szTempString = "Networking Enabled: True";
	}
	else
	{
		szTempString = "Networking Enabled: False";
	}

	m_pkHUD->PrintHUDString(szTempString, 0, 100, 12);

	if (SceneManager::GetNetworkManager()->GetIsConnected() && SceneManager::GetNetworkManager()->m_bIsClient)
	{
		szTempString = "IP Address of remote server: " + SceneManager::GetNetworkManager()->GetRemoteIPAddressString();
	}
	else if (SceneManager::GetNetworkManager()->GetIsConnected() && !SceneManager::GetNetworkManager()->m_bIsClient)
	{
		szTempString = "Server running on port: ";
		szTempString += std::to_string(SceneManager::GetNetworkManager()->m_uiPort);
	}
	else
	{
		szTempString = "No Server/Client running";
	}

	m_pkHUD->PrintHUDString(szTempString, 0, 120, 12);

	if (!m_pkGameInfo->Update(a_fDeltaTime))
	{
		return false;
	}

    return true;
}

void NetworkTestScene::ReloadAllObjectScripts()
{

}

SceneManager* NetworkTestScene::GetSceneManager()
{
    return m_pkSceneManager;
}

GameInfo* NetworkTestScene::GetGameInfo()
{
    return m_pkGameInfo;
}

HUD* NetworkTestScene::GetHUD()
{
	return m_pkHUD;
}
