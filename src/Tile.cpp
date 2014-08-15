#include "Tile.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"

#include "AnimatedTexture.h"

Tile::Tile(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Tile created. Pointer: "<<this<<std::endl;
    m_pkTexture->LoadTexture("Resources/Textures/GroundTexture.png", SceneManager::GetDisplayManager());
    SetScale(2.0f);
}

Tile::~Tile()
{
    std::cout<<"Tile Destroyed. Pointer: "<<this<<std::endl;
}

bool Tile::Update(float a_fDeltaTime)
{
#ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Tile Tick: "<<this<<std::endl;
#endif
    
    Object::Update(a_fDeltaTime);
    
    return true;
}
