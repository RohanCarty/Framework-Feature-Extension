#include "Tile.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"

#include "AnimatedTexture.h"

Tile::Tile(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Tile created. Pointer: "<<this<<std::endl;
    //m_pkTexture->LoadTexture("Resources/Textures/GroundTexture.png", SceneManager::GetDisplayManager());
    SetScale(2.0f);
    SetSize(Vector(256,256,256));
    
    m_fCost = 1.0f;
    m_pkOccupiedBy = NULL;
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

void Tile::SetIsOccupied(Object* a_pkOccupiedBy)
{
    m_pkOccupiedBy = a_pkOccupiedBy;
}

bool Tile::GetIsOccupied()
{
    if(m_pkOccupiedBy == NULL)
    {
        return false;
    }
    
    return true;
}

Object* Tile::GetIsOccupiedBy()
{
    return m_pkOccupiedBy;
}

void Tile::SetCost(float a_fNewCost)
{
    m_fCost = a_fNewCost;
}

float Tile::GetCost()
{
    return m_fCost;
}

void Tile::SetCoordinate(Vector a_vCoordinates)
{
    m_vCoordinates = a_vCoordinates;
}

Vector Tile::GetCoordinate()
{
    return m_vCoordinates;
}