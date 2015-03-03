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
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


D3D11DisplayManager::D3D11DisplayManager(int argc, char **argv) : DisplayManager(argc, argv)
{
    //TODO
    std::cout<<"D3D11DisplayManager Created."<<std::endl;

    m_pkViewMatrix = new Matrix();

    CreateScreen();
}

D3D11DisplayManager::~D3D11DisplayManager()
{
    //TODO
	m_pkSwapchain->SetFullscreenState(FALSE, NULL);		//Switch to windowed mode on close, y'know, because directx needs that.

	//Close and release all existing COM objects
	m_pkVertexShader->Release();
	m_pkPixelShader->Release();
	m_pkSwapchain->Release();
	m_pkBackBuffer->Release();
	m_pkDevice->Release();
	m_pkContext->Release();

    delete m_pkViewMatrix;
}

unsigned int D3D11DisplayManager::LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader)
{
	//TODO
	//Load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	//Encapsulate both shaders into shader objects
	m_pkDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pkVertexShader);
	m_pkDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pkPixelShader);

	//Set the shader objects
	m_pkContext->VSSetShader(m_pkVertexShader, 0, 0);
	m_pkContext->PSSetShader(m_pkPixelShader, 0, 0);

	//Create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	m_pkDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pkInputLayout);
	//Set it
	m_pkContext->IASetInputLayout(m_pkInputLayout);

	return 0;
}

bool D3D11DisplayManager::CreateScreen()
{
    //TODO
    if(SDL_Init(SDL_INIT_VIDEO) < 0) // Die if SDL2 Init failed
	{
		std::cout<<"Unable to initialize SDL2"<<std::endl;
		exit(1);
	}

	m_iXResolution = 1600;
	m_iYResolution = 900;

	m_pkMainWindow = SDL_CreateWindow("Pegasus Feather 0.5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_iXResolution, m_iYResolution, SDL_WINDOW_SHOWN);

	if (!m_pkMainWindow) // Die if window creation failed
	{
		std::cout<<"Window Creation Failed After Succesful SDL2 Initialization."<<std::endl;
		exit(1);
	}

	//retrieve native window handle info to pass to directX
	SDL_SysWMinfo kInfo;
	SDL_VERSION(&kInfo.version); //required before calling GetWindowWMInfo
	SDL_GetWindowWMInfo(m_pkMainWindow, &kInfo);

	//this bit is Win32-specific, but then so is D3D;
	// other OSes have different sub-structs here
	HWND kHandle = kInfo.info.win.window;

	//create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	//clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	//fill the swap chain description struct
	scd.BufferCount = 1;									//One back buffer
	scd.BufferDesc.Format =  DXGI_FORMAT_R8G8B8A8_UNORM;	//Use 32-bit colour
	scd.BufferDesc.Width = m_iXResolution;					//Set the back buffer width
	scd.BufferDesc.Height = m_iYResolution;					//Set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//How swap chain is to be used
	scd.OutputWindow = kHandle;								//The window to be used
	scd.SampleDesc.Count = 4;								//How many multisamples				<== MSAA
	scd.Windowed = TRUE;									//Windowed mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		//Allow full-screen switching

	//Create a device, device context, and swap chain using the scd struct
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &m_pkSwapchain, &m_pkDevice, NULL, &m_pkContext);

	SDL_ShowCursor(0); //Hide cursor
	// Enable V-Sync
	// Enable Transparency
	// Set Transparency settings.

	m_iDefaultTexture = LoadTexture("Resources/Textures/System/Error.png");

	//Block for detecting videocard info;

	HRESULT hr;

	ZeroMemory(&hr, sizeof(HRESULT));

	IDXGIDevice1 * pDXGIDevice;
	hr = m_pkDevice->QueryInterface(__uuidof(IDXGIDevice1), (void **)&pDXGIDevice);
      
	IDXGIAdapter1 * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter1), (void **)&pDXGIAdapter);

	DXGI_ADAPTER_DESC1 kAdapterDescription;

	pDXGIAdapter->GetDesc1(&kAdapterDescription);

	std::wstring wszDescription = std::wstring(kAdapterDescription.Description);
	std::string szDescription = std::string(wszDescription.begin(), wszDescription.end());

	std::cout<<"Window created, D3D Context: "<<std::endl
		<<"Device ID: "<<kAdapterDescription.DeviceId<<std::endl
		<<"Vendor ID: "<<kAdapterDescription.VendorId<<std::endl
		<<"VRAM: "<<kAdapterDescription.DedicatedVideoMemory / 1048576<<std::endl //divide by this number to change from bytes to megabytes
		<<"Revision: "<<kAdapterDescription.Revision<<std::endl
		<<"Description: "<<szDescription<<std::endl;
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
	m_pkContext->ClearRenderTargetView(m_pkBackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	//Setting up vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;					//Write access by CPU and GPU
	bd.ByteWidth = sizeof(D3DVERTEX) * 3;			//Size is the D3DVERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//Use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Allow CPU to write in buffer

	m_pkDevice->CreateBuffer(&bd, NULL, &m_pkVertexBuffer); //Create the buffer

	LoadShaderProgram("shaders.shader", "shaders.shader");

	D3DVERTEX OurVertices[] =
	{
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	D3D11_MAPPED_SUBRESOURCE ms;
	m_pkContext->Map(m_pkVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	//Map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));
	m_pkContext->Unmap(m_pkVertexBuffer, NULL);

    return true;
}

int D3D11DisplayManager::LoadTexture(std::string a_sName)
{
	//Check to see if texture has already been used
	/*for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if(a_sName == m_astLoadedTextures[iDx].m_szFileName)
		{
			//std::cout<<"Texture already in memory: "<<a_sName<<std::endl;
			m_astLoadedTextures[iDx].m_uiReferences++; //increase the number of references to this texture.
			return m_astLoadedTextures[iDx].m_iTextureNumber;
		}
	}

	SDL_Surface *pkImage;
    int piSize;
	piSize = PackManager::GetSizeOfFile(a_sName.c_str());
	pkImage=IMG_Load_RW(SDL_RWFromMem(PackManager::LoadResource(a_sName.c_str()), piSize), 1);
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

	int iTempTextureNumber = LoadTextureSDLSurface(pkImage);

	stTextureInfo tempTextureInfo;

	tempTextureInfo.m_iTextureNumber = iTempTextureNumber;
	tempTextureInfo.m_szFileName = a_sName;
	tempTextureInfo.m_uiReferences = 1; // start with the first reference.

	m_astLoadedTextures.push_back(tempTextureInfo);

    std::cout<<"Texture number is: "<<iTempTextureNumber<<std::endl;

	return iTempTextureNumber;*/
	return 0;
}

//Loading textures directly from SDL Surfaces doesn't support duplicate detection
int D3D11DisplayManager::LoadTextureSDLSurface(SDL_Surface* a_pkSurface)
{
	// get the number of channels in the texture to check if there's an alpha channel and also determine the format of the image
	/*GLenum texture_format = 0;
	GLint nOfColours;
	nOfColours = a_pkSurface->format->BytesPerPixel;
	if (nOfColours == 4)
	{
		if (a_pkSurface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else if (nOfColours == 3)
	{
		if(a_pkSurface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}
	else
	{
		std::cout<<"Warning: "<<a_pkSurface<<" is not a truecolour image"<<std::endl;
	}

	//Get a pointer.
	GLuint* puiTemp = new GLuint[1];

	//Create a single texture name.
	glGenTextures(1, puiTemp);

	//Bind the texture name that was created.
	glBindTexture(GL_TEXTURE_2D, *puiTemp);

	//Set some parameters (I think this is mip-mapping)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	// set the texture's stretching properties
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// set the texture object's image data using the information SDL_Surface gives us
	glTexImage2D(GL_TEXTURE_2D,		//target
                 0,						//level
                 GL_RGBA,				//Internal Format
                 a_pkSurface->w, a_pkSurface->h,					//width and height
                 0,
                 texture_format, GL_UNSIGNED_BYTE,		//format and type
                 a_pkSurface->pixels);		//data

	SDL_FreeSurface( a_pkSurface );

	//Return texture number
	int iTemp;
	iTemp = ((int)*puiTemp);
	delete puiTemp;
	return iTemp;*/
	return 0;
}

void D3D11DisplayManager::UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber)
{
	// get the number of channels in the texture to check if there's an alpha channel and also determine the format of the image
	/*GLenum texture_format = 0;
	GLint nOfColours;
	nOfColours = a_pkSurface->format->BytesPerPixel;
	if (nOfColours == 4)
	{
		if (a_pkSurface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else if (nOfColours == 3)
	{
		if(a_pkSurface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}
	else
	{
		std::cout<<"Warning: "<<a_pkSurface<<" is not a truecolour image"<<std::endl;
	}

	//Bind the texture name that was created.
	glBindTexture(GL_TEXTURE_2D, a_iTextureNumber);

	//Set some parameters (I think this is mip-mapping)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	// set the texture's stretching properties
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// set the texture object's image data using the information SDL_Surface gives us
	glTexImage2D(GL_TEXTURE_2D,		//target
                 0,						//level
                 GL_RGBA,				//Internal Format
                 a_pkSurface->w, a_pkSurface->h,					//width and height
                 0,
                 texture_format, GL_UNSIGNED_BYTE,		//format and type
                 a_pkSurface->pixels);		//data

	SDL_FreeSurface( a_pkSurface );*/
}

void D3D11DisplayManager::UnloadTexture(int a_iTextureNumber)
{
	/*for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if(m_astLoadedTextures[iDx].m_iTextureNumber == a_iTextureNumber)
		{
			m_astLoadedTextures[iDx].m_uiReferences--;//Reduce the references by one

			//std::cout<<"Reducing References of "<<m_astLoadedTextures[iDx].m_szFileName<<", now: "<<m_astLoadedTextures[iDx].m_uiReferences<<std::endl;

			if(m_astLoadedTextures[iDx].m_uiReferences <= 0)//only unload if this is the only reference.
			{
				//std::cout<<"Deleting Texture: "<<m_astLoadedTextures[iDx].m_szFileName<<std::endl;
				GLuint* puiTemp = new GLuint[1];

				puiTemp[0] = m_astLoadedTextures[iDx].m_iTextureNumber;

				glDeleteTextures(1, puiTemp);

				delete puiTemp;

				m_astLoadedTextures.erase(m_astLoadedTextures.begin() + iDx, m_astLoadedTextures.begin() + iDx + 1);
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
				GLuint* puiTemp = new GLuint[1];

				puiTemp[0] = m_astLoadedTextures[iDx].m_iTextureNumber;

				glDeleteTextures(1, puiTemp);

				delete puiTemp;

				m_astLoadedTextures.erase(m_astLoadedTextures.begin() + iDx, m_astLoadedTextures.begin() + iDx + 1);
			}
		}
	}*/
}

bool D3D11DisplayManager::Update(float a_fDeltaTime)
{
    //TODO

	//Show the frame.
	m_pkSwapchain->Present(0,0);

	//Clear the back buffer for the next frame.
    m_pkContext->ClearRenderTargetView(m_pkBackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    //Delay a little bit in order to give cpu a break.
    SDL_Delay(1);
    return true;
}
// Transforming worldspace to screenspace, really needs to get replace, I mean, software transformations are 90's as fuck.
float D3D11DisplayManager::TransformToScreenSpaceX(double a_pkPosition)
{
	/*a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().x;
	a_pkPosition += m_iXResolution /2;
	return(float)(a_pkPosition / (m_iXResolution / 2))- 1.0f;*/
}

float D3D11DisplayManager::TransformToScreenSpaceY(double a_pkPosition)
{
	/*a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().y;
	a_pkPosition += m_iYResolution / 2;
	return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));*/
}

float D3D11DisplayManager::HUDTransformToScreenSpaceX(double a_pkPosition)
{
	//return(float)(a_pkPosition / (m_iXResolution / 2))- 1.0f;
}

float D3D11DisplayManager::HUDTransformToScreenSpaceY(double a_pkPosition)
{
	//return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

void D3D11DisplayManager::SetViewMatrix(Matrix* a_pkNewMatrix)
{
	//*m_pkViewMatrix = *a_pkNewMatrix;
}

void D3D11DisplayManager::SetCameraDimensions(float a_fCameraTop, float a_fCameraRight)
{
	//m_fCameraTop = a_fCameraTop; m_fCameraRight = a_fCameraRight;
}

void D3D11DisplayManager::SetShaderProgram(unsigned int a_uiShaderProgramNumber)
{
	//Legacy OpenGL Renderer doesn't support shaders.
	return;
}

bool D3D11DisplayManager::Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* a_pkTexture)
{
    /*glBindTexture(GL_TEXTURE_2D, a_pkTexture->GetTextureNumber());

    glBegin(GL_QUADS);

	//TODO: allow drawing code to handle something that's not a quad

	glTexCoord2f(a_pkTexture->m_fMinU, a_pkTexture->m_fMinV);
    glVertex3f((GLfloat)TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).x),
              (GLfloat)TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).y),
              (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).z);

	glTexCoord2f(a_pkTexture->m_fMaxU, a_pkTexture->m_fMinV);
    glVertex3f((GLfloat)TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).x),
              (GLfloat)TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).y),
              (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).z);

	glTexCoord2f(a_pkTexture->m_fMaxU, a_pkTexture->m_fMaxV);
    glVertex3f((GLfloat)TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).x),
              (GLfloat)TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).y),
              (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).z);

	glTexCoord2f(a_pkTexture->m_fMinU, a_pkTexture->m_fMaxV);
    glVertex3f((GLfloat)TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).x),
              (GLfloat)TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).y),
              (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).z);
	glEnd();*/

	//Select which vertex buffer to display
	UINT uiStride = sizeof(D3DVERTEX);
	UINT uiOffset = 0;
	m_pkContext->IASetVertexBuffers(0, 1, &m_pkVertexBuffer, &uiStride, &uiOffset);

	//Select which primitive type we are using
	m_pkContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Draw the vertex buffer to the back buffer
	m_pkContext->Draw(3, 0);

    return true;
}

bool D3D11DisplayManager::HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* a_iTexture)
{
	/*glBindTexture(GL_TEXTURE_2D, a_iTexture->GetTextureNumber());

	glBegin(GL_TRIANGLE_STRIP);
	//Vertices
	glTexCoord2f(a_iTexture->m_fMinU,a_iTexture->m_fMinV);// Triangle 1 Top-left
	glVertex2f(HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).y)); // Triangle 1 Top-left
	glTexCoord2f(a_iTexture->m_fMaxU,a_iTexture->m_fMinV);// Top-Right
	glVertex2f(HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).y)); // Top-Right
	glTexCoord2f(a_iTexture->m_fMaxU,a_iTexture->m_fMaxV);// Triangle 2 Bottom-Right
	glVertex2f(HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).y)); // Triangle 2 Bottom-Right
	glTexCoord2f(a_iTexture->m_fMinU,a_iTexture->m_fMaxV);// Bottom-Left
	glVertex2f(HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).y)); // Bottom-Left
	glTexCoord2f(a_iTexture->m_fMinU,a_iTexture->m_fMinV);// Top-Left
	glVertex2f(HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).y)); // Top-Left

	glEnd();*/
	return true;
}

#endif //ifdef _WIN32