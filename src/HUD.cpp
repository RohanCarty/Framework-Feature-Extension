#include "HUD.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "TextLibrary.h"
#include "Vector.h"
#include "Texture.h"
#include "GameInfo.h"

#include <cstdlib>
#include <cstdio>

HUD::HUD(Scene* a_pkScene)
{
    std::cout<<"HUD created. Pointer: "<<this<<std::endl;

    m_pkScene = a_pkScene;

    m_pkTextLibrary = new TextLibrary(SceneManager::GetDisplayManager());

    m_cpTempString = new char[256];
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

    //Population
	sprintf(m_cpTempString,"%d",((GameScene*)m_pkScene)->GetGameInfo()->GetScore());
	sTest = m_cpTempString;
	sTest = "Score: " + sTest;

    m_pkTextLibrary->PrintHUDString(sTest, 0,0,iFontSize);
    
    sprintf(m_cpTempString,"%d", (int) (1.0f / a_fDeltaTime ));
    
    sTest = "FPS: ";
    sTest = sTest + m_cpTempString; //what the fuck LLVM?
    
    m_pkTextLibrary->PrintHUDString(sTest, 0, SceneManager::GetDisplayManager()->GetYScreenResolution() - iFontSize, iFontSize);

    //Current Score, TODO
	/*sprintf(m_cpTempString,"%d",((GameScene*)m_pkScene)->GetGameInfo()->GetScore());
    sTest = m_cpTempString;
    sTest = "Score: " + sTest;

    m_pkTextLibrary->PrintHUDString(sTest, SceneManager::GetDisplayManager()->GetXScreenResolution() - (m_pkTextLibrary->GetStringSize(sTest, iFontSize).x),0,iFontSize);*/

    return true;
}

TextLibrary* HUD::GetTextLibrary()
{
	return m_pkTextLibrary;
}