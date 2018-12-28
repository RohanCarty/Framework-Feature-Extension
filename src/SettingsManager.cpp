#include "SettingsManager.h"
#include "PackManager.h"

#include <iostream>

SettingsManager::SettingsManager()
{
	std::cout<<"Settings Manager starting."<<std::endl;

    //TODO: load XML files
    
    //fill out struct with defaults in case the XML file isn't there or if anything is missing
    m_stCurrentSettings.iXResolution = 1024;
    m_stCurrentSettings.iYResolution = 600;
    m_stCurrentSettings.bFullscreen = false;
    m_stCurrentSettings.bBorderless = false;
    m_stCurrentSettings.fMasterVolume = 1.0f;
    m_stCurrentSettings.fMusicVolume = 1.0f;
    m_stCurrentSettings.fSoundEffectVolume = 1.0f;
    m_stCurrentSettings.iSoundSampleRate = 44100;
    m_stCurrentSettings.iSoundBitDepth = 16;
    
    //otherwise, begin loading in the XML file
    
    //save out on any updates such as a return to defaults
    
    //for debug, print settings in stdout
    std::cout<<std::boolalpha<<"Printing Settings:"<<std::endl
    <<"    Resolution: "<<m_stCurrentSettings.iXResolution<<" x "<<m_stCurrentSettings.iYResolution<<std::endl
    <<"    Fullscreen: "<<m_stCurrentSettings.bFullscreen<<std::endl
    <<"    Borderless: "<<m_stCurrentSettings.bBorderless<<std::endl
    <<"    Master Volume: "<<m_stCurrentSettings.fMasterVolume<<std::endl
    <<"    Music Volume: "<<m_stCurrentSettings.fMusicVolume<<std::endl
    <<"    Sound Effect Volume: "<<m_stCurrentSettings.fSoundEffectVolume<<std::endl
    <<"    Sound Sample Rate: "<<m_stCurrentSettings.iSoundSampleRate<<std::endl
    <<"    Sound Bit Depth: "<<m_stCurrentSettings.iSoundBitDepth<<std::endl<<std::endl;
}

SettingsManager::~SettingsManager()
{
    //save out on destruction as well
}

stSettingsBlock SettingsManager::GetCurrentSettings()
{
    return m_stCurrentSettings;
}