#include "HUD.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "TextLibrary.h"
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

	//TODO: Displaying bars for health along with numbers.
}

HUD::~HUD()
{
    std::cout<<"HUD Destroyed. Pointer: "<<this<<std::endl;

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