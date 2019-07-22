#ifndef _ANIMATEDTEXTURE_H_
#define _ANIMATEDTEXTURE_H_
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
#include <vector>

#include "Texture.h"

class DisplayManager;

struct TextureFrame
{
    float UMin;
    float VMin;
    float UMax;
    float VMax;
};

struct Animation
{
    std::string szName;
    std::vector<TextureFrame*> apkFrames;
    float fFrameTime; // in seconds
};

//Extension of class used to store Textures as well as all functions related to manipulating them.
//  Extensions include animations by use of UV's on a spritesheet, frames being stored in a vector with a String to determine which set to be looping through
//      This'll allow animations to be called by name
class AnimatedTexture : public Texture
{
public:
	AnimatedTexture(DisplayManager* a_pDisplayManager);
	~AnimatedTexture();

	bool Update(float a_fDeltaTime);
    
    void SwitchAnimation(std::string a_szName);

	void NextFrame();

	void SwitchFrame(unsigned int a_iNewFrame);

    void LoadTexture(std::string a_sName, DisplayManager* a_pDisplayManager);

    std::string LoadAnimation(std::string a_sAnimation, DisplayManager* a_pDisplayManager);
    
    void FlipTexture(bool a_bNewSetting);

	bool GetIsTextureFlipped();

private:
    unsigned int m_iCurrentFrame;
    std::vector<Animation*> m_apkAnimations;
    float m_fCurrentTime;
    unsigned int m_uiCurrentAnimation;
    bool m_bIsTextureFlipped;
};

#endif
