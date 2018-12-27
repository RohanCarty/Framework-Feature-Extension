#include "SettingsManager.h"
#include "PackManager.h"

#include <iostream>

SettingsManager::SettingsManager()
{
	std::cout<<"Settings Manager starting."<<std::endl;

    //TODO: load XML files
    
    //fill out struct with defaults in case the XML file isn't there or if anything is missing
    m_stCurrentSettings.iXResolution = 1280;
    m_stCurrentSettings.iYResolution = 720;
    m_stCurrentSettings.bFullscreen = false;
    m_stCurrentSettings.bBorderless = false;
    m_stCurrentSettings.iMasterVolume = 100;
    m_stCurrentSettings.iMusicVolume = 100;
    m_stCurrentSettings.iSoundEffectVolume = 100;
    
    //otherwise, begin loading in the XML file
    
    //save out on any updates such as a return to defaults
    
    //for debug, print settings in stdout
    std::cout<<std::boolalpha<<"Printing Settings:"<<std::endl
    <<"    Resolution: "<<m_stCurrentSettings.iXResolution<<" x "<<m_stCurrentSettings.iYResolution<<std::endl
    <<"    Fullscreen: "<<m_stCurrentSettings.bFullscreen<<std::endl
    <<"    bBorderless: "<<m_stCurrentSettings.bBorderless<<std::endl
    <<"    iMasterVolume: "<<m_stCurrentSettings.iMasterVolume<<std::endl
    <<"    iMusicVolume: "<<m_stCurrentSettings.iMusicVolume<<std::endl
    <<"    iSoundEffectVolume: "<<m_stCurrentSettings.iSoundEffectVolume<<std::endl<<std::endl;
}

SettingsManager::~SettingsManager()
{
    //save out on destruction as well
}
