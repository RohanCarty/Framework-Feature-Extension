#include "Button.h"
#include "UIElement.h"
#include "Object.h"

#include "SceneManager.h"
#include "InputManager.h"

#include <cstdlib>

Button::Button(Scene* a_pkScene) : UIElement(a_pkScene)
{
    std::cout<<"Scene created. Pointer: "<<this<<std::endl;

	m_bIsToggled = false;
}

Button::~Button()
{
    std::cout<<"Scene Destroyed. Pointer: "<<this<<std::endl;
}

bool Button::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Button Tick: "<<m_iCounter<<std::endl;
    #endif

	UIElement::Update(a_fDeltaTime);

	if(SceneManager::GetInputManager()->GetMousePosition()->WithinBox2D(*GetLocation(), GetSize()) && !m_bIsToggled)
	{
		if(SceneManager::GetInputManager()->GetIsKeyDown(SDL_BUTTON_LEFT))
		{
			m_bIsToggled = true;

			std::cout<<"Button Toggled"<<std::endl; 
		}
	}

    return true;
}

bool Button::GetIsToggled()
{
	return m_bIsToggled;
}

void Button::SetIsToggled(bool a_bIsToggled)
{
	m_bIsToggled = a_bIsToggled;
}