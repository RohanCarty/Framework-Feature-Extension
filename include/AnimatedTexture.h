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

//Class used to store Textures as well as all functions related to manipulating them.
class AnimatedTexture : public Texture
{
public:
	AnimatedTexture(DisplayManager* a_pDisplayManager);
	~AnimatedTexture();

	bool Update(float a_fDeltaTime);

	void NextFrame();

	void SwitchFrame(unsigned int a_iNewFrame);

    void LoadTexture(std::string a_sName, DisplayManager* a_pDisplayManager);

    std::string LoadAnimation(std::string a_sAnimation, DisplayManager* a_pDisplayManager);

private:
    unsigned int m_iCurrentFrame;
    std::vector<TextureFrame*> m_vkFrames;
    float m_fCurrentTime;
    float m_fFrameTime; // in seconds
};

#endif
