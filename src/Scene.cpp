#include "Scene.h"
#include "Actor.h"
#include "HUD.h"
#include "GameInfo.h"
#include "SceneManager.h"

#include <cstdlib>

Scene::Scene()
{
    std::cout<<"Scene created. Pointer: "<<this<<std::endl;

	m_iSceneType = eDefaultScene;
}

Scene::~Scene()
{
    std::cout<<"Scene Destroyed. Pointer: "<<this<<std::endl;
}

bool Scene::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Scene Tick: "<<m_iCounter<<std::endl;
    #endif

    return true;
}

int Scene::GetSceneType()
{
	return m_iSceneType;
}
