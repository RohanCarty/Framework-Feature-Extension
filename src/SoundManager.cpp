#include "SoundManager.h"
#include "PackManager.h"

#include <iostream>

SoundManager::SoundManager()
{
	std::cout<<"Sound Manager starting."<<std::endl;
	Mix_Init(0);

	if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		std::cout<<"Mix_OpenAudio: "<< Mix_GetError()<<std::endl;
	}

	Mix_Music* pkMusic;

	//pkMusic = Mix_LoadMUS("Resources/Music/test2.flac");
    
	int piSize;
	delete PackManager::LoadResource("Resources/Music/lunarmarch.mp3",&piSize);
	SDL_RWops* pkMusicFile = SDL_RWFromConstMem(PackManager::LoadResource("Resources/Music/lunarmarch.mp3",&piSize), piSize);
	if(pkMusicFile == NULL)
	{
		 std::cout<<"Failed to create SDL_RWops pointer: "<<SDL_GetError()<<std::endl;
	}

	pkMusic=Mix_LoadMUS_RW(pkMusicFile, 1);

	if(pkMusic == NULL)
	{
		 std::cout<<"Failed to create pkMusic pointer: "<<Mix_GetError()<<std::endl;
	}
//    delete piSize;

	if(Mix_FadeInMusic(pkMusic, -1, 5000) == -1) 
	{
		std::cout<<"Mix_FadeInMusic: "<< Mix_GetError()<<std::endl;
	}
}

SoundManager::~SoundManager()
{
	while(m_astAudioChunks.size() > 0)
	{
		Mix_FreeChunk(m_astAudioChunks.back().m_apkAudioChunk);
		m_astAudioChunks.pop_back();
	}

	Mix_CloseAudio();
}

bool SoundManager::Update(float a_fDeltaTime)
{
	return true;
}

void SoundManager::PlaySound(std::string a_szFileName)
{
	int iReference = -1;
	for(unsigned int iDx = 0; iDx < m_astAudioChunks.size(); iDx++)
	{
		if(m_astAudioChunks[iDx].m_szFileName == a_szFileName)
		{
			iReference = iDx;
			//std::cout<<"Sound Already in Memory: "<<a_szFileName<<std::endl;
		}
	}

    //TODO: Async loading and playing of files.
    
	//if file not in list, load dat shit
	if(iReference == -1)
	{
		stSoundChunkInfo stTemp;
		stTemp.m_szFileName = a_szFileName;
        
        int piSize;
        stTemp.m_apkAudioChunk = Mix_LoadWAV_RW(SDL_RWFromMem(PackManager::LoadResource(a_szFileName.c_str(),&piSize), piSize), 1);
        
		if(!stTemp.m_apkAudioChunk) 
		{
			std::cout<<a_szFileName<<" Mix_LoadWAV: "<< Mix_GetError()<<std::endl;
			//Something fucked up, leave
			return;
		}
		m_astAudioChunks.push_back(stTemp);
		iReference = (int)m_astAudioChunks.size() - 1;
	}

	//Play it finally
	if(Mix_PlayChannel(-1, m_astAudioChunks[iReference].m_apkAudioChunk, 0) == -1) 
	{
		std::cout<<"Mix_PlayChannel: "<< Mix_GetError()<<std::endl;
	}
}