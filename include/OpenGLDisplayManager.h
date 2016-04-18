#ifndef __UWP__
#ifndef _OPENGLDISPLAYMANAGER_H_
#define _OPENGLDISPLAYMANAGER_H_
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
#include <string>
#include <vector>

#ifdef __APPLE__
#include <GL/GLEW.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include "DisplayManager.h"

//Forward declare in order to use the pointer.
class Vector;
class Vertex;
class Mesh;
class Matrix;
class Texture;

class OpenGLDisplayManager : public DisplayManager
{
public:
    OpenGLDisplayManager(int argc, char **argv);
    virtual ~OpenGLDisplayManager();

	unsigned int LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader);

	std::string LoadShader(std::string a_szShaderName);

    bool CreateScreen(int argc, char **argv);

	int GetEmptyTextureNumber();

    int LoadTexture(std::string a_sName);
	int LoadTextureSDLSurface(SDL_Surface* a_pkSurface);
	void UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber);

	virtual void UnloadTexture(int a_iTextureNumber);
	virtual void UnloadTexture(std::string a_szTextureFilename);

    bool Update(float a_fDeltaTime);

	float TransformToScreenSpaceX(double a_pkPosition);
	float TransformToScreenSpaceY(double a_pkPosition);

	float HUDTransformToScreenSpaceX(double a_pkPosition);
	float HUDTransformToScreenSpaceY(double a_pkPosition);

	void SetViewMatrix(Matrix* a_pkNewMatrix);
	void SetCameraDimensions(float a_fCameraTop, float a_fCameraRight);

	virtual void SetShaderProgram(unsigned int a_uiShaderProgramNumber);

	bool HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* a_pkTexture);
    bool Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* a_pkTexture);

private:
	GLfloat* m_akMeshData;

	int m_iCurrentMeshDataSize;

	GLuint* m_akVao;
	GLuint* m_akBuffers;

	Matrix* m_pkViewMatrix;
	float m_fCameraTop, m_fCameraRight;
	
	SDL_Window* m_pkMainWindow;
	SDL_GLContext m_kMainContext;

	GLuint m_gluiProgramObject;

	std::vector<stTextureInfo> m_astLoadedTextures;
};

#endif
#endif //__UWP__