#ifndef _D3D11DISPLAYMANAGER_H_
#define _D3D11DISPLAYMANAGER_H_

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

#include "DisplayManager.h"

//include the basic windows header files and the D3D header files
#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3d10.h>
#include <DirectXMath.h>

#include <DXGI.h>
#include <dxgi1_2.h>

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
	ID3D11Texture2D* m_pkTexture2D;
	unsigned int m_uiReferences;
};

struct VS_CONSTANT_BUFFER //make sure it's 16-byte aligned, IMPORTANT!
{
	DirectX::XMFLOAT4X4 mWorldViewProj;
	DirectX::XMFLOAT4X4 mObjectMatrix;
	DirectX::XMFLOAT4 vSomeVectorThatMayBeNeededByASpecificShader;
	DirectX::XMFLOAT2 vVectorThatMightAlsoBeNeeded;
	DirectX::XMFLOAT2 vVectorStatingCurrentResolution; 
};

class D3D11DisplayManager : public DisplayManager
{
public:
    D3D11DisplayManager(int argc, char **argv);
    virtual ~D3D11DisplayManager();

	virtual unsigned int LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader);
	std::string LoadShader(std::string a_szShaderName);

	bool CreateScreen();

    int LoadTexture(std::string a_sName);
	int LoadTextureSDLSurface(SDL_Surface* a_pkSurface);
	void UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber);

	void UnloadTexture(int a_iTextureNumber);
	void UnloadTexture(std::string a_szTextureFilename);

    bool Update(float a_fDeltaTime);
	
	void ReportLiveObjects();

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
	IDXGISwapChain1* m_pkSwapchain;			//the pointer to the swap chain interface
	ID3D11Device* m_pkDevice;				//the pointer to our Direct3D device
	ID3D11DeviceContext* m_pkContext;		//the pointer to our Direct3D context
    ID3D11RenderTargetView* m_pkBackBuffer;	//The pointer to the backbuffer

	ID3D11Buffer* m_pkVertexBuffer;
	ID3D11InputLayout* m_pkInputLayout;

	ID3D11SamplerState* m_pkTexSamplerState;
	ID3D11SamplerState* m_pkCurrentTexSamplerState;
	ID3D11ShaderResourceView* m_pkCurrentTextureResource;
	ID3D11BlendState* m_pkBlendState;

	ID3D11VertexShader* m_pkVertexShader;
	ID3D11PixelShader* m_pkPixelShader;
	ID3D11Buffer* m_pkVSConstantBuffer;

	D3DVERTEX m_pkVerticies[6];

	D3D11_MAPPED_SUBRESOURCE m_pkMappedSubresource;



    Matrix* m_pkViewMatrix;
    float m_fCameraTop, m_fCameraRight;

	float m_afTempClearColour[4];
    
	std::vector<stTextureInfoD3D> m_astLoadedTextures;
};

#endif
