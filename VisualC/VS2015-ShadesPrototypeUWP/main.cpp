#include  <SDL.h>
#include "SDL2/SDL_syswm.h"

#include <Windows.h>
#include <windowsx.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <d3d10.h>
#include <string>
#include <iostream>
#include <d3dcompiler.h>

struct D3DVERTEX
{
	FLOAT X, Y, Z;	// position
	FLOAT U, V;		// TEXCOORD
};


struct stTextureInfoD3D
{
	std::string m_szFileName;
	ID3D11ShaderResourceView* m_pkTextureResource;
	unsigned int m_uiReferences;
};


int main(int argc, char **argv)
{

	SDL_Window* m_pkMainWindow;

	// D3D declarations
	IDXGISwapChain1* m_pkSwapchain;			//the pointer to the swap chain interface
	ID3D11Device* m_pkDevice;				//the pointer to our Direct3D device
	ID3D11DeviceContext* m_pkContext;		//the pointer to our Direct3D context
	ID3D11RenderTargetView* m_pkBackBuffer;	//The pointer to the backbuffer

	ID3D11Buffer* m_pkVertexBuffer;
	ID3D11InputLayout* m_pkInputLayout;

	ID3D11SamplerState* m_pkTexSamplerState;
	ID3D11BlendState* m_pkBlendState;

	ID3D11VertexShader* m_pkVertexShader;
	ID3D11PixelShader* m_pkPixelShader;

	float m_fCameraTop, m_fCameraRight;

	float m_afTempClearColour[4];

	int m_iXResolution;
	int m_iYResolution;


	SDL_DisplayMode mode;
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;
	SDL_Event evt;
	IDXGIFactory2 * m_pkDXGIFactory;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		return 1;
	}

	if (SDL_GetCurrentDisplayMode(0, &mode) != 0)
	{
		return 1;
	}

	//TODO
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Die if SDL2 Init failed
	{
		std::cout << "Unable to initialize SDL2" << std::endl;
		exit(1);
	}

	m_iXResolution = 1920;
	m_iYResolution = 1080;

	m_pkMainWindow = SDL_CreateWindow("Pegasus Feather Engine 0.5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_iXResolution, m_iYResolution, SDL_WINDOW_SHOWN);

	if (!m_pkMainWindow) // Die if window creation failed
	{
		std::cout << "Window Creation Failed After Succesful SDL2 Initialization." << std::endl;
		exit(1);
	}

	//retrieve native window handle info to pass to directX
	SDL_SysWMinfo kInfo;
	SDL_VERSION(&kInfo.version); //required before calling GetWindowWMInfo
	SDL_GetWindowWMInfo(m_pkMainWindow, &kInfo);

	//create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC1 scd;

	//clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC1));

	//fill the swap chain description struct
	scd.BufferCount = 2;									//Back buffer with 2 drawzones
	scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				//Use 32-bit colour
	scd.Width = m_iXResolution;								//Set the back buffer width
	scd.Height = m_iYResolution;							//Set the back buffer height
	scd.Stereo = false;										//3D - no please
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//How swap chain is to be used
	scd.SampleDesc.Count = 1;								//How many multisamples				<== MSAA
	scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;				//Set Alpha mode;
	scd.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;		//Set scaling mode;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;		//Set swap mode (sequential is one after another, all we need)

	//Create a device, device context, then a swap chain using the scd struct
	HRESULT hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
#ifdef _DEBUG
		D3D11_CREATE_DEVICE_DEBUG,
#else
		NULL,
#endif
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&m_pkDevice,
		NULL,
		&m_pkContext);


	IDXGIDevice1 * pDXGIDevice;
	hr = m_pkDevice->QueryInterface(__uuidof(IDXGIDevice1), (void **)&pDXGIDevice);

	IDXGIAdapter1 * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter1), (void **)&pDXGIAdapter);

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void **)&m_pkDXGIFactory);

	hr = m_pkDXGIFactory->CreateSwapChainForCoreWindow(
		m_pkDevice,
		kInfo.info.winrt.window,
		&scd,
		nullptr,
		&m_pkSwapchain);

	SDL_ShowCursor(1); //Ensure cursor will display

	//Block for detecting videocard info;

	ZeroMemory(&hr, sizeof(HRESULT));

	hr = m_pkDevice->QueryInterface(__uuidof(IDXGIDevice1), (void **)&pDXGIDevice);

	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter1), (void **)&pDXGIAdapter);

	DXGI_ADAPTER_DESC1 kAdapterDescription;

	pDXGIAdapter->GetDesc1(&kAdapterDescription);

	std::wstring wszDescription = std::wstring(kAdapterDescription.Description);
	std::string szDescription = std::string(wszDescription.begin(), wszDescription.end());

	std::cout << "Window created, D3D Context: " << std::endl
		<< "Description: " << szDescription << std::endl
		<< "VRAM: " << kAdapterDescription.DedicatedVideoMemory / 1048576 << std::endl //divide by this number to change from bytes to megabytes
		<< "Revision: " << kAdapterDescription.Revision << std::endl
		<< "Vendor ID: " << kAdapterDescription.VendorId << std::endl
		<< "Device ID: " << kAdapterDescription.DeviceId << std::endl;

	pDXGIAdapter->Release();
	pDXGIDevice->Release();
	//End of videocard info block

	//Bind backbuffer to swapchain

	//get the address of the back buffer
	ID3D11Texture2D* pBackBuffer;
	m_pkSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//use the back buffer address to create the render target
	m_pkDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pkBackBuffer);
	pBackBuffer->Release();

	//set the render target as the back buffer
	m_pkContext->OMSetRenderTargets(1, &m_pkBackBuffer, NULL);

	//End of binding back buffer render target

	//Set the viewport

	D3D11_VIEWPORT kViewport;
	ZeroMemory(&kViewport, sizeof(D3D11_VIEWPORT));

	kViewport.TopLeftX = 0;
	kViewport.TopLeftY = 0;
	kViewport.Width = m_iXResolution;
	kViewport.Height = m_iYResolution;

	m_pkContext->RSSetViewports(1, &kViewport);

	//End of setting the viewport

	//Clear the back buffer to prepare for first frame.
	//Set the clear colour array
	m_afTempClearColour[0] = 0.1f; m_afTempClearColour[1] = 1.0f; m_afTempClearColour[2] = 0.1f; m_afTempClearColour[3] = 1.0f;  //R,G,B,A

	m_pkContext->ClearRenderTargetView(m_pkBackBuffer, m_afTempClearColour);

	//Setting up vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;					//Write access by CPU and GPU
	bd.ByteWidth = sizeof(D3DVERTEX) * 6;			//Size is the D3DVERTEX struct * 3	//TODO: Allow changing of this.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//Use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Allow CPU to write in buffer

	m_pkDevice->CreateBuffer(&bd, NULL, &m_pkVertexBuffer); //Create the buffer

															//Finished with buffer, load default shader.

															//Load and compile the two shaders
	ID3D10Blob *VS, *PS;

	std::string cpFullFile("Texture2D ObjTexture;\nSamplerState ObjSamplerState;\nstruct VS_OUTPUT\n{\nfloat4 Pos : SV_POSITION;\nfloat2 TexCoord : TEXCOORD;\n};\nVS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)\n{\nVS_OUTPUT output;\noutput.Pos = inPos;\noutput.TexCoord = inTexCoord;\nreturn output;\n}\nfloat4 PS(VS_OUTPUT input) : SV_TARGET\n{\nreturn ObjTexture.Sample(ObjSamplerState, input.TexCoord);\n}\n");

	int piSize = cpFullFile.size();

	cpFullFile.resize(piSize);

	hr = NULL;
	hr = D3DCompile(cpFullFile.c_str(), piSize, "D3DShaders.shader", 0, 0, "VS", "vs_4_0", 0, 0, &VS, 0);
	if (hr != NULL)
	{
		std::cout << "Shader error: " << hr << std::endl;
	}

	hr = NULL;
	hr = D3DCompile(cpFullFile.c_str(), piSize, "D3DShaders.shader", 0, 0, "PS", "ps_4_0", 0, 0, &PS, 0);
	if (hr != NULL)
	{
		std::cout << "Shader error: " << hr << std::endl;
	}

	//Encapsulate both shaders into shader objects
	m_pkDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pkVertexShader);
	m_pkDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pkPixelShader);

	//Set the shader objects
	m_pkContext->VSSetShader(m_pkVertexShader, 0, 0);
	m_pkContext->PSSetShader(m_pkPixelShader, 0, 0);

	//Create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_pkDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pkInputLayout);
	//Set it
	m_pkContext->IASetInputLayout(m_pkInputLayout);

	//Create sampler state, we'll use this for all textures for now.
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pkDevice->CreateSamplerState(&sampDesc, &m_pkTexSamplerState);

	// Enable Transparency and set Transparency settings.

	//Create the blend state
	D3D11_BLEND_DESC bsd;
	ZeroMemory(&bsd, sizeof(bsd));
	bsd.RenderTarget[0].BlendEnable = TRUE;
	bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	m_pkDevice->CreateBlendState(&bsd, &m_pkBlendState);

	float blendFactor[] = { 0, 0, 0, 0 };
	UINT sampleMask = 0xffffffff;

	m_pkContext->OMSetBlendState(m_pkBlendState, blendFactor, sampleMask);

	//Have set the blend state

	while (1)
	{
		while (SDL_PollEvent(&evt))
		{
			//Show the frame.
			m_pkSwapchain->Present(0, 0);

			//Clear the back buffer for the next frame.

			m_pkContext->ClearRenderTargetView(m_pkBackBuffer, m_afTempClearColour);

		}
	}
}