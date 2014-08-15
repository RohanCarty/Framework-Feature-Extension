#include "InputManager.h"

#include "Vector.h"

#include "Camera.h"

#include "SceneManager.h"

#include <cstdio>

InputManager::InputManager()
{
	m_pkMousePosition = new Vector();

	m_pkCamera = NULL;

	m_bIsWindowInFocus = true;

	std::cout<<"InputManager Created."<<std::endl;
}
InputManager::~InputManager()
{
	//TODO: Actually clean up everything created.

	delete m_pkMousePosition;
	
	std::cout<<"Input Manager closed successfully."<<std::endl;
}

bool InputManager::Update(float a_fDeltaTime)
{		
	SDL_Event test_event;

	//Clear everything pressed
	m_aiKeyCodes.erase(m_aiKeyCodes.begin(), m_aiKeyCodes.begin() + m_aiKeyCodes.size());

	while(SDL_PollEvent(&test_event)) //TODO: receive all inputs.
	{
		switch(test_event.type)
		{
			case SDL_MOUSEMOTION:
				m_pkMousePosition->x = test_event.motion.x;
				m_pkMousePosition->y = test_event.motion.y;
				//printf("Current mouse position is: (%d, %d)\n", test_event.motion.x, test_event.motion.y);
				break;
            case SDL_MOUSEBUTTONDOWN:
				m_aiKeyCodes.push_back(test_event.button.button);
                break;
			case SDL_KEYDOWN:
				m_aiKeyCodes.push_back(test_event.key.keysym.scancode);
				break;
			case SDL_QUIT:
				return false;//TODO: Proper quitting (I think it's done?)
			case SDL_WINDOWEVENT:
				switch(test_event.window.event)
				{
					case SDL_WINDOWEVENT_ENTER:
						m_bIsWindowInFocus = true;
						break;
					case SDL_WINDOWEVENT_LEAVE:
						m_bIsWindowInFocus = false;
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						std::cout<<"Window Focus Gained"<<std::endl;
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						std::cout<<"Window Focus Lost"<<std::endl;
						break;
				}
				break;
			default:
				break;
			
		}
	}
	
	return true;
}

bool InputManager::GetIsKeyDown(int a_iKeycode)
{
	for(unsigned int iDx = 0; iDx < m_aiKeyCodes.size(); iDx++)
	{
		if( a_iKeycode == m_aiKeyCodes[iDx])
		{
			//std::cout<<"Found keycode"<<std::endl;
			return true;
		}
	}

	return false;
}

bool InputManager::GetWindowIsInFocus()
{
	return m_bIsWindowInFocus;
}

void InputManager::SetCameraObject(Camera* a_pkCamera)
{
	m_pkCamera = a_pkCamera;
}

Camera* InputManager::GetCameraObject()
{
    return m_pkCamera;
}

Vector* InputManager::GetMousePosition()
{
	return m_pkMousePosition;
}

Vector InputManager::GetMouseWorldPosition()
{
    // Temporary middle of screen bollocks for an accurate mouse cursor until a moving camera is implemented.
    /*Vector vTemp;
    
    float fXMiddleScreen, fYMiddleScreen;
    fXMiddleScreen = SceneManager::GetDisplayManager()->GetXScreenResolution() / 2;
    fYMiddleScreen = SceneManager::GetDisplayManager()->GetYScreenResolution() / 2;
    
    vTemp.x = (GetMousePosition()->x - fXMiddleScreen);
    vTemp.y = (GetMousePosition()->y - fYMiddleScreen);
    vTemp.z = 0.0;*/
	if(m_pkCamera == NULL)
	{
		return *m_pkMousePosition;
	}

	Vector vTemp = *m_pkMousePosition;

	vTemp.x = GetMousePosition()->x - (m_pkCamera->GetViewportX() / 2) + m_pkCamera->GetWorldLocation()->x;
	vTemp.y = GetMousePosition()->y - (m_pkCamera->GetViewportY() / 2) + m_pkCamera->GetWorldLocation()->y;

	return vTemp;
}