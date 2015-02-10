#include "AnimatedTexture.h"
#include "DisplayManager.h"
#include "PackManager.h"

#include <fstream>
#include <cstdlib>

//Texture Functions
AnimatedTexture::AnimatedTexture(DisplayManager* a_pDisplayManager) : Texture(a_pDisplayManager)
{
    m_iCurrentFrame = 0;
    m_fCurrentTime = 0.0f;

    m_uiCurrentAnimation = 0;
    
	m_bIsAnimated = true;
    
    m_bIsTextureFlipped = false;
}

AnimatedTexture::~AnimatedTexture()
{
    for(int iDx = 0; iDx < m_apkAnimations.size();iDx++)
    {
        for(unsigned int i = 0; i < m_apkAnimations[iDx]->apkFrames.size(); i++)
        {
            delete m_apkAnimations[iDx]->apkFrames[i];
        }
        
        delete m_apkAnimations[iDx];
    }
}

bool AnimatedTexture::Update(float a_fDeltaTime)
{
	if(m_apkAnimations.size() == 0 || m_apkAnimations[m_uiCurrentAnimation]->apkFrames.size() == 0) // zero frames
	{
		return true;
	}

    m_fCurrentTime += a_fDeltaTime;

    if(m_fCurrentTime > m_apkAnimations[m_uiCurrentAnimation]->fFrameTime)
    {
        NextFrame();
        m_fCurrentTime = 0.0f;
    }

    return true;
}

void AnimatedTexture::SwitchAnimation(std::string a_szName)
{
    for(int iDx = 0; iDx < m_apkAnimations.size();iDx++)
    {
        if(m_apkAnimations[iDx]->szName == a_szName)
        {
            m_iCurrentFrame = 0;
            m_uiCurrentAnimation = iDx;
            return;
        }
    }
    
    std::cout<<"Animation switch not possible at this time, couldn't find Animation: "<<a_szName<<std::endl;
}

void AnimatedTexture::NextFrame()
{
    m_iCurrentFrame++;

    if(m_iCurrentFrame > m_apkAnimations[m_uiCurrentAnimation]->apkFrames.size() - 1)
    {
        m_iCurrentFrame = 0;
		//std::cout<<"Frame Wrap for animated texture"<<std::endl;
    }

    SwitchFrame(m_iCurrentFrame);
}

void AnimatedTexture::SwitchFrame(unsigned int a_iNewFrame)
{
    if(m_bIsTextureFlipped)
    {
        m_fMinU = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->UMax;
        m_fMinV = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->VMin;
        m_fMaxU = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->UMin;
        m_fMaxV = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->VMax;
    }
    else
    {
        m_fMinU = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->UMin;
        m_fMinV = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->VMin;
        m_fMaxU = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->UMax;
        m_fMaxV = m_apkAnimations[m_uiCurrentAnimation]->apkFrames[a_iNewFrame]->VMax;
    }

	//std::cout<<"Updated UV's for animated texture. "<<std::endl<<"U:"<<m_fMinU<<", V:"<<m_fMinV<<std::endl<<"U:"<<m_fMaxU<<", V:"<<m_fMaxV<<std::endl;
}

void AnimatedTexture::LoadTexture(std::string a_sName, DisplayManager* a_pDisplayManager)
{
	if(a_sName.find(".animated") == std::string::npos) //doesn't contain the name animated
	{
		Texture::LoadTexture(a_sName, a_pDisplayManager);
	}
	else
	{
		LoadAnimation(a_sName, a_pDisplayManager);

		SwitchFrame(0);
	}
}

std::string AnimatedTexture::LoadAnimation(std::string a_sAnimation, DisplayManager* a_pDisplayManager)
{
	std::string sFullFile;
	std::string sLine;
	
	std::string szFile((char*)PackManager::LoadResource(a_sAnimation.c_str()));

	sLine = szFile.substr(0, szFile.find_first_of("\n"));
	szFile.erase(0, szFile.find_first_of("\n") + 1);
    sFullFile += sLine;

	SetTextureNumber(a_pDisplayManager->LoadTexture(sLine));
    
    //TODO: Put in stuff to do with having different animations being added
    m_apkAnimations.push_back(new Animation);
    
    //Default animation creation
    m_apkAnimations[0]->szName = "Default";
    TextureFrame* pkTempFrame = new TextureFrame;
    
    pkTempFrame->UMin = 0.0f;
    
    pkTempFrame->VMin = 0.0f;
    
    pkTempFrame->UMax = 0.0f;
    
    pkTempFrame->VMax = 0.0f;
    
    m_apkAnimations[m_apkAnimations.size() - 1]->apkFrames.push_back(pkTempFrame);
    
	while ( szFile.find_first_of("\n") != std::string::npos )
	{
		sLine = szFile.substr(0, szFile.find_first_of("\n"));
		szFile.erase(0, szFile.find_first_of("\n") + 1);
		sFullFile += sLine;
        
        if(sLine == "Name")
        {
            m_apkAnimations.push_back(new Animation);
            
            sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            m_apkAnimations[m_apkAnimations.size() - 1]->szName = sLine.c_str();
        }
        
        //Different Framerates for Different animations
        if(sLine == "FrameTime")
        {
            //one divded by framerate
            sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            m_apkAnimations[m_apkAnimations.size() - 1]->fFrameTime = (float)atof(sLine.c_str());
        }
        
		if(sLine == "Frame")
		{
		    TextureFrame* pkTempFrame = new TextureFrame;

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->UMin = (float)atof(sLine.c_str());

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->VMin = (float)atof(sLine.c_str());

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->UMax = (float)atof(sLine.c_str());

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->VMax = (float)atof(sLine.c_str());

            m_apkAnimations[m_apkAnimations.size() - 1]->apkFrames.push_back(pkTempFrame);
		}
	}

	return sFullFile;
}

void AnimatedTexture::FlipTexture(bool a_bNewSetting)
{
    m_bIsTextureFlipped = a_bNewSetting;
}
