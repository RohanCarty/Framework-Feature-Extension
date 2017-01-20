#ifndef __arm__
#ifndef __UWP__

#include "OpenGLDisplayManager.h"
#include "Vector.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Texture.h"
#include "PackManager.h"

#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#else
#include "SDL_image.h"
//#include <GL/gl.h>
#endif

OpenGLDisplayManager::OpenGLDisplayManager(int argc, char **argv) : DisplayManager(argc, argv)
{
    std::cout<<"OpenGLDisplayManager Created."<<std::endl;

    CreateScreen(argc, argv);

	m_pkViewMatrix = new Matrix();

	m_akMeshData = new GLfloat[1];

	m_iCurrentMeshDataSize = 1;
}

OpenGLDisplayManager::~OpenGLDisplayManager()
{
	//TODO: Actually clean up everything created by the display manager.
	std::cout<<"Deleting Vertex Buffers"<<std::endl;
	delete m_akVao;
	delete m_akBuffers;

	delete m_pkViewMatrix;

	delete m_akMeshData;

	//Delete our opengl context, destroy our window, and shutdown SDL
	std::cout<<"Deleting OpenGL Context"<<std::endl;
    SDL_GL_DeleteContext(m_kMainContext);
	std::cout<<"Closing Window"<<std::endl;
    SDL_DestroyWindow(m_pkMainWindow);
	std::cout<<"Closing SDL2"<<std::endl;
    SDL_Quit();

	std::cout<<"OpenGL Displaymanager closed successfully."<<std::endl;
}

unsigned int OpenGLDisplayManager::LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader)
{
	GLuint vertexShaderObject, fragmentShaderObject, ProgramObject;

	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	std::string szVShaderSource = LoadShader(a_szVertexShader);

	const GLchar* vShaderSource = szVShaderSource.c_str();

	std::string szFShaderSource = LoadShader(a_szFragmentShader);

	const GLchar* fShaderSource = szFShaderSource.c_str();

	if(szVShaderSource == "Error" || szFShaderSource == "Error")
	{
		return m_uiDefaultShader;
	}

	int vlength = (int)strlen(vShaderSource);
	int flength = (int)strlen(fShaderSource);

	//std::cout<<"'Cat'ing string length's for the shader source files: "<<vlength<<" "<<flength<<std::endl;

	glShaderSource(vertexShaderObject, 1, &vShaderSource, &vlength);
	glShaderSource(fragmentShaderObject, 1, &fShaderSource, &flength);

	GLint blen = 0;
	GLsizei slen = 0;

	//Compile vertex shader
	glCompileShader(vertexShaderObject);

	glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH , &blen);

	if (blen > 1)
	{
	GLchar* compiler_log = (GLchar*)malloc(blen);

	glGetInfoLogARB(vertexShaderObject, blen, &slen, compiler_log);
	std::cout <<a_szVertexShader<<" compiler_log:"<<compiler_log;
	free (compiler_log);
	}

	//compile fragment shader
	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH , &blen);

	if (blen > 1)
	{
	GLchar* compiler_log = (GLchar*)malloc(blen);

	glGetInfoLogARB(fragmentShaderObject, blen, &slen, compiler_log);
	std::cout<<a_szFragmentShader<<" compiler_log:"<<compiler_log;
	free (compiler_log);
	}

	ProgramObject = glCreateProgram();

	glAttachShader(ProgramObject, vertexShaderObject);
	glAttachShader(ProgramObject, fragmentShaderObject);

	glLinkProgram(ProgramObject);

	return ProgramObject;
}

std::string OpenGLDisplayManager::LoadShader(std::string a_szShaderName)
{
	int piSize;
	piSize = PackManager::GetSizeOfFile(a_szShaderName);
	void* pTempResource = PackManager::LoadResource(a_szShaderName);

	std::string cpFullFile = (char*)pTempResource;
	cpFullFile.resize(piSize);

	delete pTempResource;
	return cpFullFile;
}

bool OpenGLDisplayManager::CreateScreen(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) // Die if SDL2 Init failed
	{
		std::cout<<"Unable to initialize SDL2"<<std::endl;
		exit(1);
	}

	m_iXResolution = 1600;
	m_iYResolution = 900;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_pkMainWindow = SDL_CreateWindow("Pegasus Feather 0.5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_iXResolution, m_iYResolution, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!m_pkMainWindow) // Die if window creation failed
	{
		std::cout<<"Window Creation Failed After Succesful SDL2 Initialization."<<std::endl;
		exit(1);
	}

	m_kMainContext = SDL_GL_CreateContext(m_pkMainWindow);


	glewExperimental = true;

	if(glewInit())
	{
		std::cerr<<"Unable to initialize GLEW ... exiting"<<std::endl;
		exit(EXIT_FAILURE);
	}

	SDL_ShowCursor(0); //Hide cursor

	//SDL_GL_SetSwapInterval(1); // Enable V-Sync
	glEnable(GL_DEPTH); // Enable Depth
	glEnable(GL_BLEND); // Enable Transparency
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );// Set Transparency settings.

	std::cout<<"Window created, OpenGL Context Vendor: "<<glGetString(GL_VENDOR)<<" Renderer: "<<glGetString(GL_RENDERER)<<" Version: "<< glGetString(GL_VERSION)<<std::endl;

	m_akVao = new GLuint[1];
	m_akBuffers = new GLuint[1];

//	std::cout<<"Creating Vertex Buffer"<<std::endl;

	glGenVertexArrays(1, m_akVao);
//	std::cout<<"Binding Vertex Buffer"<<std::endl;
	glBindVertexArray(m_akVao[0]);

	//std::cout<<"Defining Triangles"<<std::endl;

	GLfloat vertices[6][2] = {
	{-0.50f,-0.50f }, // Triangle 1
	{ 0.50f,-0.50f },
	{ 0.50f, 0.50f },
	{ 0.50f, 0.50f }, // Triangle 2
	{-0.50f, 0.50f },
	{-0.50f,-0.50f }
	};

	//std::cout<<"Creating Array Buffer."<<std::endl;

	glGenBuffers(1, m_akBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_akBuffers[0]);

	//std::cout<<"Filling Array Buffer."<<std::endl;

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	//std::cout<<"Loading Shader"<<std::endl;

	GLuint ProgramObject;

	ProgramObject = LoadShaderProgram("Resources/Shaders/System/texturevert.glsl","Resources/Shaders/System/texturefrag.glsl");

	glUseProgram(ProgramObject);

	m_gluiProgramObject = ProgramObject;

	//Set default shader
	m_uiDefaultShader = ProgramObject;

	//Set default Texture
	m_iDefaultTexture = LoadTexture("Resources/Textures/System/Error.png");

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	std::cout<<"Window Creation Complete."<<std::endl;

    return true;
}

int OpenGLDisplayManager::GetEmptyTextureNumber()
{
	//Get a pointer.
	GLuint* puiTemp = new GLuint[1];

	//Create a single texture name.
	glGenTextures(1, puiTemp);

	//Bind the texture name that was created.
	glBindTexture(GL_TEXTURE_2D, *puiTemp);

	//Set some parameters (I think this is mip-mapping)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	//Return texture number
	int iTemp;
	iTemp = *puiTemp;
	delete puiTemp;
	return iTemp;
}

int OpenGLDisplayManager::LoadTexture(std::string a_sName)
{
	//TODO: Use SDL2_image again

	//Check to see if texture has already been used
	for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if(a_sName == m_astLoadedTextures[iDx].m_szFileName)
		{
			//std::cout<<"Texture already in memory: "<<a_sName<<std::endl;
			m_astLoadedTextures[iDx].m_uiReferences++; //increase the number of references to this texture.
			return m_astLoadedTextures[iDx].m_iTextureNumber;
		}
	}

	SDL_Surface *pkImage;
	int piSize = 0;
	//Hacky as fuck
	piSize = PackManager::GetSizeOfFile(a_sName.c_str());
	SDL_RWops* pkImgBufferTemp = SDL_RWFromMem(PackManager::LoadResource(a_sName.c_str()), piSize);
	if(pkImgBufferTemp == NULL)
	{
		//std::cout<<"IMG_Load failue: "<<IMG_GetError()<<std::endl;
		std::cout<<"SDL_RWOps failue Size:"<<piSize<<" : "<<SDL_GetError()<<std::endl;
	}
	pkImage=IMG_Load_RW(pkImgBufferTemp, 1);
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

	return iTempTextureNumber;
}

//Loading textures directly from SDL Surfaces doesn't support duplicate detection
int OpenGLDisplayManager::LoadTextureSDLSurface(SDL_Surface* a_pkSurface)
{
	// get the number of channels in the texture to check if there's an alpha channel and also determine the format of the image
	GLenum texture_format = 0;
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

	// set the texture's stretching properties // Texture filtering, makes pixel art game look kinda trash
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

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
	iTemp = *puiTemp;
	delete puiTemp;
	return iTemp;
}

void OpenGLDisplayManager::UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber)
{
	// get the number of channels in the texture to check if there's an alpha channel and also determine the format of the image
	GLenum texture_format = 0;
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

	// set the texture object's image data using the information SDL_Surface gives us
	glTexImage2D(GL_TEXTURE_2D,		//target
			0,						//level
			GL_RGBA,				//Internal Format
			a_pkSurface->w, a_pkSurface->h,					//width and height
			0,
			texture_format, GL_UNSIGNED_BYTE,		//format and type
			a_pkSurface->pixels);		//data

	SDL_FreeSurface( a_pkSurface );
}

void OpenGLDisplayManager::UnloadTexture(int a_iTextureNumber)
{
	for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
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
	}
}

void OpenGLDisplayManager::UnloadTexture(std::string a_szTextureFilename)
{
	for(unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
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
	}
}

bool OpenGLDisplayManager::Update(float a_fDeltaTime)
{
	glFlush();// using glFlush instead of glFinish in order to not wait for the video card (v-sync-ish)

	SDL_GL_SwapWindow(m_pkMainWindow); //Buffer Swap

	glClear(GL_COLOR_BUFFER_BIT);
    //Delay a little bit in order to give cpu a break.
    SDL_Delay(1);
	return true;
}

// Transforming worldspace to screenspace, really needs to get replace, I mean, software transformations are 90's as fuck.
float OpenGLDisplayManager::TransformToScreenSpaceX(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().x;
	a_pkPosition += m_iXResolution /2;
	return(float)(a_pkPosition / (m_iXResolution / 2)) - 1.0f;
	//return a_pkPosition;
}

float OpenGLDisplayManager::TransformToScreenSpaceY(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().y;
	a_pkPosition += m_iYResolution / 2;
	return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));
	//return a_pkPosition;
}

float OpenGLDisplayManager::HUDTransformToScreenSpaceX(double a_pkPosition)
{
	return(float)(a_pkPosition / (m_iXResolution / 2))- 1.0f;
}

float OpenGLDisplayManager::HUDTransformToScreenSpaceY(double a_pkPosition)
{
	return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

void OpenGLDisplayManager::SetViewMatrix(Matrix* a_pkNewMatrix)
{
	*m_pkViewMatrix = *a_pkNewMatrix;

	//gluLookAt(0,0,-0.5,0,0,0,0,1,0);

	//glUniformMatrix4dv(glGetUniformLocation(m_gluiProgramObject, "in_view_matrix"), 1, true, *m_pkViewMatrix->CoOrd);
	//std::cout<<"New View Matrix Set"<<std::endl;
}

void OpenGLDisplayManager::SetCameraDimensions(float a_fCameraTop, float a_fCameraRight)
{
	m_fCameraTop = a_fCameraTop; m_fCameraRight = a_fCameraRight;
}

void OpenGLDisplayManager::SetShaderProgram(unsigned int a_uiShaderProgramNumber)
{
	glUseProgram(a_uiShaderProgramNumber);
	return;
}

bool OpenGLDisplayManager::HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* a_pkTexture)
{
	glBindTexture(GL_TEXTURE_2D, a_pkTexture->GetTextureNumber());

	GLfloat triangledata[10][2] = {
	//Vertices
	{HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).y)}, // Triangle 1 Top-left
	{HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).y)}, // Top-Right
	{HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).y)}, // Triangle 2 Bottom-Right
	{HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).y)}, // Bottom-Left
	{HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).y)}, // Top-Left
	//UV's
	{a_pkTexture->m_fMinU,a_pkTexture->m_fMinV},// Triangle 1 Top-left
	{a_pkTexture->m_fMaxU,a_pkTexture->m_fMinV},// Top-Right
	{a_pkTexture->m_fMaxU,a_pkTexture->m_fMaxV},// Triangle 2 Bottom-Right
	{a_pkTexture->m_fMinU,a_pkTexture->m_fMaxV},// Bottom-Left
	{a_pkTexture->m_fMinU,a_pkTexture->m_fMinV} // Top-Left
	};

	//Setup vertex attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(10 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_akBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangledata), triangledata, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    return true;
}

bool OpenGLDisplayManager::Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* a_pkTexture)
{
	glBindTexture(GL_TEXTURE_2D, a_pkTexture->GetTextureNumber());

	//if array is too small, reallocate the array // Always times sizes by five, three for X/Y/Z and two for U/V
	if(a_pkMesh->GetNumberOfVertices() * 5 > m_iCurrentMeshDataSize)
	{
		delete m_akMeshData;
		m_akMeshData = new GLfloat[a_pkMesh->GetNumberOfVertices() * 5];

		m_iCurrentMeshDataSize = a_pkMesh->GetNumberOfVertices() * 5;
	}

	//Mesh vertex counter
	int iDy = 0;

	//TODO: Not needing to transform shit to screenspace, reeks of fail.

	for(int iDx = 0; iDx < a_pkMesh->GetNumberOfVertices() * 3; iDx += 3)
	{
		//X
		m_akMeshData[iDx] = TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[iDy].GetLocation()).x);
		//Y
		m_akMeshData[iDx + 1] = TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[iDy].GetLocation()).y);
		//Z
		m_akMeshData[iDx + 2] = (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[iDy].GetLocation()).z;
		//Make sure to increment the model;
		iDy++;
	}

	//Mesh vertex Counter
	iDy = 0;
	//iDy = a_pkMesh->GetNumberOfVertices() * 3;
	//TODO: Unhack from four vertex things only.

	for(int iDx = a_pkMesh->GetNumberOfVertices() * 3; iDx < (a_pkMesh->GetNumberOfVertices() * 3) + (a_pkMesh->GetNumberOfVertices() * 2); iDx += 2)
	{
		//U
		m_akMeshData[iDx] = a_pkMesh->GetTextureUArray()[iDy];/* * a_pkTexture->m_fMinV;*/
		//V
		m_akMeshData[iDx + 1] = a_pkMesh->GetTextureVArray()[iDy];/* * a_pkTexture->m_fMaxV;*/
		iDy++;
	}

	/*m_akMeshData[iDy] = a_pkTexture->m_fMinU;
	m_akMeshData[iDy+1] = a_pkTexture->m_fMinV;
	m_akMeshData[iDy+2] = a_pkTexture->m_fMaxU;
	m_akMeshData[iDy+3] = a_pkTexture->m_fMinV;
	m_akMeshData[iDy+4] = a_pkTexture->m_fMaxU;
	m_akMeshData[iDy+5] = a_pkTexture->m_fMaxV;

	m_akMeshData[iDy+6] = a_pkTexture->m_fMinU;
	m_akMeshData[iDy+7] = a_pkTexture->m_fMaxV;*/
	/*m_akMeshData[iDy+8] = a_pkTexture->m_fMinU;
	m_akMeshData[iDy+9] = a_pkTexture->m_fMaxV;
	m_akMeshData[iDy+10] = a_pkTexture->m_fMinU;
	m_akMeshData[iDy+11] = a_pkTexture->m_fMinV;*/
		/*m_akMeshData[iDx] = a_pkTexture->m_fMinU;
		m_akMeshData[iDx + 2] = a_pkTexture->m_fMinV;
		m_akMeshData[iDx + 3] = a_pkTexture->m_fMaxU;
		m_akMeshData[iDx + 1] = a_pkTexture->m_fMaxV;*/
		//Make sure to increment the model;
	/*	iDy++;
	}*/


	/*for(int iDx = a_pkMesh->GetNumberOfVertices() * 3; iDx < a_pkMesh->GetNumberOfVertices() * 5; iDx++)
	{
		std::cout<<"Mesh Data "<<iDx<<": "<<m_akMeshData[iDx]<<std::endl;
	}*/

	//Setup vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * (a_pkMesh->GetNumberOfVertices() * 3)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_akBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * a_pkMesh->GetNumberOfVertices() * 5, m_akMeshData, GL_DYNAMIC_DRAW);

	if(m_bWireframeMode)
	{
		glDrawArrays(GL_LINES, 0, a_pkMesh->GetNumberOfVertices());
	}
	else
	{
		glDrawArrays(GL_QUADS, 0, a_pkMesh->GetNumberOfVertices());
	}

    return true;
}

#endif //__UWP__
#endif //__arm__