#include "Tile.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Mesh.h"

#include "AnimatedTexture.h"

Tile::Tile(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Tile created. Pointer: "<<this<<std::endl;
    m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/GroundTexture.png", SceneManager::GetDisplayManager());

    SetScale(0.5f);
    SetSize(Vector(64,64,64));
    
    m_iObjectType = eTile;
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

void Tile::SetCoordinate(Vector a_vCoordinates)
{
    m_vCoordinates = a_vCoordinates;
}

Vector Tile::GetCoordinate()
{
    return m_vCoordinates;
}
