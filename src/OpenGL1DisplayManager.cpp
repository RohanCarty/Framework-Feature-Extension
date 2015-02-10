#include "DisplayManager.h"
#include "Vector.h"
#include "Matrix.h"
#include "Vertex.h"
#include "Texture.h"
#include "Mesh.h"
#include "PackManager.h"

#include "OpenGL1DisplayManager.h"

#ifdef __APPLE__
#include <GL/glew.h>
#include <OpenGL/OpenGL.h>
#include <SDL2_image/SDL_image.h>
#else
#include "SDL2/SDL_opengl.h"
#include "SDL_image.h"
//#include <GL/gl.h>
#endif

OpenGL1DisplayManager::OpenGL1DisplayManager(int argc, char **argv) : DisplayManager(argc, argv)
{
    //TODO
    std::cout<<"OpenGL1DisplayManager Created."<<std::endl;

    m_pkViewMatrix = new Matrix();

    CreateScreen();
}

OpenGL1DisplayManager::~OpenGL1DisplayManager()
{
    //TODO

    delete m_pkViewMatrix;
}

unsigned int OpenGL1DisplayManager::LoadShaderProgram(std::string a_szVertexShader, std::string a_szFragmentShader)
{
	//TODO
	return 0;
}

bool OpenGL1DisplayManager::CreateScreen()
{
    //TODO
    if(SDL_Init(SDL_INIT_VIDEO) < 0) // Die if SDL2 Init failed
	{
		std::cout<<"Unable to initialize SDL2"<<std::endl;
		exit(1);
	}

	m_iXResolution = 1024;
	m_iYResolution = 600;

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_pkMainWindow = SDL_CreateWindow("Pegasus Feather 0.5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_iXResolution, m_iYResolution, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!m_pkMainWindow) // Die if window creation failed
	{
		std::cout<<"Window Creation Failed After Succesful SDL2 Initialization."<<std::endl;
		exit(1);
	}

	m_kMainContext = SDL_GL_CreateContext(m_pkMainWindow);

	SDL_ShowCursor(0); //Hide cursor

	//SDL_GL_SetSwapInterval(1); // Enable V-Sync
	glEnable(GL_BLEND); // Enable Transparency
	glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );// Set Transparency settings.

	m_iDefaultTexture = LoadTexture("Resources/Textures/System/Error.png");

	std::cout<<"Window created, OpenGL Context Vendor: "<<glGetString(GL_VENDOR)<<" Renderer: "<<glGetString(GL_RENDERER)<<" Version: "<< glGetString(GL_VERSION)<<std::endl;
	std::cout<<"Window Creation Complete."<<std::endl;

    return true;
}

int OpenGL1DisplayManager::LoadTexture(std::string a_sName)
{
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

	return iTempTextureNumber;
}

//Loading textures directly from SDL Surfaces doesn't support duplicate detection
int OpenGL1DisplayManager::LoadTextureSDLSurface(SDL_Surface* a_pkSurface)
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
	return iTemp;
}

void OpenGL1DisplayManager::UpdateTextureSDLSurface(SDL_Surface* a_pkSurface, int a_iTextureNumber)
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
}

void OpenGL1DisplayManager::UnloadTexture(int a_iTextureNumber)
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

void OpenGL1DisplayManager::UnloadTexture(std::string a_szTextureFilename)
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

bool OpenGL1DisplayManager::Update(float a_fDeltaTime)
{
    //TODO
    glFlush();// using glFlush instead of glFinish in order to not wait for the video card (v-sync-ish)

	SDL_GL_SwapWindow(m_pkMainWindow); //Buffer Swap

	glClear(GL_COLOR_BUFFER_BIT);

    //Delay a little bit in order to give cpu a break.
    SDL_Delay(1);
    return true;
}
// Transforming worldspace to screenspace, really needs to get replace, I mean, software transformations are 90's as fuck.
float OpenGL1DisplayManager::TransformToScreenSpaceX(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().x;
	a_pkPosition += m_iXResolution /2;
	return(float)(a_pkPosition / (m_iXResolution / 2))- 1.0f;
}

float OpenGL1DisplayManager::TransformToScreenSpaceY(double a_pkPosition)
{
	a_pkPosition *= m_pkViewMatrix->GetTranslation().z;
	a_pkPosition -= m_pkViewMatrix->GetTranslation().y;
	a_pkPosition += m_iYResolution / 2;
	return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

float OpenGL1DisplayManager::HUDTransformToScreenSpaceX(double a_pkPosition)
{
	return(float)(a_pkPosition / (m_iXResolution / 2))- 1.0f;
}

float OpenGL1DisplayManager::HUDTransformToScreenSpaceY(double a_pkPosition)
{
	return(float)( 1.0f - (a_pkPosition / (m_iYResolution / 2)));
}

void OpenGL1DisplayManager::SetViewMatrix(Matrix* a_pkNewMatrix)
{
	*m_pkViewMatrix = *a_pkNewMatrix;

	//gluLookAt(0,0,-0.5,0,0,0,0,1,0);

	//glUniformMatrix4dv(glGetUniformLocation(m_gluiProgramObject, "in_view_matrix"), 1, true, *m_pkViewMatrix->CoOrd);
	//std::cout<<"New View Matrix Set"<<std::endl;
}

void OpenGL1DisplayManager::SetCameraDimensions(float a_fCameraTop, float a_fCameraRight)
{
	m_fCameraTop = a_fCameraTop; m_fCameraRight = a_fCameraRight;

}

void OpenGL1DisplayManager::SetShaderProgram(unsigned int a_uiShaderProgramNumber)
{
	//Legacy OpenGL Renderer doesn't support shaders.
	return;
}

bool OpenGL1DisplayManager::Draw(Mesh* a_pkMesh, int a_iSizeOfArray, Texture* a_pkTexture)
{
    glBindTexture(GL_TEXTURE_2D, a_pkTexture->GetTextureNumber());

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

	/*glTexCoord2f(a_pkTexture->m_fMinU, a_pkTexture->m_fMaxV);
    glVertex3f((GLfloat)TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[4].GetLocation()).x),
              (GLfloat)TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[4].GetLocation()).y),
              (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[4].GetLocation()).z);

	glTexCoord2f(a_pkTexture->m_fMinU, a_pkTexture->m_fMinV);
    glVertex3f((GLfloat)TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[5].GetLocation()).x),
              (GLfloat)TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[5].GetLocation()).y),
              (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[5].GetLocation()).z);*/

    /*for(int iDx = 0; iDx < a_pkMesh->GetNumberOfVertices(); iDx++)
    {
        glTexCoord2f(a_pkMesh->GetTextureUArray()[iDx], a_pkMesh->GetTextureVArray()[iDx]);
        glVertex3f((GLfloat)TransformToScreenSpaceX(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[iDx].GetLocation()).x),
                   (GLfloat)TransformToScreenSpaceY(m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[iDx].GetLocation()).y),
                   (GLfloat)m_pkViewMatrix->MultiplyVector(a_pkMesh->GetVertexArray()[iDx].GetLocation()).z);
    }*/

	glEnd();

    return true;
}

bool OpenGL1DisplayManager::HUDDraw(Vertex* a_aLocations, int a_iSizeOfArray, Texture* a_iTexture)
{
	glBindTexture(GL_TEXTURE_2D, a_iTexture->GetTextureNumber());

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

	glEnd();
	return true;
}
