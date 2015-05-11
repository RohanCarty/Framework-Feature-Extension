#include "UIElement.h"
#include "SceneManager.h"
#include "Vector.h"
#include "Vertex.h"
#include "Mesh.h"
#include "AnimatedTexture.h"
#include "Matrix.h"

#include <cstdlib>

UIElement::UIElement(Scene* a_pkScene) : Object(a_pkScene)
{
    std::cout<<"UIElement created. Pointer: "<<this<<std::endl;

	m_bIsInFocus = false;
}

UIElement::~UIElement()
{
    std::cout<<"UIElement Destroyed. Pointer: "<<this<<std::endl;
}

bool UIElement::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Button Tick: "<<m_iCounter<<std::endl;
    #endif

	HUDDraw(0.0f);

    return true;
}

void UIElement::SetSize(Vector a_vSize)
{
	m_vSize = a_vSize;
	GetRenderables()[0].m_pkMesh->GenerateBasicMesh(a_vSize.x, a_vSize.y, a_vSize.z);
}

void UIElement::SetScale(float a_fScale)
{
	m_fScale = a_fScale;
}

//Drawn a HUD progress bar.
/*void HUD::DrawHUDProgressBar(Vector a_vPosition, Vector a_vSize, float a_fNormalisedPercentage)
{
	//get the two textures used for progress bars.

	//use the same object for all of them.
	m_pkProgressBarBacking->SetLocation(a_vPosition);
	m_pkProgressBar->SetLocation(a_vPosition);

	Vector vProgressSize = a_vSize;
	vProgressSize.x = a_vSize.x * a_fNormalisedPercentage;

	m_pkProgressBarBacking->GetRenderables()[0].m_pkMesh->GenerateBasicMesh(a_vSize.x, a_vSize.y, a_vSize.z);
	m_pkProgressBar->GetRenderables()[0].m_pkMesh->GenerateBasicMesh(vProgressSize.x, vProgressSize.y, vProgressSize.z);

	m_pkProgressBarBacking->HUDDraw(0.0f);
	m_pkProgressBar->HUDDraw(0.0f);
}*/