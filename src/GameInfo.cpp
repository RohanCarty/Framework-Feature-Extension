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

int GameInfo::GetCurrentPopulation()
{
	return (int)SceneManager::GetUnitManager()->GetUnitList().size();
}

int GameInfo::GetMaxPopulation()
{
	int iTempMax = 0;

    for(int iDx = 0;iDx < SceneManager::GetUnitManager()->GetBuildingList().size(); iDx++)
	{
		iTempMax += SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetPopulationCapacity();
	}

	return iTempMax;
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

	if(m_fVillagerSpawnCooldown > 0.0f)
	{
		m_fVillagerSpawnCooldown -= a_fDeltaTime;
		//std::cout<<m_fVillagerSpawnCooldown<<std::endl;
	}

	if(m_fVillagerSpawnCooldown < 0.0f && GetCurrentPopulation() < GetMaxPopulation()) //and add a food requirement
	{
		SceneManager::GetUnitManager()->SpawnNewUnit(0);
		m_fVillagerSpawnCooldown = 3.0f;
	}

    return true;
}
