#include "StartScreen.h"
#include "Actor.h"
#include "HUD.h"
#include "GameInfo.h"
#include "SceneManager.h"

#include <cstdlib>

StartScreen::StartScreen() : Scene()
{
    std::cout<<"StartScreen created. Pointer: "<<this<<std::endl;
}

StartScreen::~StartScreen()
{
    std::cout<<"StartScreen Destroyed. Pointer: "<<this<<std::endl;
}

bool StartScreen::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"StartScreen Tick: "<<m_iCounter<<std::endl;
    #endif

    return false;
}
