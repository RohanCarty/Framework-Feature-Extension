#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_
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

#ifdef __APPLE__
#include <SDL2_mixer/SDL_mixer.h>
#else
#include "SDL_mixer.h"
#endif

#include <SDL.h>

#include <string>
#include <vector>

struct stSoundChunkInfo
{
	std::string m_szFileName;
	Mix_Chunk* m_apkAudioChunk;
};

class SoundManager
{
public:
    SoundManager();
    virtual ~SoundManager();

	bool Update(float a_fDeltaTime);

	void PlaySoundFile(std::string a_szFileName);
private:
	std::vector<stSoundChunkInfo> m_astAudioChunks;
};

#endif
