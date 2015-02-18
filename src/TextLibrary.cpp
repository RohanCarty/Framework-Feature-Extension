#include "TextLibrary.h"
#include "Texture.h"
#include "Vector.h"
#include "Vertex.h"
#include "Mesh.h"

#include "OpenGLDisplayManager.h"
#include "PackManager.h"

TextLibrary::TextLibrary(DisplayManager* a_kDisplayManager)
{
	std::cout<<"TextLibrary Initialized."<<std::endl;

	m_kDisplayManager = a_kDisplayManager;

    m_pkTexture = new Texture(m_kDisplayManager);

	//Just here so it ends up with a different texture number (fuck)

	m_pkTexture->LoadTexture("Resources/Textures/System/Text.png", m_kDisplayManager);

	if(TTF_Init() == -1)
	{
        std::cout<<"TTF_Init Error: "<<TTF_GetError()<<std::endl;
	}

	int piSize;
	piSize = PackManager::GetSizeOfFile("Resources/Fonts/System/AldotheApache.ttf");
	m_pkFont=TTF_OpenFontRW(SDL_RWFromMem(PackManager::LoadResource("Resources/Fonts/System/AldotheApache.ttf"), piSize), 1, 10);

    if(m_pkFont == NULL)
    {
        std::cout<<"TTF_OpenFontRW Error: "<<TTF_GetError()<<std::endl;
    }

	//m_pkFont = TTF_OpenFont("Resources/Fonts/System/AldotheApache.ttf", 10);

	//m_pkTexture->SetTextureNumber(LoadTextureSDLSurface());

	m_uiPreviousCharacterSize = 10;

	m_pakVerticies = new Vertex[4];

	m_pkMesh = new Mesh();
}

TextLibrary::~TextLibrary()
{
	TTF_CloseFont(m_pkFont);

	TTF_Quit();
    delete m_pkTexture;
    delete[] m_pakVerticies;
	delete m_pkMesh;
}

//Prints Strings passed in through a string, takes a position and a character size.
void TextLibrary::PrintString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	if(CharacterSize > m_uiPreviousCharacterSize)
	{
		TTF_CloseFont(m_pkFont);
		int piSize;
        piSize = PackManager::GetSizeOfFile("Resources/Fonts/System/AldotheApache.ttf");
		m_pkFont=TTF_OpenFontRW(SDL_RWFromMem(PackManager::LoadResource("Resources/Fonts/System/AldotheApache.ttf"), piSize),1 , CharacterSize);
		//m_pkFont = TTF_OpenFont("Resources/Fonts/System/AldotheApache.ttf", CharacterSize);
		m_uiPreviousCharacterSize = CharacterSize;
	}
	SDL_Color tTempColour;
	tTempColour.r = 255;
	tTempColour.g = 255;
	tTempColour.b = 255;
	tTempColour.a = 255;
	m_kDisplayManager->UpdateTextureSDLSurface(
		TTF_RenderText_Blended(m_pkFont, sString.c_str(), tTempColour), m_pkTexture->GetTextureNumber());

	TTF_SizeText(m_pkFont, sString.c_str(), &m_piSizeX, &m_piSizeY);

	m_pkMesh->GenerateBasicMesh((float)m_piSizeX, (float)m_piSizeY, 0);

	for(int iDx = 0; iDx < m_pkMesh->GetNumberOfVertices(); iDx++)
	{
		m_pkMesh->GetVertexArray()[iDx].SetLocation(Vector(m_pkMesh->GetVertexArray()[iDx].GetLocation()->x + x, m_pkMesh->GetVertexArray()[iDx].GetLocation()->y + y, m_pkMesh->GetVertexArray()[iDx].GetLocation()->z + 0));
	}

	/*m_pakVerticies[0].GetLocation()->x = x; m_pakVerticies[0].GetLocation()->y = y;
	m_pakVerticies[1].GetLocation()->x = x + m_piSizeX; m_pakVerticies[1].GetLocation()->y = y;
	m_pakVerticies[2].GetLocation()->x = x + m_piSizeX; m_pakVerticies[2].GetLocation()->y = y + m_piSizeY;
	m_pakVerticies[3].GetLocation()->x = x; m_pakVerticies[3].GetLocation()->y = y + m_piSizeY;*/

	m_kDisplayManager->Draw(m_pkMesh, 4, m_pkTexture);
}

//TODO: remove hud versions entirely
void TextLibrary::PrintHUDString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	if(CharacterSize > m_uiPreviousCharacterSize)
	{
		TTF_CloseFont(m_pkFont);
		int piSize;
        piSize = PackManager::GetSizeOfFile("Resources/Fonts/System/AldotheApache.ttf");
		m_pkFont=TTF_OpenFontRW(SDL_RWFromMem(PackManager::LoadResource("Resources/Fonts/System/AldotheApache.ttf"), piSize),1 , CharacterSize);
		//m_pkFont = TTF_OpenFont("Resources/Fonts/System/AldotheApache.ttf", CharacterSize);
		m_uiPreviousCharacterSize = CharacterSize;
	}
	SDL_Color tTempColour;
	tTempColour.r = 255;
	tTempColour.g = 255;
	tTempColour.b = 255;
	tTempColour.a = 255;
	m_kDisplayManager->UpdateTextureSDLSurface(
		TTF_RenderText_Blended(m_pkFont, sString.c_str(), tTempColour), m_pkTexture->GetTextureNumber());

	TTF_SizeText(m_pkFont, sString.c_str(), &m_piSizeX, &m_piSizeY);

	m_pakVerticies[0].GetLocation()->x = x; m_pakVerticies[0].GetLocation()->y = y;
	m_pakVerticies[1].GetLocation()->x = x + m_piSizeX; m_pakVerticies[1].GetLocation()->y = y;
	m_pakVerticies[2].GetLocation()->x = x + m_piSizeX; m_pakVerticies[2].GetLocation()->y = y + m_piSizeY;
	m_pakVerticies[3].GetLocation()->x = x; m_pakVerticies[3].GetLocation()->y = y + m_piSizeY;

	m_kDisplayManager->HUDDraw(m_pakVerticies, 4, m_pkTexture);
}

Vector TextLibrary::GetStringSize(std::string& sString, unsigned int CharacterSize)
{
	if(CharacterSize > m_uiPreviousCharacterSize)
	{
		TTF_CloseFont(m_pkFont);
		int piSize;
        piSize = PackManager::GetSizeOfFile("Resources/Fonts/System/AldotheApache.ttf");
		m_pkFont=TTF_OpenFontRW(SDL_RWFromMem(PackManager::LoadResource("Resources/Fonts/System/AldotheApache.ttf"), piSize),1 , CharacterSize);
		//m_pkFont = TTF_OpenFont("Resources/Fonts/System/AldotheApache.ttf", CharacterSize);
		m_uiPreviousCharacterSize = CharacterSize;
	}
	TTF_SizeText(m_pkFont, sString.c_str(), &m_piSizeX, &m_piSizeY);
	Vector vTemp;
	vTemp.x = m_piSizeX; vTemp.y = m_piSizeY;

	return vTemp;
}
