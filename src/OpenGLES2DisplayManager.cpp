#include "OpenGLES2DisplayManager.h"
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
#endif

OpenGLES2DisplayManager::OpenGLES2DisplayManager(int argc, char **argv) : DisplayManager(argc, argv)
{
    std::cout<<"OpenGLES2DisplayManager Created."<<std::endl;

	CreateScreen(argc, argv);

	m_pkViewMatrix = new Matrix();

	m_akMeshData = new GLfloat[1];

	m_iCurrentMeshDataSize = 1;
}

OpenGLES2DisplayManager::~OpenGLES2DisplayManager()
{	
	std::cout << "Deleting Vertex Buffers" << std::endl;
	delete m_akVbo;
	delete m_akBuffers;

	delete m_pkViewMatrix;

	delete m_akMeshData;

	SDL_GL_DeleteContext(m_kMainContext);
	SDL_DestroyWindow(m_pkMainWindow);
	SDL_Quit();

	std::cout<<"OpenGLES2 Displaymanager closed successfully."<<std::endl;
}

unsigned int OpenGLES2DisplayManager::LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader)
{
	GLuint vertexShaderObject, fragmentShaderObject, ProgramObject;

	std::cout << "Creating empty shaders..." << std::endl;

	vertexShaderObject = glCreateShader_ptr(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader_ptr(GL_FRAGMENT_SHADER);

	std::cout << "Getting Vertex Shader Source" << std::endl;

	//Don't load shaders from file for OpenGLES2 for now, might reenable later but probably not

	//std::string szVShaderSource = LoadShader(a_szVertexShader);

	std::string szVShaderSource = "attribute vec4 a_position;\nattribute vec2 a_texCoord;\nvarying vec2 v_texCoord;\nvoid main()\n{\ngl_Position = a_position;\nv_texCoord = a_texCoord;\n}\n";

	const GLchar* vShaderSource = szVShaderSource.c_str();

	std::cout << "Getting Fragment Shader Source" << std::endl;

	//std::string szFShaderSource = LoadShader(a_szFragmentShader);

	std::string szFShaderSource = "precision mediump float;\nvarying vec2 v_texCoord;\nuniform sampler2D s_texture;\nvoid main()\n{\ngl_FragColor = texture2D( s_texture, v_texCoord );\n}\n";

	const GLchar* fShaderSource = szFShaderSource.c_str();

	std::cout<<"Shader source dump:"<<std::endl<<szVShaderSource<<std::endl<<std::endl<<szFShaderSource<<std::endl;

	if (szVShaderSource == "Error" || szFShaderSource == "Error")
	{
		return m_uiDefaultShader;
	}

	int vlength = (int)strlen(vShaderSource);
	int flength = (int)strlen(fShaderSource);

	std::cout<<"'Cat'ing string length's for the shader source files: "<<vlength<<" "<<flength<<std::endl;

	glShaderSource_ptr(vertexShaderObject, 1, &vShaderSource, &vlength);
	glShaderSource_ptr(fragmentShaderObject, 1, &fShaderSource, &flength);

	GLint blen = 0;

	//Compile vertex shader
	std::cout << "Compiling Vertex Shader Source" << std::endl;

	glCompileShader_ptr(vertexShaderObject);

	glGetShaderiv_ptr(vertexShaderObject, GL_INFO_LOG_LENGTH, &blen);

	//compile fragment shader
	std::cout << "Compiling Fragment Shader Source" << std::endl;

	glCompileShader_ptr(fragmentShaderObject);

	glGetShaderiv_ptr(fragmentShaderObject, GL_INFO_LOG_LENGTH, &blen);

	std::cout << "Creating Shader Program" << std::endl;

	ProgramObject = glCreateProgram_ptr();

	std::cout << "Attaching shaders to program" << std::endl;

	glAttachShader_ptr(ProgramObject, vertexShaderObject);
	glAttachShader_ptr(ProgramObject, fragmentShaderObject);

	std::cout << "Linking Program" << std::endl;

	glLinkProgram_ptr(ProgramObject);

	positionLoc = glGetAttribLocation_ptr(ProgramObject, "a_position");
	texCoordLoc = glGetAttribLocation_ptr(ProgramObject, "a_texCoord");

	samplerLoc = glGetUniformLocation_ptr(ProgramObject, "s_texture");

	return ProgramObject;
}

std::string OpenGLES2DisplayManager::LoadShader(std::string a_szShaderName)
{
	int piSize;
	piSize = PackManager::GetSizeOfFile(a_szShaderName);
	void* pTempResource = PackManager::LoadResource(a_szShaderName);

	std::string cpFullFile = (char*)pTempResource;
	cpFullFile.resize(piSize);

	delete pTempResource;
	return cpFullFile;
}

bool OpenGLES2DisplayManager::CreateScreen(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
		return false;
	}
	
	SDL_DisplayMode videomode;
	if (SDL_GetCurrentDisplayMode (0, &videomode) != 0)
	{
		std::cerr << "Error getting current display mode: " << SDL_GetError() << std::endl;
		return false;
	}

	m_iXResolution=1920;m_iYResolution = 1080;

	std::cout << "Current screen mode: " << videomode.w << "x" << videomode.h << std::endl;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	m_pkMainWindow = SDL_CreateWindow("Peagsus Feather",
                                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        videomode.w, videomode.h,
                                        SDL_WINDOW_OPENGL);
	if (!m_pkMainWindow)
	{
	  std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
	  return false;
	}

	m_kMainContext = SDL_GL_CreateContext(m_pkMainWindow);

	std::cout<<"GL_RENDERER: "<<glGetString(GL_RENDERER)<<std::endl;
	std::cout<<"GL_SHADING_LANGUAGE_VERSION: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
	std::cout<<"GL_VERSION: "<<glGetString(GL_VERSION)<<std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	SDL_ShowCursor(0); //Hide cursor

	//glEnable(GL_DEPTH); // Depth not supported on OpenGLES
	//glEnable(GL_BLEND); // Enable Transparency
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// Set Transparency settings.

	m_akVbo = new GLuint[1];
	m_akBuffers = new GLuint[1];

	if (!SetupExtensions()) //quit if missing any required extensions
	{
		std::cout << "Missing required extensions..." << std::endl;
		return false;
	}

	//std::cout<<"Filling Array Buffer."<<std::endl;
	
	std::cout << "Loading Shader" << std::endl;

	GLuint ProgramObject;

	ProgramObject = LoadShaderProgram("Resources/Shaders/System/GLES2texturevert.glsl", "Resources/Shaders/System/GLES2texturefrag.glsl");

	std::cout << "Using created shader" << std::endl;

	glUseProgram_ptr(ProgramObject);

	SDL_GL_SwapWindow(m_pkMainWindow);

	m_gluiProgramObject = ProgramObject;

	glViewport(0,0,videomode.w,videomode.h);

	//Set default shader
	m_uiDefaultShader = ProgramObject;

	//Set default Texture
	m_iDefaultTexture = CreateSimpleTexture2D();

	return true;
}

bool OpenGLES2DisplayManager::SetupExtensions()
{
	glCreateShader_ptr = (glCreateShader_Func)SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource_ptr = (glShaderSource_Func)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader_ptr = (glCompileShader_Func)SDL_GL_GetProcAddress("glCompileShader");
	glGetShaderiv_ptr = (glGetShaderiv_Func)SDL_GL_GetProcAddress("glGetShaderiv");
	glCreateProgram_ptr = (glCreateProgram_Func)SDL_GL_GetProcAddress("glCreateProgram");
	glAttachShader_ptr = (glAttachShader_Func)SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram_ptr = (glLinkProgram_Func)SDL_GL_GetProcAddress("glLinkProgram");
	glUseProgram_ptr = (glUseProgram_Func)SDL_GL_GetProcAddress("glUseProgram");
	glGenBuffers_ptr = (glGenBuffers_Func)SDL_GL_GetProcAddress("glGenBuffers");
	glBindBuffer_ptr = (glBindBuffer_Func)SDL_GL_GetProcAddress("glBindBuffer");
	glBufferData_ptr = (glBufferData_Func)SDL_GL_GetProcAddress("glBufferData");
	glGetAttribLocation_ptr = (glGetAttribLocation_Func)SDL_GL_GetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray_ptr = (glEnableVertexAttribArray_Func)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer_ptr = (glVertexAttribPointer_Func)SDL_GL_GetProcAddress("glVertexAttribPointer");
	glGetUniformLocation_ptr = (glGetUniformLocation_Func)SDL_GL_GetProcAddress("glGetUniformLocation");
	glActiveTexture_ptr = (glActiveTexture_Func)SDL_GL_GetProcAddress("glActiveTexture");
	glUniform1i_ptr = (glUniform1i_Func)SDL_GL_GetProcAddress("glUniform1i");
	

	return true;
}

int OpenGLES2DisplayManager::GetEmptyTextureNumber()
{
	//Get a pointer.
	GLuint* puiTemp = new GLuint[1];

	//Create a single texture name.
	glGenTextures(1, puiTemp);

	//Bind the texture name that was created.
	glBindTexture(GL_TEXTURE_2D, *puiTemp);

	//Set some parameters (I think this is mip-mapping)
	glTexParameteri(GL_TEXTURE_2D, 0, 0);
	glTexParameteri(GL_TEXTURE_2D, 1000, 0);

	//Return texture number
	int iTemp;
	iTemp = *puiTemp;
	delete puiTemp;
	return iTemp;
}

unsigned int OpenGLES2DisplayManager::CreateSimpleTexture2D()
{
	GLuint textureId;

	GLubyte pixels [4*3] = {
	255, 0, 0,
	0, 255, 0,
	0, 0, 255,
	255, 255, 0
	};

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return textureId;
}

int OpenGLES2DisplayManager::LoadTexture(std::string a_sName)
{
	//Check to see if texture has already been used
	for (unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if (a_sName == m_astLoadedTextures[iDx].m_szFileName)
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
	if (pkImgBufferTemp == NULL)
	{
		//std::cout<<"IMG_Load failue: "<<IMG_GetError()<<std::endl;
		std::cout << "SDL_RWOps failue Size:" << piSize << " : " << SDL_GetError() << std::endl;
	}
	pkImage = IMG_Load_RW(pkImgBufferTemp, 1);
	//pkImage=SDL_LoadBMP(a_sName.c_str());
	if (pkImage == NULL)
	{
		//std::cout<<"IMG_Load failue: "<<IMG_GetError()<<std::endl;
		std::cout << "IMG_Load failue: " << a_sName << ": " << SDL_GetError() << std::endl;
		// handle error
		return m_iDefaultTexture;
	}

	// Check image dimensions are powers of 2
	if ((pkImage->w & (pkImage->w - 1)) != 0)
	{
		std::cout << "Warning: " << a_sName << "'s width is not a power of 2." << std::endl;
	}
	if ((pkImage->h & (pkImage->h - 1)) != 0)
	{
		std::cout << "Warning: " << a_sName << "'s height is not a power of 2." << std::endl;
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
int OpenGLES2DisplayManager::LoadTextureSDLSurface(SDL_Surface* a_pkSurface)
{
	// OpenGLES doesn't support texture swizzling
	GLenum texture_format = 0;
	texture_format = GL_RGBA;

	//Get a pointer.
	GLuint* puiTemp = new GLuint[1];

	//Create a single texture name.
	glGenTextures(1, puiTemp);

	//Bind the texture name that was created.
	glBindTexture(GL_TEXTURE_2D, *puiTemp);

	//Set some parameters (I think this is mip-mapping)
	glTexParameteri(GL_TEXTURE_2D, 0, 0);
	glTexParameteri(GL_TEXTURE_2D, 1000, 0);

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

	SDL_FreeSurface(a_pkSurface);

	//Return texture number
	int iTemp;
	iTemp = *puiTemp;
	delete puiTemp;
	return iTemp;
}

void OpenGLES2DisplayManager::UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber)
{
	// OpenGLES doesn't support texture swizzling
	GLenum texture_format = 0;
	texture_format = GL_RGBA;

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

	SDL_FreeSurface(a_pkSurface);
}

void OpenGLES2DisplayManager::UnloadTexture(int a_iTextureNumber)
{
	for (unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if (m_astLoadedTextures[iDx].m_iTextureNumber == a_iTextureNumber)
		{
			m_astLoadedTextures[iDx].m_uiReferences--;//Reduce the references by one

													  //std::cout<<"Reducing References of "<<m_astLoadedTextures[iDx].m_szFileName<<", now: "<<m_astLoadedTextures[iDx].m_uiReferences<<std::endl;

			if (m_astLoadedTextures[iDx].m_uiReferences <= 0)//only unload if this is the only reference.
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

void OpenGLES2DisplayManager::UnloadTexture(std::string a_szTextureFilename)
{
	for (unsigned int iDx = 0; iDx < m_astLoadedTextures.size(); iDx++)
	{
		if (m_astLoadedTextures[iDx].m_szFileName == a_szTextureFilename)
		{
			m_astLoadedTextures[iDx].m_uiReferences--;//Reduce the references by one

			if (m_astLoadedTextures[iDx].m_uiReferences <= 0)//only unload if this is the only reference.
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

bool OpenGLES2DisplayManager::Update(float a_fDeltaTime)
{
	SDL_GL_SwapWindow(m_pkMainWindow);

	glClear(GL_COLOR_BUFFER_BIT);

    //Delay a little bit in order to give cpu a break.
    //SDL_Delay(1);
	return true;
}

// Transforming worldspace to screenspace, really needs to get replace, I mean, software transformations are 90's as fuck.
float OpenGLES2DisplayManager::TransformToScreenSpaceX(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().x;
	a_pkPosition += m_iXResolution / 2;
	return(float)(a_pkPosition / (m_iXResolution / 2)) - 1.0f;
}

float OpenGLES2DisplayManager::TransformToScreenSpaceY(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().y;
	a_pkPosition += m_iYResolution / 2;
	return(float)(1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

float OpenGLES2DisplayManager::HUDTransformToScreenSpaceX(double a_pkPosition)
{
	return(float)(a_pkPosition / (m_iXResolution / 2)) - 1.0f;
}

float OpenGLES2DisplayManager::HUDTransformToScreenSpaceY(double a_pkPosition)
{
	return(float)(1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

void OpenGLES2DisplayManager::SetViewMatrix(Matrix* a_pkNewMatrix)
{
	*m_pkViewMatrix = *a_pkNewMatrix;
}

void OpenGLES2DisplayManager::SetCameraDimensions(float a_fCameraTop, float a_fCameraRight)
{
	m_fCameraTop = a_fCameraTop; m_fCameraRight = a_fCameraRight;
}

void OpenGLES2DisplayManager::SetShaderProgram(unsigned int a_uiShaderProgramNumber)
{
	glUseProgram_ptr(a_uiShaderProgramNumber);
	return;
}

bool OpenGLES2DisplayManager::HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* a_pkTexture)
{
	glUseProgram_ptr(m_uiDefaultShader);

	glActiveTexture_ptr(GL_TEXTURE0);

	//glBindTexture(GL_TEXTURE_2D, a_pkTexture->GetTextureNumber());
	glBindTexture(GL_TEXTURE_2D, m_iDefaultTexture);

	GLfloat triangledata[]= {
		//Vertices
		HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).y), 0.0f, // Triangle 1 Top-left
		HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[1].GetLocation()).y), 0.0f, // Top-Right
		HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).y), 0.0f, // Bottom-Right
		HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[2].GetLocation()).y), 0.0f, // Triangle 2 Bottom-Right
		HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[3].GetLocation()).y), 0.0f, // Bottom-Left
		HUDTransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).x),HUDTransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_aLocations[0].GetLocation()).y), 0.0f // Top-Left
	};

	GLfloat texturedata[]= {
	//UV's
	a_pkTexture->m_fMinU,a_pkTexture->m_fMinV,// Triangle 1 Top-left
	a_pkTexture->m_fMaxU,a_pkTexture->m_fMinV,// Top-Right
	a_pkTexture->m_fMaxU,a_pkTexture->m_fMaxV,// Bottom-Right
	a_pkTexture->m_fMaxU,a_pkTexture->m_fMaxV,// Triangle 2 Bottom-Right
	a_pkTexture->m_fMinU,a_pkTexture->m_fMaxV,// Bottom-Left
	a_pkTexture->m_fMinU,a_pkTexture->m_fMinV // Top-Left
	};


	//Setup vertex attributes
	glVertexAttribPointer_ptr(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, triangledata);
	glEnableVertexAttribArray_ptr(positionLoc);
	glVertexAttribPointer_ptr(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texturedata);
	glEnableVertexAttribArray_ptr(texCoordLoc);

	glUniform1i_ptr(samplerLoc, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	return true;
}

bool OpenGLES2DisplayManager::Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* a_pkTexture)
{
	glUseProgram_ptr(m_uiDefaultShader);

	glActiveTexture_ptr(GL_TEXTURE0);

	//glBindTexture(GL_TEXTURE_2D, a_pkTexture->GetTextureNumber());
	glBindTexture(GL_TEXTURE_2D, m_iDefaultTexture);

	GLfloat MeshData[18] = {
	TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).x), TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).y), 0.0f,
	TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).x), TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[1].GetLocation()).y), 0.0f,
	TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).x), TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).y), 0.0f,
	TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).x), TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[2].GetLocation()).y), 0.0f,
	TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).x), TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[3].GetLocation()).y), 0.0f,
	TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).x), TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[0].GetLocation()).y), 0.0f
};

	GLfloat texturedata[] = {
		//UV's
		a_pkTexture->m_fMinU,a_pkTexture->m_fMinV,// Triangle 1 Top-left
		a_pkTexture->m_fMaxU,a_pkTexture->m_fMinV,// Top-Right
		a_pkTexture->m_fMaxU,a_pkTexture->m_fMaxV,// Bottom-Right
		a_pkTexture->m_fMaxU,a_pkTexture->m_fMaxV,// Triangle 2 Bottom-Right
		a_pkTexture->m_fMinU,a_pkTexture->m_fMaxV,// Bottom-Left
		a_pkTexture->m_fMinU,a_pkTexture->m_fMinV // Top-Left
	};

	/*GLfloat vertices[] = {0.0f, 0.5f, 0.0f,
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f};*/
	
	glVertexAttribPointer_ptr(positionLoc,3, GL_FLOAT, GL_FALSE, 0, MeshData);
	glEnableVertexAttribArray_ptr(positionLoc);

	glVertexAttribPointer_ptr(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texturedata);
	glEnableVertexAttribArray_ptr(texCoordLoc);

	glUniform1i_ptr(samplerLoc, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	return true;
}
