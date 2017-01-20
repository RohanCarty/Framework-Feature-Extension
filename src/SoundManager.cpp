#include "SoundManager.h"
#include "PackManager.h"

#include <iostream>

SoundManager::SoundManager()
{
	std::cout<<"Sound Manager starting."<<std::endl;
	SDL_Init(SDL_INIT_AUDIO);

	std::string sz_temperrorstring = SDL_GetError();

	if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		std::string sz_temperrorstring = Mix_GetError();
		std::cerr<<"Mix_OpenAudio: "<< Mix_GetError()<<std::endl;
	}

	Mix_Music* pkMusic;

	std::string szMusicFileName;

	switch(rand() % 1)
	{
	case 0:
		szMusicFileName = "Sounds/Music/lunarmarch.ogg";
		break;
	case 1:
		szMusicFileName = "Sounds/Music/Broken Reality.ogg";
		break;
	case 2:
		szMusicFileName = "Sounds/Music/rollingstart.ogg";
		break;
	default:
		std::cout<<"Music Switch Default"<<std::endl;
		break;
	}

	int piSize;
	piSize = PackManager::GetSizeOfFile(szMusicFileName);

	void* pResource = PackManager::LoadResource(szMusicFileName);

	SDL_RWops* pkMusicFile = SDL_RWFromMem(pResource, piSize);

	if(pkMusicFile == NULL)
	{
		 std::cerr<<"Failed to create SDL_RWops pointer: "<<SDL_GetError()<<std::endl;
	}

	pkMusic=Mix_LoadMUS_RW(pkMusicFile, 1);

	if(pkMusic == NULL)
	{
		 std::cerr<<"Failed to create pkMusic pointer: "<<Mix_GetError()<<std::endl;
	}

	if(Mix_PlayMusic(pkMusic, -1) == -1)
	{
		std::cerr<<"Mix_FadeInMusic: "<< Mix_GetError()<<std::endl;
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

void SoundManager::PlaySoundFile(std::string a_szFileName)
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
        piSize = PackManager::GetSizeOfFile(a_szFileName.c_str());

		void* pTempResource = PackManager::LoadResource(a_szFileName.c_str());

        stTemp.m_apkAudioChunk = Mix_LoadWAV_RW(SDL_RWFromMem(pTempResource, piSize), 1);

		delete pTempResource;

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
