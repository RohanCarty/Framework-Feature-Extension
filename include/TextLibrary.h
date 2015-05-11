#ifndef _OPENGLTEXTLIBRARY_H_
#define _OPENGLTEXTLIBRARY_H_
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

#include "DisplayManager.h"

#ifdef __APPLE__
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif

#include <vector>

class Texture;
class Vector;
class Vertex;

struct stTTFInfo
{
	std::string szResourceFile;
	int iSize;
	TTF_Font* m_pkFont;
};

class TextLibrary
{
public:
	TextLibrary(DisplayManager* a_kDisplayManager);
	~TextLibrary();

	//Prints Characters, taking in the ascii code to print and the positions and sizes of the characters.
	//HUD versions use HUD Draw which are put into screen space instead of worldspace;
	//Prints Strings passed in, takes a position and a character size.
	void PrintString(std::string& sString, double x, double y, unsigned int CharacterSize);
	void PrintHUDString(std::string& sString, double x, double y, unsigned int CharacterSize);

	//Returns a vector containing the x (width) and y (height) of the passed in string at the chosen character size
	Vector GetStringSize(std::string& sString, unsigned int CharacterSize);

	stTTFInfo GetTTFBySizeAndResource(int a_iSize, std::string a_szResourceFile);

private:
	DisplayManager* m_kDisplayManager;
	Texture* m_pkTexture;
	Vertex* m_pakVerticies;

	std::vector<stTTFInfo> m_apkTTFs;

	Mesh* m_pkMesh; //for worldspace drawing instead of screenspace

	//Variables for holding the size in order to figure out how much of a surface is needed to draw
	int m_piSizeX;
	int m_piSizeY;

	unsigned int m_uiPreviousCharacterSize;
};

#endif //_OPENGLTEXTLIBRARY_H_
