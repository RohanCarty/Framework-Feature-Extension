#ifndef _DISPLAYMANAGER_H_
#define _DISPLAYMANAGER_H_
//Dem debug things
#ifdef _WIN32
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <malloc.h>
		#include <crtdbg.h>
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
	   #endif
	#endif
#endif

#include <iostream>

#include <SDL2/SDL.h>

//Forward declare in order to use the pointer.
class Vector;
class Vertex;
class Mesh;
class Matrix;
class Texture;

struct stTextureInfo
{
	std::string m_szFileName;
	int m_iTextureNumber;
	unsigned int m_uiReferences;
};

class DisplayManager
{
public:
    DisplayManager(int argc, char **argv);
    virtual ~DisplayManager();

	virtual unsigned int LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader);

    virtual bool CreateScreen();

    virtual int LoadTexture(std::string a_sName);
	virtual int LoadTextureSDLSurface(SDL_Surface* a_pkSurface);
    
    virtual void UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber);

	virtual void UnloadTexture(int a_iTextureNumber);
	virtual void UnloadTexture(std::string a_szTextureFilename);

    virtual bool Update(float a_fDeltaTime);

	virtual bool ToggleWireframeMode();
	virtual void ReportLiveObjects();

	virtual int GetXScreenResolution();
	virtual int GetYScreenResolution();

	virtual unsigned int GetDefaultShader();
	virtual int GetDefaultTexture();

	virtual void SetViewMatrix(Matrix* a_pkNewMatrix);

	virtual void SetShaderProgram(unsigned int a_uiShaderProgramNumber);

    virtual bool Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* iTexture);
	virtual bool HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* iTexture);
protected:
	int m_iXResolution;
	int m_iYResolution;

	unsigned int m_uiDefaultShader;
	int m_iDefaultTexture;

	bool m_bWireframeMode;
};

#endif
