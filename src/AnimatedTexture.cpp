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

	m_bIsAnimated = true;
}

AnimatedTexture::~AnimatedTexture()
{
    for(unsigned int i = 0; i < m_vkFrames.size(); i++)
    {
        delete m_vkFrames[i];
    }
}

bool AnimatedTexture::Update(float a_fDeltaTime)
{
	if(m_vkFrames.size() == 0) // zero frames
	{
		return true;
	}

    m_fCurrentTime += a_fDeltaTime;

    if(m_fCurrentTime > m_fFrameTime)
    {
        NextFrame();
        m_fCurrentTime = 0.0;
    }

    return true;
}

void AnimatedTexture::NextFrame()
{
    m_iCurrentFrame++;

    if(m_iCurrentFrame > m_vkFrames.size() - 1)
    {
        m_iCurrentFrame = 0;
		//std::cout<<"Frame Wrap for animated texture"<<std::endl;
    }

    SwitchFrame(m_iCurrentFrame);
}

void AnimatedTexture::SwitchFrame(unsigned int a_iNewFrame)
{
    m_fMinU = m_vkFrames[a_iNewFrame]->UMin;
    m_fMinV = m_vkFrames[a_iNewFrame]->VMin;
    m_fMaxU = m_vkFrames[a_iNewFrame]->UMax;
    m_fMaxV = m_vkFrames[a_iNewFrame]->VMax;

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

	int piSize;

	PackManager::LoadResource(a_sAnimation.c_str(),&piSize);
	
	std::string szFile((char*)PackManager::LoadResource(a_sAnimation.c_str(),&piSize));

	sLine = szFile.substr(0, szFile.find_first_of("\n"));
	szFile.erase(0, szFile.find_first_of("\n") + 1);
    sFullFile += sLine;
	
	SetTextureNumber(a_pDisplayManager->LoadTexture(sLine));

    sLine = szFile.substr(0, szFile.find_first_of("\n"));
	szFile.erase(0, szFile.find_first_of("\n") + 1);
    sFullFile += sLine;

	//one divded by framerate
    m_fFrameTime = atof(sLine.c_str());

	while ( szFile.find_first_of("\n") != std::string::npos )
	{
		sLine = szFile.substr(0, szFile.find_first_of("\n"));
		szFile.erase(0, szFile.find_first_of("\n") + 1);
		sFullFile += sLine;

		if(sLine == "Frame")
		{
		    TextureFrame* pkTempFrame = new TextureFrame;

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->UMin = atof(sLine.c_str());

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->VMin = atof(sLine.c_str());

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->UMax = atof(sLine.c_str());

		    sLine = szFile.substr(0, szFile.find_first_of("\n"));
			szFile.erase(0, szFile.find_first_of("\n") + 1);
            pkTempFrame->VMax = atof(sLine.c_str());

            m_vkFrames.push_back(pkTempFrame);
		}
	}

	return sFullFile;
}
