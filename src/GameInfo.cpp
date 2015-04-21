#include "GameInfo.h"
#include "SceneManager.h"
#include "UnitManager.h"
#include "Building.h"

GameInfo::GameInfo(Scene* a_pkScene)
{
    std::cout<<"GameInfo created. Pointer: "<<this<<std::endl;

    m_iScore = 0;

	m_iHauntingLevel = 0;

    m_fHauntingIncreaseCooldown = 10.0f;
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
