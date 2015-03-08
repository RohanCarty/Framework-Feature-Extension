#ifndef _D3D11DISPLAYMANAGER_H_
#define _D3D11DISPLAYMANAGER_H_

#include "DisplayManager.h"

//include the basic windows header files and the D3D header files
#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3d10.h>

#include <vector>

struct D3DVERTEX
{
      FLOAT X, Y, Z;	// position
      FLOAT U,V;		// TEXCOORD
};


struct stTextureInfoD3D
{
	std::string m_szFileName;
	ID3D11ShaderResourceView* m_pkTextureResource;
	unsigned int m_uiReferences;
};



class D3D11DisplayManager : public DisplayManager
{
public:
    D3D11DisplayManager(int argc, char **argv);
    virtual ~D3D11DisplayManager();

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

	// D3D declarations
	IDXGISwapChain* m_pkSwapchain;			//the pointer to the swap chain interface
	ID3D11Device* m_pkDevice;				//the pointer to our Direct3D device
	ID3D11DeviceContext* m_pkContext;		//the pointer to our Direct3D context
    ID3D11RenderTargetView* m_pkBackBuffer;	//The pointer to the backbuffer

	ID3D11Buffer* m_pkVertexBuffer;
	ID3D11InputLayout* m_pkInputLayout;

	ID3D11SamplerState* m_pkTexSamplerState;
	ID3D11BlendState* m_pkBlendState;

	ID3D11VertexShader* m_pkVertexShader;
	ID3D11PixelShader* m_pkPixelShader;

    Matrix* m_pkViewMatrix;
    float m_fCameraTop, m_fCameraRight;
    
	std::vector<stTextureInfoD3D> m_astLoadedTextures;
};

#endif
