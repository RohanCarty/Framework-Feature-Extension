#ifdef _WIN32

#include "DisplayManager.h"
#include "Vector.h"
#include "Matrix.h"
#include "Vertex.h"
#include "Texture.h"
#include "Mesh.h"
#include "PackManager.h"

#include "D3D11DisplayManager.h"

#include "SDL2/SDL_syswm.h"
#include "SDL_image.h"

// include the Direct3D Library file

#include <DXGI.h>
#include <dxgi1_2.h>

#include <D3Dcompiler.h>


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3d10.lib")
#pragma comment (lib, "D3Dcompiler.lib")

#pragma comment (lib, "comsuppw.lib")

D3D11DisplayManager::D3D11DisplayManager(int argc, char **argv) : DisplayManager(argc, argv)
{
    //TODO
    std::cout<<"D3D11DisplayManager Created."<<std::endl;

    m_pkViewMatrix = new Matrix();

    CreateScreen();
}

D3D11DisplayManager::~D3D11DisplayManager()
{
	//Kill all textures;
	for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		m_astLoadedTextures[iDx].m_pkTextureResource->Release();
		//std::cout<<"Texture already in memory: "<<a_sName<<std::endl;
		/*if(m_astLoadedTextures[iDx].m_pkTextureResource != NULL) //increase the number of references to this texture.
		{
			UnloadTexture(iDx);
		}*/
	}

	m_pkContext->ClearState();

	m_pkSwapchain->SetFullscreenState(FALSE, NULL);		//Switch to windowed mode on close, y'know, because directx needs that.

	//Close and release all existing COM objects
	m_pkInputLayout->Release();
	m_pkTexSamplerState->Release();
	m_pkBlendState->Release();
	m_pkVertexBuffer->Release();
	m_pkVertexShader->Release();
	m_pkPixelShader->Release();
	m_pkSwapchain->Release();
	m_pkBackBuffer->Release();
	m_pkDevice->Release();

	m_pkContext->Flush();
	//Report any live objects to help hunt memory leaks.
	ReportLiveObjects();
	m_pkContext->Release();

	std::cout<<"Closing Window"<<std::endl;
    SDL_DestroyWindow(m_pkMainWindow);
	std::cout<<"Closing SDL2"<<std::endl;
    SDL_Quit();

    delete m_pkViewMatrix;

	std::cout<<"D3D11 Displaymanager closed successfully."<<std::endl;
}

unsigned int D3D11DisplayManager::LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader)
{
	//Load and compile the two shaders
	ID3D10Blob *VS, *PS;

	std::string cpFullFile("Texture2D ObjTexture;\nSamplerState ObjSamplerState;\nstruct VS_OUTPUT\n{\nfloat4 Pos : SV_POSITION;\nfloat2 TexCoord : TEXCOORD;\n};\nVS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)\n{\nVS_OUTPUT output;\noutput.Pos = inPos;\noutput.TexCoord = inTexCoord;\nreturn output;\n}\nfloat4 PS(VS_OUTPUT input) : SV_TARGET\n{\nreturn ObjTexture.Sample(ObjSamplerState, input.TexCoord);\n}\n");

	int piSize = cpFullFile.size();

	cpFullFile.resize(piSize);

	HRESULT hr = NULL;

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

	return 0;
}

bool D3D11DisplayManager::CreateScreen()
{
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

	m_iXResolution = 1280;
	m_iYResolution = 720;

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
	scd.SampleDesc.Quality = 0;								//Quality of the multisamples
	scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;				//Set Alpha mode;
#ifdef __UWP__
	scd.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;		//Set scaling mode;
#else
	scd.Scaling = DXGI_SCALING_STRETCH;		//Set scaling mode; (Aspect ratio scaling won't work on non core-window instances
#endif //#ifndef __UWP__
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
#ifndef __UWP__
	hr = m_pkDXGIFactory->CreateSwapChainForHwnd(
		m_pkDevice,
		kInfo.info.win.window,
		&scd,
		nullptr,
		nullptr,
		&m_pkSwapchain);
#else //#ifndef __UWP__
	hr = m_pkDXGIFactory->CreateSwapChainForCoreWindow(
		m_pkDevice,
		kInfo.info.winrt.window,
		&scd,
		nullptr,
		&m_pkSwapchain);
#endif //#ifndef __UWP__

	if (hr != S_OK)
	{
		std::cout<< "Create Swap Chain Error: " << hr << std::endl;

		return false;
	}

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
	m_afTempClearColour[0] = 0.0f; m_afTempClearColour[1] = 0.0f; m_afTempClearColour[2] = 0.0f; m_afTempClearColour[3] = 0.0f;  //R,G,B,A

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
	LoadShaderProgram("Resources/Shaders/System/D3DShaders.shader", "Resources/Shaders/System/D3DShaders.shader");
	

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

    return true;
}

int D3D11DisplayManager::LoadTexture(std::string a_sName)
{
	//Check to see if texture has already been used
	for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if(a_sName == m_astLoadedTextures[iDx].m_szFileName)
		{
			//std::cout<<"Texture already in memory: "<<a_sName<<std::endl;
			m_astLoadedTextures[iDx].m_uiReferences++; //increase the number of references to this texture.
			return iDx;
		}
	}
	SDL_Surface *pkImage;
	int piSize = 0;
	//Hacky as fuck
	piSize = PackManager::GetSizeOfFile(a_sName.c_str());

	void* pTempResource = PackManager::LoadResource(a_sName.c_str());

	SDL_RWops* pkImgBufferTemp = SDL_RWFromMem(pTempResource, piSize);

	if(pkImgBufferTemp == NULL)
	{
		//std::cout<<"IMG_Load failue: "<<IMG_GetError()<<std::endl;
		std::cout<<"SDL_RWOps failue Size:"<<piSize<<" : "<<SDL_GetError()<<std::endl;
	}
	pkImage=IMG_Load_RW(pkImgBufferTemp, 1);

	delete pTempResource;

	//pkImage=SDL_LoadBMP(a_sName.c_str());
	if(pkImage == NULL)
	{
		//std::cout<<"IMG_Load failue: "<<IMG_GetError()<<std::endl;
		std::cout<<"IMG_Load failue: "<<a_sName<<": "<<SDL_GetError()<<std::endl;
		// handle error
		return m_iDefaultTexture;
	}

	// Check image dimensions are powers of 2
	if ( (pkImage->w & (pkImage->w - 1)) != 0 )
	{
		std::cout<<"Warning: "<<a_sName<<"'s width is not a power of 2."<<std::endl;
	}
	if ( (pkImage->h & (pkImage->h - 1)) != 0 )
	{
		std::cout<<"Warning: "<<a_sName<<"'s height is not a power of 2."<<std::endl;
	}

	//Create texture
	D3D11_TEXTURE2D_DESC t2d;
	ZeroMemory(&t2d, sizeof(t2d));
	t2d.Width = pkImage->w;
	t2d.Height = pkImage->h;
	t2d.MipLevels = 1;
	t2d.ArraySize = 1;
	t2d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	t2d.SampleDesc.Count = 1;
	t2d.SampleDesc.Quality = 0;
	t2d.Usage = D3D11_USAGE_DEFAULT;
	t2d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	t2d.CPUAccessFlags = 0;
	t2d.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = pkImage->pixels;
	initData.SysMemPitch = pkImage->pitch;

	//Create a single texture name.
	HRESULT hr;

	ID3D11Texture2D* pkTex = nullptr;
	hr = m_pkDevice->CreateTexture2D(& t2d, &initData, &pkTex);

	//Bind the texture name that was created.
	ID3D11ShaderResourceView* m_pkTexture = nullptr;

	if( SUCCEEDED(hr) && pkTex != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        memset( &SRVDesc, 0, sizeof( SRVDesc ) );
        SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//Set some parameters
        SRVDesc.Texture2D.MipLevels = 1;

		hr = m_pkDevice->CreateShaderResourceView( pkTex, &SRVDesc, &m_pkTexture);

		if(FAILED(hr))
		{
			m_pkTexture->Release();

			return 0; // return 0 for the default texture if this doesn't load
		}
	}

	pkTex->Release(); //Be sure to decrement reference count.

	stTextureInfoD3D tempTextureInfo;

	tempTextureInfo.m_pkTextureResource = m_pkTexture;
	tempTextureInfo.m_szFileName = a_sName;
	tempTextureInfo.m_uiReferences = 1; // start with the first reference.

	m_astLoadedTextures.push_back(tempTextureInfo);

    std::cout<<"Texture number is: "<<m_astLoadedTextures.size() - 1<<std::endl;

	SDL_FreeSurface(pkImage);

	return m_astLoadedTextures.size() - 1;
}

//Loading textures directly from SDL Surfaces doesn't support duplicate detection
int D3D11DisplayManager::LoadTextureSDLSurface(SDL_Surface* a_pkSurface)
{
	//Create texture
	D3D11_TEXTURE2D_DESC t2d;
	ZeroMemory(&t2d, sizeof(t2d));
	t2d.Width = a_pkSurface->w;
	t2d.Height = a_pkSurface->h;
	t2d.MipLevels = 1;
	t2d.ArraySize = 1;
	t2d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	t2d.SampleDesc.Count = 1;
	t2d.SampleDesc.Quality = 0;
	t2d.Usage = D3D11_USAGE_DEFAULT;
	t2d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	t2d.CPUAccessFlags = 0;
	t2d.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = a_pkSurface->pixels;
	initData.SysMemPitch = a_pkSurface->pitch;

	//Create a single texture name.
	HRESULT hr;

	ID3D11Texture2D* pkTex = nullptr;
	hr = m_pkDevice->CreateTexture2D(& t2d, &initData, &pkTex);

	//Bind the texture name that was created.
	ID3D11ShaderResourceView* m_pkTexture = nullptr;

	if( SUCCEEDED(hr) && pkTex != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        memset( &SRVDesc, 0, sizeof( SRVDesc ) );
        SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//Set some parameters
        SRVDesc.Texture2D.MipLevels = 1;

		hr = m_pkDevice->CreateShaderResourceView( pkTex, &SRVDesc, &m_pkTexture);

		if(FAILED(hr))
		{
			m_pkTexture->Release();

			return 0; // return 0 for the default texture if this doesn't load
		}
	}
	
	stTextureInfoD3D tempTextureInfo;

	tempTextureInfo.m_pkTextureResource = m_pkTexture;
	tempTextureInfo.m_szFileName = "Generated from SDL Surface";
	tempTextureInfo.m_uiReferences = 1; // start with the first reference.

	m_astLoadedTextures.push_back(tempTextureInfo);
	SDL_FreeSurface( a_pkSurface );

	//Return texture number
	int iTemp = m_astLoadedTextures.size() - 1;
	
	return iTemp;
}

void D3D11DisplayManager::UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber)
{
	//Create texture
	D3D11_TEXTURE2D_DESC t2d;
	ZeroMemory(&t2d, sizeof(t2d));
	t2d.Width = a_pkSurface->w;
	t2d.Height = a_pkSurface->h;
	t2d.MipLevels = 1;
	t2d.ArraySize = 1;
	t2d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	t2d.SampleDesc.Count = 1;
	t2d.SampleDesc.Quality = 0;
	t2d.Usage = D3D11_USAGE_DEFAULT;
	t2d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	t2d.CPUAccessFlags = 0;
	t2d.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = a_pkSurface->pixels;
	initData.SysMemPitch = a_pkSurface->pitch;

	//Create a single texture name.
	HRESULT hr;

	ID3D11Texture2D* pkTex = nullptr;
	hr = m_pkDevice->CreateTexture2D(& t2d, &initData, &pkTex);

	if( SUCCEEDED(hr) && pkTex != 0)
	{
		//Clear the current shader resource view
		m_astLoadedTextures[a_iTextureNumber].m_pkTextureResource->Release();

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        memset( &SRVDesc, 0, sizeof( SRVDesc ) );
        SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//Set some parameters
        SRVDesc.Texture2D.MipLevels = 1;

		//Create a new one.
		hr = m_pkDevice->CreateShaderResourceView( pkTex, &SRVDesc, &m_astLoadedTextures[a_iTextureNumber].m_pkTextureResource);

		if(FAILED(hr))
		{
			m_astLoadedTextures[a_iTextureNumber].m_pkTextureResource->Release();

			std::cout<<"Texture update failed!"<<std::endl; // we've got a problem. //TODO: handle this properly.

			return;
		}
	}

	pkTex->Release(); //Make sure to decrement the reference count.

	SDL_FreeSurface( a_pkSurface );

	//Return texture number
	int iTemp = m_astLoadedTextures.size() - 1;
	
	return;
}

void D3D11DisplayManager::UnloadTexture(int a_iTextureNumber)
{
	/*for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if(iDx == a_iTextureNumber)
		{
			m_astLoadedTextures[iDx].m_uiReferences--;//Reduce the references by one

			//std::cout<<"Reducing References of "<<m_astLoadedTextures[iDx].m_szFileName<<", now: "<<m_astLoadedTextures[iDx].m_uiReferences<<std::endl;

			if(m_astLoadedTextures[iDx].m_uiReferences <= 0)//only unload if this is the only reference.
			{
				//std::cout<<"Deleting Texture: "<<m_astLoadedTextures[iDx].m_szFileName<<std::endl;
				m_astLoadedTextures[iDx].m_pkTextureResource->Release();

				//TODO: Soon, must fix
				//m_astLoadedTextures.erase(m_astLoadedTextures.begin() + iDx, m_astLoadedTextures.begin() + iDx + 1);
			}
		}
	}*/
}

void D3D11DisplayManager::UnloadTexture(std::string a_szTextureFilename)
{
	/*for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if(m_astLoadedTextures[iDx].m_szFileName == a_szTextureFilename)
		{
			m_astLoadedTextures[iDx].m_uiReferences--;//Reduce the references by one

			if(m_astLoadedTextures[iDx].m_uiReferences <= 0)//only unload if this is the only reference.
			{
				m_astLoadedTextures[iDx].m_pkTextureResource->Release();

				//TODO: Soon, must fix
				//m_astLoadedTextures.erase(m_astLoadedTextures.begin() + iDx, m_astLoadedTextures.begin() + iDx + 1);
			}
		}
	}*/
}

bool D3D11DisplayManager::Update(float a_fDeltaTime)
{
    //TODO

	//Show the frame.
	m_pkSwapchain->Present(0, 0);

	//set the render target as the back buffer
	m_pkContext->OMSetRenderTargets(1, &m_pkBackBuffer, NULL);

	//Clear the back buffer for the next frame.
	
	m_pkContext->ClearRenderTargetView(m_pkBackBuffer, m_afTempClearColour);
    return true;
}

void D3D11DisplayManager::ReportLiveObjects()
{
#ifdef _DEBUG
	ID3D11Debug* DebugDevice = NULL;
	HRESULT hr = m_pkDevice->QueryInterface(__uuidof(ID3D11Debug), (void **)&DebugDevice);
	
	hr = DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	DebugDevice->Release();
#endif
}

// Transforming worldspace to screenspace, really needs to get replace, I mean, software transformations are 90's as fuck.
float D3D11DisplayManager::TransformToScreenSpaceX(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().x;
	a_pkPosition += m_iXResolution /2;
	return(float)(a_pkPosition / (m_iXResolution / 2))- 1.0f;
}

float D3D11DisplayManager::TransformToScreenSpaceY(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().y;
	a_pkPosition += m_iYResolution / 2;
	return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

float D3D11DisplayManager::HUDTransformToScreenSpaceX(double a_pkPosition)
{
	return(float)(a_pkPosition / (m_iXResolution / 2))- 1.0f;
}

float D3D11DisplayManager::HUDTransformToScreenSpaceY(double a_pkPosition)
{
	return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

void D3D11DisplayManager::SetViewMatrix(Matrix* a_pkNewMatrix)
{
	*m_pkViewMatrix = *a_pkNewMatrix;
}

void D3D11DisplayManager::SetCameraDimensions(float a_fCameraTop, float a_fCameraRight)
{
	m_fCameraTop = a_fCameraTop; m_fCameraRight = a_fCameraRight;
}

void D3D11DisplayManager::SetShaderProgram(unsigned int a_uiShaderProgramNumber)
{
	//Current D3D11 Renderer doesn't support swapping shaders.
	return;
}

bool D3D11DisplayManager::Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* a_pkTexture)
{
    /*glBindTexture(GL_TEXTURE_2D, a_pkTexture->GetTextureNumber());*/

	m_pkContext->PSSetShaderResources(0, 1, &m_astLoadedTextures[a_pkTexture->GetTextureNumber()].m_pkTextureResource);
	m_pkContext->PSSetSamplers( 0, 1, &m_pkTexSamplerState);

	//TODO: allow drawing code to handle something that's not a quad

	D3DVERTEX OurVertices[6];

	OurVertices[0].X = TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).x);
	OurVertices[0].Y = TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).y);
	OurVertices[0].Z = m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).z;
	OurVertices[0].U = a_pkTexture->m_fMinU;
	OurVertices[0].V = a_pkTexture->m_fMinV;
	
	OurVertices[1].X = TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).x);
	OurVertices[1].Y = TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).y);
	OurVertices[1].Z = m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).z;
	OurVertices[1].U = a_pkTexture->m_fMaxU;
	OurVertices[1].V = a_pkTexture->m_fMinV;
	
	OurVertices[2].X = TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).x);
	OurVertices[2].Y = TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).y);
	OurVertices[2].Z = m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).z;
	OurVertices[2].U = a_pkTexture->m_fMinU;
	OurVertices[2].V = a_pkTexture->m_fMaxV;
	
	OurVertices[3].X = TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).x);
	OurVertices[3].Y = TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).y);
	OurVertices[3].Z = m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).z;
	OurVertices[3].U = a_pkTexture->m_fMinU;
	OurVertices[3].V = a_pkTexture->m_fMaxV;
	
	OurVertices[4].X = TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).x);
	OurVertices[4].Y = TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).y);
	OurVertices[4].Z = m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).z;
	OurVertices[4].U = a_pkTexture->m_fMaxU;
	OurVertices[4].V = a_pkTexture->m_fMinV;
	
	OurVertices[5].X = TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).x);
	OurVertices[5].Y = TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).y);
	OurVertices[5].Z = m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).z;
	OurVertices[5].U = a_pkTexture->m_fMaxU;
	OurVertices[5].V = a_pkTexture->m_fMaxV;

	D3D11_MAPPED_SUBRESOURCE ms;
	m_pkContext->Map(m_pkVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	//Map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));
	m_pkContext->Unmap(m_pkVertexBuffer, NULL);

	//Select which vertex buffer to display
	UINT uiStride = sizeof(D3DVERTEX);
	UINT uiOffset = 0;
	m_pkContext->IASetVertexBuffers(0, 1, &m_pkVertexBuffer, &uiStride, &uiOffset);

	//Select which primitive type we are using
	m_pkContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw the vertex buffer to the back buffer
	m_pkContext->Draw(6, 0);

    return true;
}

bool D3D11DisplayManager::HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* a_iTexture)
{
	//Bind the textures
	m_pkContext->PSSetShaderResources(0, 1, &m_astLoadedTextures[a_iTexture->GetTextureNumber()].m_pkTextureResource);
	m_pkContext->PSSetSamplers( 0, 1, &m_pkTexSamplerState);

	//TODO: allow drawing code to handle something that's not a quad

	D3DVERTEX OurVertices[6];

	OurVertices[0].X = HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).x);
	OurVertices[0].Y = HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).y);
	OurVertices[0].Z = 0;
	OurVertices[0].U = a_iTexture->m_fMinU;
	OurVertices[0].V = a_iTexture->m_fMinV;
	
	OurVertices[1].X = HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).x);
	OurVertices[1].Y = HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).y);
	OurVertices[1].Z = 0;
	OurVertices[1].U = a_iTexture->m_fMaxU;
	OurVertices[1].V = a_iTexture->m_fMinV;
	
	OurVertices[2].X = HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).x);
	OurVertices[2].Y = HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).y);
	OurVertices[2].Z = 0;
	OurVertices[2].U = a_iTexture->m_fMinU;
	OurVertices[2].V = a_iTexture->m_fMaxV;
	
	OurVertices[3].X = HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).x);
	OurVertices[3].Y = HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).y);
	OurVertices[3].Z = 0;
	OurVertices[3].U = a_iTexture->m_fMinU;
	OurVertices[3].V = a_iTexture->m_fMaxV;
	
	OurVertices[4].X = HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).x);
	OurVertices[4].Y = HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).y);
	OurVertices[4].Z = 0;
	OurVertices[4].U = a_iTexture->m_fMaxU;
	OurVertices[4].V = a_iTexture->m_fMinV;
	
	OurVertices[5].X = HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).x);
	OurVertices[5].Y = HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).y);
	OurVertices[5].Z = 0;
	OurVertices[5].U = a_iTexture->m_fMaxU;
	OurVertices[5].V = a_iTexture->m_fMaxV;

	D3D11_MAPPED_SUBRESOURCE ms;
	m_pkContext->Map(m_pkVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	//Map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));
	m_pkContext->Unmap(m_pkVertexBuffer, NULL);

	//Select which vertex buffer to display
	UINT uiStride = sizeof(D3DVERTEX);
	UINT uiOffset = 0;
	m_pkContext->IASetVertexBuffers(0, 1, &m_pkVertexBuffer, &uiStride, &uiOffset);

	//Select which primitive type we are using
	m_pkContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw the vertex buffer to the back buffer
	m_pkContext->Draw(6, 0);

	return true;
}
#endif //ifdef _WIN32