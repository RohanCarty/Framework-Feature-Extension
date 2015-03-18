#include "HUD.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "TextLibrary.h"
#include "Mesh.h"
#include "AnimatedTexture.h"
#include "Vector.h"
#include "Texture.h"
#include "GameInfo.h"
#include "Player.h"

#include <cstdlib>
#include <cstdio>

HUD::HUD(Scene* a_pkScene)
{
    std::cout<<"HUD created. Pointer: "<<this<<std::endl;

    m_pkScene = a_pkScene;

    m_pkTextLibrary = new TextLibrary(SceneManager::GetDisplayManager());

    m_cpTempString = new char[256];

	m_pkProgressBarBacking = new Object(a_pkScene);

	m_pkProgressBar = new Object(a_pkScene);

	m_pkProgressBarBacking->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/White.png", SceneManager::GetDisplayManager());

	m_pkProgressBar->GetRenderables()[0].m_pkTexture->LoadTexture("Resources/Textures/Red.png", SceneManager::GetDisplayManager());

	//TODO: Displaying bars for health along with numbers.
}

HUD::~HUD()
{
    std::cout<<"HUD Destroyed. Pointer: "<<this<<std::endl;

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

	int iFontSize = 36;//SceneManager::GetDisplayManager()->GetXScreenResolution() * 11;

    std::string sTest;

	//Print FPS
	sprintf(m_cpTempString,"%d", (int) (1.0f / a_fDeltaTime ));
    
    sTest = "FPS: ";
    sTest = sTest + m_cpTempString; //what the fuck LLVM?
    
    m_pkTextLibrary->PrintHUDString(sTest, 0, 0, iFontSize);

    //Print the details of the players included in the game.
	for(unsigned int uiDx = 0; uiDx < SceneManager::GetUnitManager()->GetPlayerList().size(); uiDx++)
	{
		//Print Gamertag //TODO: Implement, for now just number of players.
		sprintf(m_cpTempString,"%d", uiDx + 1);
		sTest = m_cpTempString;
		sTest = "Player " + sTest;

		m_pkTextLibrary->PrintHUDString(sTest, uiDx + 1 * 128, SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 3, iFontSize);

		//Print current health
		sprintf(m_cpTempString,"%d", SceneManager::GetUnitManager()->GetPlayerList()[0]->GetHealth());
		sTest = m_cpTempString;
		sTest = "Health: " + sTest;

		m_pkTextLibrary->PrintHUDString(sTest, uiDx + 1 * 128, SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 2, iFontSize);

		//Print current soul power.
		sprintf(m_cpTempString,"%d", SceneManager::GetUnitManager()->GetPlayerList()[0]->GetCurrentSoulPowerLevel());
		sTest = m_cpTempString;
		sTest = "Soul Power Level: " + sTest;

		m_pkTextLibrary->PrintHUDString(sTest, uiDx + 1 * 128, SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize * 1, iFontSize);
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