#ifndef _OPENGLES2DISPLAYMANAGER_H_
#define _OPENGLES2DISPLAYMANAGER_H_
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

#ifndef _WIN32
#define APIENTRY
#endif

#include <iostream>
#include <string>
#include <vector>
#ifdef __arm__
#include "SDL_opengles2.h"
#else
#include "SDL_opengl.h"
#endif

#include "DisplayManager.h"

//Forward declare in order to use the pointer.
class Vector;
class Vertex;
class Mesh;
class Matrix;
class Texture;

class OpenGLES2DisplayManager : public DisplayManager
{
public:
	OpenGLES2DisplayManager(int argc, char **argv);
    virtual ~OpenGLES2DisplayManager();

	unsigned int LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader);

	std::string LoadShader(std::string a_szShaderName);

    bool CreateScreen(int argc, char **argv);

	bool SetupExtensions();

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

	GLuint* m_akVbo;
	GLuint* m_akBuffers;

	GLint positionLoc, texCoordLoc, samplerLoc;

	unsigned int CreateSimpleTexture2D();

	Matrix* m_pkViewMatrix;
	float m_fCameraTop, m_fCameraRight;
	
	SDL_Window* m_pkMainWindow;
	SDL_GLContext m_kMainContext;

	GLuint m_gluiProgramObject;

	std::vector<stTextureInfo> m_astLoadedTextures;

	typedef GLuint(APIENTRY * glCreateShader_Func)(GLenum);
	glCreateShader_Func glCreateShader_ptr;

	typedef void(APIENTRY * glShaderSource_Func)(GLuint, GLsizei, const char **, const GLint *);
	glShaderSource_Func glShaderSource_ptr;

	typedef void(APIENTRY * glCompileShader_Func)(GLuint);
	glCompileShader_Func glCompileShader_ptr;

	typedef void(APIENTRY * glGetShaderiv_Func)(GLuint, GLenum, GLint *);
	glGetShaderiv_Func glGetShaderiv_ptr;

	typedef GLuint(APIENTRY * glCreateProgram_Func)(void);
	glCreateProgram_Func glCreateProgram_ptr;

	typedef void(APIENTRY * glAttachShader_Func)(GLuint, GLuint);
	glAttachShader_Func glAttachShader_ptr;

	typedef void(APIENTRY * glLinkProgram_Func)(GLuint);
	glLinkProgram_Func glLinkProgram_ptr;

	typedef void(APIENTRY * glUseProgram_Func)(GLuint);
	glUseProgram_Func glUseProgram_ptr;

	typedef void(APIENTRY * glGenBuffers_Func)(GLsizei n, GLuint* buffers);//
	glGenBuffers_Func glGenBuffers_ptr;

	typedef void(APIENTRY * glBindBuffer_Func)(GLenum target, GLuint buffer);
	glBindBuffer_Func glBindBuffer_ptr;

	typedef void(APIENTRY * glBufferData_Func)(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
	glBufferData_Func glBufferData_ptr;

	typedef int(APIENTRY * glGetAttribLocation_Func)(GLuint program, const GLchar* name);
	glGetAttribLocation_Func glGetAttribLocation_ptr;

	typedef void(APIENTRY * glEnableVertexAttribArray_Func)(GLuint index);
	glEnableVertexAttribArray_Func glEnableVertexAttribArray_ptr;

	typedef void(APIENTRY * glVertexAttribPointer_Func)(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr);
	glVertexAttribPointer_Func glVertexAttribPointer_ptr;

	typedef GLint(APIENTRY * glGetUniformLocation_Func)(GLuint program, const GLchar *name);
	glGetUniformLocation_Func glGetUniformLocation_ptr;

	typedef void(APIENTRY * glActiveTexture_Func)(GLenum texture);
	glActiveTexture_Func glActiveTexture_ptr;

	typedef void(APIENTRY * glUniform1i_Func)(GLint location, GLint v0);
	glUniform1i_Func glUniform1i_ptr;
};

#endif
