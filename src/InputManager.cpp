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

	m_iMouseWheelDelta = 0;

	//Setup the joystick subsystem for SDL2

	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) != 0)
	{
		std::cout<<"Joystick/Game Controller/Haptic init failure: "<<SDL_GetError()<<std::endl;
	}

	m_iJoystickDeadzone = 8000;

	if(SDL_NumJoysticks() < 1)
	{
		std::cout<<"No Joysticks Connected"<<std::endl;
	}
	else
	{
		for(unsigned int iDx = 0; iDx < SDL_NumJoysticks(); iDx++)
		{
			AddGameController(iDx);
		}
	}

	m_uiCurrentNumOfJoysticks = 0;

	std::cout<<"InputManager Created."<<std::endl;
}

InputManager::~InputManager()
{
	//TODO: Actually clean up everything created.

	delete m_pkMousePosition;

	while(m_apkJoysticks.size() > 0)
	{
		delete m_apkJoysticks[m_apkJoysticks.size() - 1].pkJoystick;
		delete m_apkJoysticks[m_apkJoysticks.size() - 1].pkGameController;
		
		m_apkJoysticks.pop_back();
	}
	
	std::cout<<"Input Manager closed successfully."<<std::endl;
}

bool InputManager::Update(float a_fDeltaTime)
{		
	SDL_Event test_event;

	//Clear everything pressed
	m_aiKeyCodes.erase(m_aiKeyCodes.begin(), m_aiKeyCodes.begin() + m_aiKeyCodes.size());
	m_iMouseWheelDelta = 0;

	while(SDL_PollEvent(&test_event)) //TODO: receive all inputs.
	{
		switch(test_event.type)
		{
			case SDL_MOUSEMOTION:
				m_pkMousePosition->x = test_event.motion.x;
				m_pkMousePosition->y = test_event.motion.y;
				//printf("Current mouse position is: (%d, %d)\n", test_event.motion.x, test_event.motion.y);
				break;
			case SDL_MOUSEWHEEL:
				m_iMouseWheelDelta = test_event.wheel.y;
				//printf("Mousewheel event: %d\n",test_event.wheel.y);
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

	return vTemp / m_pkCamera->GetLocation()->z;
}

int InputManager::GetMouseWheelDelta()
{
	return m_iMouseWheelDelta;
}

bool InputManager::AddGameController(int a_iId)
{
	stGameControllerDetails stTemp;

	stTemp.pkJoystick = SDL_JoystickOpen(a_iId);

	if(stTemp.pkJoystick == NULL)
	{
		std::cout<<"Error: Unable to open game controller! SDL Error: "<<SDL_GetError()<<std::endl;
	}
}