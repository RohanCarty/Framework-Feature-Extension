#include "GameInfo.h"
#include "SceneManager.h"
#include "UnitManager.h"
#include "Building.h"

GameInfo::GameInfo(Scene* a_pkScene)
{
    std::cout<<"GameInfo created. Pointer: "<<this<<std::endl;

    m_iScore = 0;

    m_fVillagerSpawnCooldown = 3.0f;
}

GameInfo::~GameInfo()
{
    std::cout<<"GameInfo Destroyed. Pointer: "<<this<<std::endl;
}

int GameInfo::GetScore()
{
    return m_iScore;
}

void GameInfo::ChangeScore(int a_iScore)
{
    m_iScore += a_iScore;
}

bool GameInfo::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"GameInfo Tick: "<<this<<std::endl;
    #endif

    return true;
}
