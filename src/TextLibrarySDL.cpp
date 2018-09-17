#include "TextLibrarySDL.h"
#include "TextLibrary.h"
#include "Texture.h"
#include "Vector.h"
#include "Vertex.h"
#include "Mesh.h"

//#include "VulkanDisplayManager.h"
#include "PackManager.h"

TextLibrarySDL::TextLibrarySDL(DisplayManager* a_kDisplayManager) : TextLibrary(a_kDisplayManager)
{
	std::cout<<"TextLibrarySDL Initialized."<<std::endl;

	m_kDisplayManager = a_kDisplayManager;

    m_pkTexture = new Texture(m_kDisplayManager);

	//Just here so it ends up with a different texture number (fuck)

	m_pkTexture->LoadTexture("Resources/Textures/System/Text.png", m_kDisplayManager);

	if(TTF_Init() == -1)
	{
        std::cout<<"TTF_Init Error: "<<TTF_GetError()<<std::endl;
	}

	stTTFInfo stTempInfo;

	stTempInfo.szResourceFile = "Resources/Fonts/System/AldotheApache.ttf";
	stTempInfo.iSize = 10;

	int piSize;
	piSize = PackManager::GetSizeOfFile("Resources/Fonts/System/AldotheApache.ttf");
	stTempInfo.m_pkFont=TTF_OpenFontRW(SDL_RWFromMem(PackManager::LoadResource("Resources/Fonts/System/AldotheApache.ttf"), piSize), 1, 10);

    if(stTempInfo.m_pkFont == NULL)
    {
        std::cout<<"TTF_OpenFontRW Error: "<<TTF_GetError()<<std::endl;
	}
	else
	{
		m_apkTTFs.push_back(stTempInfo);
	}

	//m_pkFont = TTF_OpenFont("Resources/Fonts/System/AldotheApache.ttf", 10);

	//m_pkTexture->SetTextureNumber(LoadTextureSDLSurface());

	m_uiPreviousCharacterSize = 10;

	m_pakVerticies = new Vertex[4];

	m_pkMesh = new Mesh();
}

TextLibrarySDL::~TextLibrarySDL()
{
	while(m_apkTTFs.size() > 0)
	{
		TTF_CloseFont(m_apkTTFs[m_apkTTFs.size() - 1].m_pkFont);
		m_apkTTFs.pop_back();
	}
	//TTF_CloseFont(m_pkFont);

	TTF_Quit();
    delete m_pkTexture;
    delete[] m_pakVerticies;
	delete m_pkMesh;
}

//Prints Strings passed in through a string, takes a position and a character size.
void TextLibrarySDL::PrintString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	SDL_Color tTempColour;
	tTempColour.r = 255;
	tTempColour.g = 255;
	tTempColour.b = 255;
	tTempColour.a = 255;
	m_kDisplayManager->UpdateTextureSDLSurface(
		TTF_RenderText_Blended(GetTTFBySizeAndResource(CharacterSize, "Resources/Fonts/System/AldotheApache.ttf").m_pkFont, sString.c_str(), tTempColour), m_pkTexture->GetTextureNumber());

	TTF_SizeText(GetTTFBySizeAndResource(CharacterSize, "Resources/Fonts/System/AldotheApache.ttf").m_pkFont, sString.c_str(), &m_piSizeX, &m_piSizeY);

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

void TextLibrarySDL::PrintHUDString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	SDL_Color tTempColour;
	tTempColour.r = 255;
	tTempColour.g = 255;
	tTempColour.b = 255;
	tTempColour.a = 255;
	m_kDisplayManager->UpdateTextureSDLSurface(
		TTF_RenderText_Blended(GetTTFBySizeAndResource(CharacterSize, "Resources/Fonts/System/AldotheApache.ttf").m_pkFont, sString.c_str(), tTempColour), m_pkTexture->GetTextureNumber());

	TTF_SizeText(GetTTFBySizeAndResource(CharacterSize, "Resources/Fonts/System/AldotheApache.ttf").m_pkFont, sString.c_str(), &m_piSizeX, &m_piSizeY);

	m_pakVerticies[0].GetLocation()->x = x; m_pakVerticies[0].GetLocation()->y = y;
	m_pakVerticies[1].GetLocation()->x = x + m_piSizeX; m_pakVerticies[1].GetLocation()->y = y;
	m_pakVerticies[2].GetLocation()->x = x + m_piSizeX; m_pakVerticies[2].GetLocation()->y = y + m_piSizeY;
	m_pakVerticies[3].GetLocation()->x = x; m_pakVerticies[3].GetLocation()->y = y + m_piSizeY;

	m_kDisplayManager->HUDDraw(m_pakVerticies, 4, m_pkTexture);
}

Vector TextLibrarySDL::GetStringSize(std::string& sString, unsigned int CharacterSize)
{
	TTF_SizeText(GetTTFBySizeAndResource(CharacterSize, "Resources/Fonts/System/AldotheApache.ttf").m_pkFont, sString.c_str(), &m_piSizeX, &m_piSizeY);
	Vector vTemp;
	vTemp.x = m_piSizeX; vTemp.y = m_piSizeY;

	return vTemp;
}

stTTFInfo TextLibrarySDL::GetTTFBySizeAndResource(int a_iSize, std::string a_szResourceFile)
{
	for(unsigned int uiDx = 0; uiDx < m_apkTTFs.size(); uiDx++)
	{
		if(m_apkTTFs[uiDx].iSize == a_iSize)
		{
			if(m_apkTTFs[uiDx].szResourceFile == a_szResourceFile)
			{
				return m_apkTTFs[uiDx];
			}
		}
	}

	stTTFInfo stTempInfo;

	stTempInfo.szResourceFile = a_szResourceFile;
	stTempInfo.iSize = a_iSize;

	int piSize;
	piSize = PackManager::GetSizeOfFile(a_szResourceFile);
	stTempInfo.m_pkFont=TTF_OpenFontRW(SDL_RWFromMem(PackManager::LoadResource(a_szResourceFile), piSize), 1, a_iSize);

    if(stTempInfo.m_pkFont == NULL)
    {
        std::cout<<"TTF_OpenFontRW Error: "<<TTF_GetError()<<std::endl;
	}
	else
	{
		m_apkTTFs.push_back(stTempInfo);
	}

	return m_apkTTFs[m_apkTTFs.size() - 1];
}