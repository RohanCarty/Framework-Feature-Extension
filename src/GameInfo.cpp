#include "Player.h"
#include "GameInfo.h"
#include "GameScene.h"
#include "HUD.h"
#include "SceneManager.h"
#include "UnitManager.h"
#include "Building.h"

GameInfo::GameInfo(Scene* a_pkScene)
{
    std::cout<<"GameInfo created. Pointer: "<<this<<std::endl;

	m_pkScene = a_pkScene;

    m_iScore = 0;

	m_iHauntingLevel = 1;

    m_fHauntingIncreaseCooldown = 10.0f;

	m_fGameOverTimer = 7.5f;
}

GameInfo::~GameInfo()
{
    std::cout<<"GameInfo Destroyed. Pointer: "<<this<<std::endl;
}

bool GameInfo::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"GameInfo Tick: "<<this<<std::endl;
    #endif

	if(GetHauntingIncreaseCooldown() < 0.0f)
	{
		SetHauntingLevel(GetHauntingLevel() + 1);
	}
	else
	{
		SetHauntingIncreaseCooldown(GetHauntingIncreaseCooldown() - a_fDeltaTime);
	}

	if(AllPlayersDead())
	{
		m_fGameOverTimer -= a_fDeltaTime;

		std::string szTempString("Game Over");
		
		((GameScene*)m_pkScene)->m_pkHUD->PrintHUDString(szTempString, 
			SceneManager::GetDisplayManager()->GetXScreenResolution() / 2 - 45 * 2.4, SceneManager::GetDisplayManager()->GetYScreenResolution() / 2.35, 72);

		szTempString = "The Officers Have Fallen";

		((GameScene*)m_pkScene)->m_pkHUD->PrintHUDString(szTempString, 
			SceneManager::GetDisplayManager()->GetXScreenResolution() / 2 - 36 * 4, SceneManager::GetDisplayManager()->GetYScreenResolution() / 2, 36);

	}

	if(m_fGameOverTimer < 0.0f)
	{
		return false;
	}

    return true;
}

bool GameInfo::AllPlayersDead()
{
	for(unsigned int uiDx = 0; uiDx < SceneManager::GetUnitManager()->GetPlayerList().size(); uiDx++)
	{
		if(SceneManager::GetUnitManager()->GetPlayerList()[uiDx]->GetIsAlive())
		{
			return false;
		}
	}

	return true;
}

int GameInfo::GetScore()
{
    return m_iScore;
}

void GameInfo::ChangeScore(int a_iScore)
{
    m_iScore += a_iScore;
}

int GameInfo::GetHauntingLevel()
{
	return m_iHauntingLevel;
}

//Will increment the haunting level as well as reset the timer to the correct time.
void GameInfo::SetHauntingLevel(int a_iHauntingLevel)
{
	m_iHauntingLevel = a_iHauntingLevel;

	SetHauntingIncreaseCooldown(10.0f * (float)m_iHauntingLevel);
}

float GameInfo::GetHauntingIncreaseCooldown()
{
	return m_fHauntingIncreaseCooldown;
}

void GameInfo::SetHauntingIncreaseCooldown(float a_fHauntingIncreaseCooldown)
{
	m_fHauntingIncreaseCooldown = a_fHauntingIncreaseCooldown;
}
