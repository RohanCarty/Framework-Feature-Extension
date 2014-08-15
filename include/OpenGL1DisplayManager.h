#ifndef _OPENGL1DISPLAYMANAGER_H_
#define _OPENGL1DISPLAYMANAGER_H_

#include "DisplayManager.h"

#include <vector>

class OpenGL1DisplayManager : public DisplayManager
{
public:
    OpenGL1DisplayManager(int argc, char **argv);
    virtual ~OpenGL1DisplayManager();

	virtual unsigned int LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader);

	bool CreateScreen();

    int LoadTexture(std::string a_sName);
	int LoadTextureSDLSurface(SDL_Surface* a_pkSurface);
	void UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber);

	void UnloadTexture(int a_iTextureNumber);
	void UnloadTexture(std::string a_szTextureFilename);

    bool Update(float a_fDeltaTime);

	float TransformToScreenSpaceX(double a_pkPosition);
	float TransformToScreenSpaceY(double a_pkPosition);
    
	float HUDTransformToScreenSpaceX(double a_pkPosition);
	float HUDTransformToScreenSpaceY(double a_pkPosition);
    
	void SetViewMatrix(Matrix* a_pkNewMatrix);
	void SetCameraDimensions(float a_fCameraTop, float a_fCameraRight);

	void SetShaderProgram(unsigned int a_uiShaderProgramNumber);

    bool Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* iTexture);
	bool HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* iTexture);
private:
    SDL_Window* m_pkMainWindow;
	SDL_GLContext m_kMainContext;
    
    Matrix* m_pkViewMatrix;
    float m_fCameraTop, m_fCameraRight;
    
	std::vector<stTextureInfo> m_astLoadedTextures;
};

#endif
