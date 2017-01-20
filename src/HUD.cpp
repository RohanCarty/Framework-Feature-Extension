#include "HUD.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "TextLibrary.h"
#include "TextLibrarySDL.h"
#include "Mesh.h"
#include "AnimatedTexture.h"
#include "Vector.h"
#include "Texture.h"
#include "GameInfo.h"
#include "UIElement.h"
#include "UnitManager.h"
#include "Player.h"

#ifdef _WIN32
#include <msvcunistd.h>>
#include <Lmcons.h>
#else
#include <unistd.h>
#endif //#ifdef _WIN32

#include <cstdlib>
#include <cstdio>

HUD::HUD(Scene* a_pkScene)
{
    std::cout<<"HUD created. Pointer: "<<this<<std::endl;

    m_pkScene = a_pkScene;

    m_pkTextLibrary = new TextLibrarySDL(SceneManager::GetDisplayManager());

    m_cpTempString = new char[256];

	m_pkProgressBarBacking = new Object(a_pkScene);

	m_pkProgressBar = new Object(a_pkScene);

	m_pkProgressBarBacking->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/White.png", SceneManager::GetDisplayManager());

	m_pkProgressBar->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/Red.png", SceneManager::GetDisplayManager());
}

HUD::~HUD()
{
    std::cout<<"HUD Destroyed. Pointer: "<<this<<std::endl;

	while(m_astPlayerInfos.size() > 0)
	{
		delete m_astPlayerInfos.back().pkBackgroundObject;
		delete m_astPlayerInfos.back().pkAbilityIcon1;
		delete m_astPlayerInfos.back().pkYButton;
		delete m_astPlayerInfos.back().pkAbilityIcon2;
		delete m_astPlayerInfos.back().pkBButton;
		delete m_astPlayerInfos.back().pkReviveIcon;
		delete m_astPlayerInfos.back().pkViewButton;

		m_astPlayerInfos.pop_back();
	}

	delete m_pkProgressBar;

	delete m_pkProgressBarBacking;

    delete m_cpTempString;

	delete m_pkTextLibrary;
}

bool HUD::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"HUD Tick: "<<this<<std::endl;
    #endif
	
	//Change shader back to default
	SceneManager::GetDisplayManager()->SetShaderProgram(SceneManager::GetDisplayManager()->GetDefaultShader());

	int iFontSize = 24;//SceneManager::GetDisplayManager()->GetXScreenResolution() * 11;

    std::string sTest;

	//Print FPS
#ifdef _WIN32
	sprintf_s(m_cpTempString, 256,"%d", (int) (1.0f / a_fDeltaTime ));
#else
	sprintf(m_cpTempString, "%d", (int)(1.0f / a_fDeltaTime));
#endif //_WIN32
    sTest = "FPS: ";
    sTest = sTest + m_cpTempString; //what the fuck LLVM?
    
    m_pkTextLibrary->PrintHUDString(sTest, 0, 0, iFontSize);

	//Print Haunting Level
	sprintf(m_cpTempString, "%d", ((GameScene*)SceneManager::GetCurrentScene())->GetGameInfo()->GetHauntingLevel());

	sTest = "Haunting Level: ";
	sTest = sTest + m_cpTempString; //what the fuck LLVM?

	m_pkTextLibrary->PrintHUDString(sTest, SceneManager::GetDisplayManager()->GetXScreenResolution() - (iFontSize * 10), 0, iFontSize);

	//Print time until increase of Haunting Level
	sprintf(m_cpTempString, "%d", (int)((GameScene*)SceneManager::GetCurrentScene())->GetGameInfo()->GetHauntingIncreaseCooldown());

	sTest = "Next Level In: ";
	sTest = sTest + m_cpTempString; //what the fuck LLVM?

	m_pkTextLibrary->PrintHUDString(sTest, SceneManager::GetDisplayManager()->GetXScreenResolution() - (iFontSize * 10), iFontSize * 2, iFontSize);

    //Print the details of the players included in the game.
	for(unsigned int uiDx = 0; uiDx < m_astPlayerInfos.size(); uiDx++)
	{
		//Display all the icons for the hud
		SetPositionOfPlayerInfoObjects(uiDx, Vector((uiDx) * 384 + 200, SceneManager::GetDisplayManager()->GetYScreenResolution() - 100,0));

		m_astPlayerInfos[uiDx].pkBackgroundObject->Update(a_fDeltaTime);
		m_astPlayerInfos[uiDx].pkAbilityIcon1->Update(a_fDeltaTime);
		m_astPlayerInfos[uiDx].pkYButton->Update(a_fDeltaTime);
		m_astPlayerInfos[uiDx].pkAbilityIcon2->Update(a_fDeltaTime);
		m_astPlayerInfos[uiDx].pkBButton->Update(a_fDeltaTime);
		m_astPlayerInfos[uiDx].pkReviveIcon->Update(a_fDeltaTime);
		m_astPlayerInfos[uiDx].pkViewButton->Update(a_fDeltaTime);

		//Print Gamertag //TODO: Implement, for now just a test string.

		m_pkTextLibrary->PrintHUDString(m_astPlayerInfos[uiDx].szGamertag, (uiDx) * 384 + 100, SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 3 - 100, iFontSize);

		//Print current health
		sprintf(m_cpTempString,"%d", SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetHealth());
		sTest = m_cpTempString;
		sTest = "Health: " + sTest;

		Vector vTempPosition((uiDx) * 384 + 272,SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 2 + 12 - 100,0);
		((GameScene*)m_pkScene)->m_pkHUD->DrawHUDProgressBar(vTempPosition,Vector(96,16,0), (float)SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetHealth() / 100.0f);

		m_pkTextLibrary->PrintHUDString(sTest, (uiDx) * 384 + 100, SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 2 - 100, iFontSize);

		//Print current soul power.
		sprintf(m_cpTempString,"%d", SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetCurrentSoulPowerLevel());
		sTest = m_cpTempString;
		sTest = "Power: " + sTest;

		vTempPosition =  Vector((uiDx) * 384 + 272,SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 1 + 12 - 100,0);
		((GameScene*)m_pkScene)->m_pkHUD->DrawHUDProgressBar(vTempPosition,Vector(96,16,0), (float)SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetCurrentSoulPowerLevel() / 100.0f);

		m_pkTextLibrary->PrintHUDString(sTest, (uiDx) * 384 + 100, SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 1 - 100, iFontSize);
	}

    return true;
}

TextLibrary* HUD::GetTextLibrary()
{
	return m_pkTextLibrary;
}

//Drawn an in world progress bar.
void HUD::DrawProgressBar(Vector a_vPosition, Vector a_vSize, float a_fNormalisedPercentage)
{
	//get the two textures used for progress bars.

	//use the same object for all of them.
	m_pkProgressBarBacking->SetLocation(a_vPosition);
	m_pkProgressBar->SetLocation(a_vPosition);

	Vector vProgressSize = a_vSize;
	vProgressSize.x = a_vSize.x * a_fNormalisedPercentage;

	m_pkProgressBarBacking->GetRenderables()[0].m_pkMesh->GenerateBasicMesh(a_vSize.x, a_vSize.y, a_vSize.z);
	m_pkProgressBar->GetRenderables()[0].m_pkMesh->GenerateBasicMesh(vProgressSize.x, vProgressSize.y, vProgressSize.z);

	m_pkProgressBarBacking->Update(0.0f);
	m_pkProgressBar->Update(0.0f);
}

//Drawn a HUD progress bar.
void HUD::DrawHUDProgressBar(Vector a_vPosition, Vector a_vSize, float a_fNormalisedPercentage)
{
	//get the two textures used for progress bars.

	//use the same object for all of them.
	m_pkProgressBarBacking->SetLocation(a_vPosition);
	m_pkProgressBar->SetLocation(a_vPosition);

	Vector vProgressSize = a_vSize;
	vProgressSize.x = a_vSize.x * a_fNormalisedPercentage;

	m_pkProgressBarBacking->GetRenderables()[0].m_pkMesh->GenerateBasicMesh(a_vSize.x, a_vSize.y, a_vSize.z);
	m_pkProgressBar->GetRenderables()[0].m_pkMesh->GenerateBasicMesh(vProgressSize.x, vProgressSize.y, vProgressSize.z);

	m_pkProgressBarBacking->HUDDraw(0.0f);
	m_pkProgressBar->HUDDraw(0.0f);
}

void HUD::PrintString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	GetTextLibrary()->PrintString(sString, x, y, CharacterSize);
}

void HUD::PrintHUDString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	GetTextLibrary()->PrintHUDString(sString, x, y, CharacterSize);
}

//Populates all the player info structs ready for updating.
void HUD::PopulatePlayerInfos()
{
	//Clear all the player infos to ensure that it's all populated.
	while(m_astPlayerInfos.size() > 0)
	{
		delete m_astPlayerInfos.back().pkBackgroundObject;
		delete m_astPlayerInfos.back().pkAbilityIcon1;
		delete m_astPlayerInfos.back().pkYButton;
		delete m_astPlayerInfos.back().pkAbilityIcon2;
		delete m_astPlayerInfos.back().pkBButton;
		delete m_astPlayerInfos.back().pkReviveIcon;
		delete m_astPlayerInfos.back().pkViewButton;

		m_astPlayerInfos.pop_back();
	}

	for(unsigned int uiDx = 0; uiDx < SceneManager::GetInputManager()->GetNumConnectedControllers(); uiDx++)
	{
		stPlayerInfo stTempInfo;

		stTempInfo.szGamertag = "Gamertag";

		//temp way of getting name (will read from an options file and will use these functions to generate default names)
#ifdef _WIN32
#ifndef __UWP__
		wchar_t* cpGamerTag;

		cpGamerTag = new wchar_t[UNLEN + 1];

		DWORD username_len = UNLEN + 1;
		int temp = GetUserName(cpGamerTag, &username_len);

		std::cout << "GetUserName result: " << temp << std::endl;

		std::wstring ws(cpGamerTag);

		stTempInfo.szGamertag = std::string(ws.begin(), ws.end());
		delete cpGamerTag;
#endif //#ifndef __UWP__
#else
		char* cpGamerTag = new char[256];
		int namelen = 0;
		getlogin_r(cpGamerTag, &namelen);
		stTempInfo.szGamertag = std::string(cpGamerTag);
		delete cpGamerTag;
#endif //#ifdef _WIN32

		stTempInfo.pkBackgroundObject = new UIElement(m_pkScene);
		stTempInfo.pkBackgroundObject->SetSize(Vector(300,200,0));
		stTempInfo.pkBackgroundObject->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/AbilityIcons/HudBackground.png", SceneManager::GetDisplayManager());

		stTempInfo.pkAbilityIcon1 = new UIElement(m_pkScene);
		stTempInfo.pkAbilityIcon1->SetSize(Vector(64, 64, 0));
		stTempInfo.pkAbilityIcon1->GetRenderables()[0].m_pkTexture->LoadTexture(SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetResourceStringOfAbility(SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->m_iSpecial1Ability), SceneManager::GetDisplayManager());

		stTempInfo.pkYButton = new UIElement(m_pkScene);
		stTempInfo.pkYButton->SetSize(Vector(24, 24, 0));
		stTempInfo.pkYButton->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/AbilityIcons/YButton.png", SceneManager::GetDisplayManager());

		stTempInfo.pkAbilityIcon2 = new UIElement(m_pkScene);
		stTempInfo.pkAbilityIcon2->SetSize(Vector(64, 64, 0));
		stTempInfo.pkAbilityIcon2->GetRenderables()[0].m_pkTexture->LoadTexture(SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetResourceStringOfAbility(SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->m_iSpecial2Ability), SceneManager::GetDisplayManager());

		stTempInfo.pkBButton = new UIElement(m_pkScene);
		stTempInfo.pkBButton->SetSize(Vector(24, 24, 0));
		stTempInfo.pkBButton->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/AbilityIcons/BButton.png", SceneManager::GetDisplayManager());

		stTempInfo.pkReviveIcon = new UIElement(m_pkScene);
		stTempInfo.pkReviveIcon->SetSize(Vector(64, 64, 0));
		stTempInfo.pkReviveIcon->GetRenderables()[0].m_pkTexture->LoadTexture(SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetResourceStringOfAbility(eRevive), SceneManager::GetDisplayManager());

		stTempInfo.pkViewButton = new UIElement(m_pkScene);
		stTempInfo.pkViewButton->SetSize(Vector(24, 24, 0));
		stTempInfo.pkViewButton->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/AbilityIcons/ViewButton.png", SceneManager::GetDisplayManager());

		m_astPlayerInfos.push_back(stTempInfo);
	}
}

void HUD::SetPositionOfPlayerInfoObjects(unsigned int a_uiInfoToUpdate, Vector a_vNewPosition)
{
	Vector vTempPosition = a_vNewPosition;

	m_astPlayerInfos[a_uiInfoToUpdate].pkBackgroundObject->SetLocation(vTempPosition);

	vTempPosition = a_vNewPosition + Vector(-68, 46, 0);

	m_astPlayerInfos[a_uiInfoToUpdate].pkAbilityIcon1->SetLocation(vTempPosition);

	vTempPosition = a_vNewPosition + Vector(-84, 70, 0);

	m_astPlayerInfos[a_uiInfoToUpdate].pkYButton->SetLocation(vTempPosition);

	vTempPosition = a_vNewPosition + Vector(0, 46, 0);

	m_astPlayerInfos[a_uiInfoToUpdate].pkAbilityIcon2->SetLocation(vTempPosition);

	vTempPosition = a_vNewPosition + Vector(-16, 70, 0);

	m_astPlayerInfos[a_uiInfoToUpdate].pkBButton->SetLocation(vTempPosition);

	vTempPosition = a_vNewPosition + Vector(68, 46, 0);

	m_astPlayerInfos[a_uiInfoToUpdate].pkReviveIcon->SetLocation(vTempPosition);

	vTempPosition = a_vNewPosition + Vector(52, 70, 0);

	m_astPlayerInfos[a_uiInfoToUpdate].pkViewButton->SetLocation(vTempPosition);

}