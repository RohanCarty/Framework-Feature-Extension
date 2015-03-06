#include "Tile.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Mesh.h"

#include "AnimatedTexture.h"

Tile::Tile(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Tile created. Pointer: "<<this<<std::endl;
    m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/LevelTiles.png", SceneManager::GetDisplayManager());

	m_bIsCollidable = false;

    SetScale(0.5f);
    SetSize(Vector(64,64,64));
    
    m_iObjectType = eTile;
}

Tile::~Tile()
{
#ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Tile Destroyed. Pointer: "<<this<<std::endl;
#endif
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

bool Tile::GetIsCollidable()
{
	return m_bIsCollidable;
}

void Tile::ChangeTileType(int a_iTileType)
{
	switch(a_iTileType)
	{
	case eWall:
		if(rand()% 2 + 1 == 1)
		{
			m_apkRenderables[0].m_pkTexture->m_fMinU = 0.0;
			m_apkRenderables[0].m_pkTexture->m_fMinV = 0.0;
			m_apkRenderables[0].m_pkTexture->m_fMaxU = 0.03125;
			m_apkRenderables[0].m_pkTexture->m_fMaxV = 0.03125;
		}
		else
		{
			m_apkRenderables[0].m_pkTexture->m_fMinU = 0.03125;
			m_apkRenderables[0].m_pkTexture->m_fMinV = 0.0;
			m_apkRenderables[0].m_pkTexture->m_fMaxU = 0.0625;
			m_apkRenderables[0].m_pkTexture->m_fMaxV = 0.03125;
		}
		m_bIsCollidable = true;
		break;
	case eFloor:
		m_apkRenderables[0].m_pkTexture->m_fMinU = 0.0625;
		m_apkRenderables[0].m_pkTexture->m_fMinV = 0.0;
		m_apkRenderables[0].m_pkTexture->m_fMaxU = 0.09375;
		m_apkRenderables[0].m_pkTexture->m_fMaxV = 0.03125;
		m_bIsCollidable = true;
		break;
	case eFloor2:
		m_apkRenderables[0].m_pkTexture->m_fMinU = 0.09375;
		m_apkRenderables[0].m_pkTexture->m_fMinV = 0.0;
		m_apkRenderables[0].m_pkTexture->m_fMaxU = 0.125;
		m_apkRenderables[0].m_pkTexture->m_fMaxV = 0.03125;
		m_bIsCollidable = true;
		break;
	case eBackground:
		if(rand()% 2 + 1 == 1)
		{
			m_apkRenderables[0].m_pkTexture->m_fMinU = 0.125;
			m_apkRenderables[0].m_pkTexture->m_fMinV = 0.0;
			m_apkRenderables[0].m_pkTexture->m_fMaxU = 0.15625;
			m_apkRenderables[0].m_pkTexture->m_fMaxV = 0.03125;
		}
		else
		{
			m_apkRenderables[0].m_pkTexture->m_fMinU = 0.15625;
			m_apkRenderables[0].m_pkTexture->m_fMinV = 0.0;
			m_apkRenderables[0].m_pkTexture->m_fMaxU = 0.1875;
			m_apkRenderables[0].m_pkTexture->m_fMaxV = 0.03125;
		}
		break;
	default:	//set the default case has just having the top left tile, will make an error tile at some point.
		m_apkRenderables[0].m_pkTexture->m_fMaxU = 0.03125;
		m_apkRenderables[0].m_pkTexture->m_fMaxV = 0.03125;
		m_bIsCollidable = false;
		break;
	}
}
void Tile::RandomiseRotation()
{
	SetRotation((rand() % 3 + 1) * 3.1415926535f / 2.0f);
}