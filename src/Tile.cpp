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
    stRenderable stTempRenderable;
	//Make a renderable for greenzones
    stTempRenderable.m_pkTexture = new AnimatedTexture(SceneManager::GetDisplayManager());
    stTempRenderable.m_pkTexture->LoadTexture("Resources/Textures/GreenZone.png", SceneManager::GetDisplayManager());
	Mesh* TempMesh = new Mesh();
	TempMesh->LoadMesh("Resources/Meshes/Plane.mesh");
	TempMesh->SetTexture(stTempRenderable.m_pkTexture);
    stTempRenderable.m_pkMesh = TempMesh;
    stTempRenderable.m_bIsHidden = true;
    m_apkRenderables.push_back(stTempRenderable);

    //Make a renderable for redzones
    stTempRenderable.m_pkTexture = new AnimatedTexture(SceneManager::GetDisplayManager());
    stTempRenderable.m_pkTexture->LoadTexture("Resources/Textures/RedZone.png", SceneManager::GetDisplayManager());
	TempMesh = new Mesh();
	TempMesh->LoadMesh("Resources/Meshes/Plane.mesh");
	TempMesh->SetTexture(stTempRenderable.m_pkTexture);
    stTempRenderable.m_pkMesh = TempMesh;
    stTempRenderable.m_bIsHidden = true;
    m_apkRenderables.push_back(stTempRenderable);

    SetScale(0.5f);
    SetSize(Vector(64,64,64));

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

void Tile::SetNoZone()
{
    m_apkRenderables[1].m_bIsHidden = true; //Green
    m_apkRenderables[2].m_bIsHidden = true; //Red
}

void Tile::SetGreenZone()
{
    m_apkRenderables[1].m_bIsHidden = false;
    m_apkRenderables[2].m_bIsHidden = true;
}

void Tile::SetRedZone()
{
    m_apkRenderables[1].m_bIsHidden = true;
    m_apkRenderables[2].m_bIsHidden = false;
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
