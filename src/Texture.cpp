#include "Texture.h"
#include "DisplayManager.h"

//Texture Functions
Texture::Texture(DisplayManager* a_pDisplayManager)
{
	m_iTexture = a_pDisplayManager->GetDefaultTexture();
    m_fMinU = 0;
    m_fMinV = 0;
    m_fMaxU = 1;
    m_fMaxV = 1;

	m_bIsAnimated = false;
}

Texture::~Texture()
{

}

bool Texture::Update(float a_fDeltaTime)
{
    return true;
}

void Texture::LoadTexture(std::string a_sName, DisplayManager* a_pDisplayManager)
{
    SetTextureNumber(a_pDisplayManager->LoadTexture(a_sName));
}

int Texture::GetTextureNumber()
{
    return m_iTexture;
}

void Texture::SetTextureNumber(int a_iTexture)
{
    m_iTexture = a_iTexture;
}

bool Texture::GetIsAnimated()
{
	return m_bIsAnimated;
}