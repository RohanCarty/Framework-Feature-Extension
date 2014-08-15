#include "DisplayManager.h"
#include "Vector.h"
#include "Vertex.h"
#include "Texture.h"

DisplayManager::DisplayManager(int argc, char **argv)
{
    //TODO
    std::cout<<"DisplayManager Created."<<std::endl;

	//Set the defaults

	m_uiDefaultShader = 0;
	m_iDefaultTexture = 0;

	m_bWireframeMode = false;
}
DisplayManager::~DisplayManager()
{
    //TODO
}

unsigned int DisplayManager::LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader)
{
	//Let child handle it
	return 0;
}

bool DisplayManager::CreateScreen()
{
    //TODO
    return true;
}

int DisplayManager::LoadTexture(std::string a_sName)
{
    //TODO
    return 0;
}

int DisplayManager::LoadTextureSDLSurface(SDL_Surface* a_pkSurface)
{
	//Abstract Class
	return 0;
}

void DisplayManager::UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber)
{
    //TODO
}

void DisplayManager::UnloadTexture(int a_iTextureNumber)
{
	//Let child handle it
}

void DisplayManager::UnloadTexture(std::string a_szTextureFilename)
{
	//Let child handle it
}

bool DisplayManager::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"DisplayManager Tick."<<std::endl;
    #endif

    return true;
}

//Flips wireframe mode and returns the new state;
bool DisplayManager::ToggleWireframeMode()
{
	m_bWireframeMode = !m_bWireframeMode;

	return m_bWireframeMode;
}

int DisplayManager::GetXScreenResolution()
{
	return m_iXResolution;
}

int DisplayManager::GetYScreenResolution()
{
	return m_iYResolution;
}

unsigned int DisplayManager::GetDefaultShader()
{
	return m_uiDefaultShader;
}

int DisplayManager::GetDefaultTexture()
{
	return m_iDefaultTexture;
}

void DisplayManager::SetViewMatrix(Matrix* a_pkNewMatrix)
{
	//Just discard that bastard.
	return;
}

void DisplayManager::SetShaderProgram(unsigned int a_uiShaderProgramNumber)
{
	//Skip and let child class handle it.
	return;
}

bool DisplayManager::Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* a_iTexture)
{
    /*
    std::cout<<"Dummy DisplayManager Drawing. Locations: "<<std::endl;

    for( int i = 0; i < a_iSizeOfArray; i++)
    {
        std::cout<<a_aLocations[i].x<<", "<<a_aLocations[i].y<<std::endl;
    }

    std::cout<<"Texture: "<<a_iTexture<<std::endl;
    */
    return true;
}

bool DisplayManager::HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* a_iTexture)
{
	return true;
}
