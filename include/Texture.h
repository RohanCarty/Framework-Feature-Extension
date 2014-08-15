#ifndef _TEXTURE_H_
#define _TEXTURE_H_
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

#include <string>

class DisplayManager;

//Class used to store Textures as well as all functions related to manipulating them.
class Texture
{
public:
	Texture(DisplayManager* a_pDisplayManager);
	virtual ~Texture();

	virtual bool Update(float a_fDeltaTime);

    virtual void LoadTexture(std::string a_sName, DisplayManager* a_pDisplayManager);

	int GetTextureNumber();
	void SetTextureNumber(int a_iTexture);

	float m_fMinU;
    float m_fMinV;
    float m_fMaxU;
    float m_fMaxV;

	bool GetIsAnimated();

protected:
	bool m_bIsAnimated;

private:
    int m_iTexture;
};

#endif
