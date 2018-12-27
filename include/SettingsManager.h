#ifndef _SETTINGSMANAGER_H_
#define _SETTINGSMANAGER_H_
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

struct stSettingsBlock
{
    int iXResolution;
    int iYResolution;
    bool bFullscreen;
    bool bBorderless;
    int iMasterVolume;
    int iMusicVolume;
    int iSoundEffectVolume;
};

class SettingsManager
{
public:
    SettingsManager();
    virtual ~SettingsManager();
    
    stSettingsBlock getCurrentSettings();
    
private:
    stSettingsBlock m_stCurrentSettings;
};

#endif
