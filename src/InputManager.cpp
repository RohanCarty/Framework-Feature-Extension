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
		for(int iDx = 0; iDx < SDL_NumJoysticks(); iDx++)
		{
			AddGameController(iDx);
		}
	}

	m_uiCurrentNumOfJoysticks = SDL_NumJoysticks();

	ClearControllerStates();

	std::cout<<"InputManager Created."<<std::endl;
}

InputManager::~InputManager()
{
	//TODO: Actually clean up everything created.

	delete m_pkMousePosition;

	while(m_apkJoysticks.size() > 0)
	{
		RemoveGameController((int)m_apkJoysticks.size() - 1);
		
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
	ClearJumpButtons();

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
			//GameController Stuff

			//TODO: Hotswapping controllers.
			case SDL_CONTROLLERDEVICEADDED:
				AddGameController((int)m_apkJoysticks.size() - 1);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				RemoveGameController(GetControllerIdByJoystickId(test_event.cdevice.which));
				break;
			case SDL_JOYAXISMOTION:
				if( test_event.jaxis.axis == 0)
				{
					if(test_event.jaxis.value < -m_iJoystickDeadzone)
					{
						GetControllerByJoystickId(test_event.jaxis.which)->fAxis1X = -1;
					}
					else if(test_event.jaxis.value > m_iJoystickDeadzone)
					{
						GetControllerByJoystickId(test_event.jaxis.which)->fAxis1X = 1;
					}
					else
					{
						GetControllerByJoystickId(test_event.jaxis.which)->fAxis1X = 0;
					}
				}
				break;
			case SDL_JOYBUTTONDOWN: //.button 10 is X (ps3) .button 11 is O (ps3) .button 12 is [] (ps3) . button 13 is /\ (ps3)
				if(test_event.jbutton.button == 10)
				{
					GetControllerByJoystickId(test_event.jbutton.which)->bJumpPressed = true;
				}
				else if(test_event.jbutton.button == 12)
				{
					GetControllerByJoystickId(test_event.jbutton.which)->bAttackPressed = true;
				}
				else if(test_event.jbutton.button == 13)
				{
					GetControllerByJoystickId(test_event.jbutton.which)->bSpecialPressed = true;
				}
				else
				{
					std::cout<<"Unknown button pressed on "<<test_event.jbutton.which<<" button I.D. is: "<<((int)test_event.jbutton.button)<<std::endl;
				}
				break;
			case SDL_JOYBUTTONUP:
				if(test_event.jbutton.button == 10)
				{
					GetControllerByJoystickId(test_event.jbutton.which)->bJumpPressed = false;
				}
				else if(test_event.jbutton.button == 12)
				{
					GetControllerByJoystickId(test_event.jbutton.which)->bAttackPressed = false;
				}
				else if(test_event.jbutton.button == 13)
				{
					GetControllerByJoystickId(test_event.jbutton.which)->bSpecialPressed = false;
				}
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

//Adds game controller struct to vector of controller mappings.
//	Returns true on success
bool InputManager::AddGameController(int a_iId)
{
	stGameControllerDetails stTemp;

	stTemp.pkJoystick = SDL_JoystickOpen(a_iId);

	if(stTemp.pkJoystick == NULL)
	{
		std::cout<<"Error: Unable to open game controller! SDL Error: "<<SDL_GetError()<<std::endl;

		return false;
	}

	stTemp.bIsBound = false;

	std::cout<<"Adding Game Controller: Name: "<<SDL_JoystickName(stTemp.pkJoystick)<<" Axes: "<<SDL_JoystickNumAxes(stTemp.pkJoystick)<<" Balls: "<<SDL_JoystickNumBalls(stTemp.pkJoystick)
		<<" Buttons: "<<SDL_JoystickNumButtons(stTemp.pkJoystick)<<" Hats: "<<SDL_JoystickNumHats(stTemp.pkJoystick)<<std::endl;

	m_apkJoysticks.push_back(stTemp);

	return true;
}

void InputManager::RemoveGameController(int a_iId)
{
	std::cout<<"Removing Joystick: "<<SDL_JoystickName(m_apkJoysticks[a_iId].pkJoystick)<<"... ";
	SDL_JoystickClose(m_apkJoysticks[a_iId].pkJoystick);

	std::cout<<"Joystick Removed."<<std::endl;
}

int InputManager::GetNumConnectedControllers()
{
	return (int)m_apkJoysticks.size();
}

//Returns a number for a controller, otherwise -1 if none are available.
int InputManager::GetControllerForPlayer()
{
	for(unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		if(!m_apkJoysticks[iDx].bIsBound)
		{
			m_apkJoysticks[iDx].bIsBound = true;
			std::cout<<"Got controller "<<iDx<<" for player."<<std::endl;
			return iDx;
		}
	}

	return -1;
}

void InputManager::ReleaseControllerForPlayer(int a_iControllerNumber)
{
	m_apkJoysticks[a_iControllerNumber].bIsBound = false;
}

bool InputManager::GetIsControllerConnected(int a_iId)
{
	if(a_iId < (int)m_apkJoysticks.size())
	{
		return true;
	}
	else
		return false;
}

stGameControllerDetails InputManager::GetControllerState(int a_iId)
{
	return m_apkJoysticks[a_iId];
}

stGameControllerDetails* InputManager::GetControllerByJoystickId(int a_iId)
{
	for(unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		if(SDL_JoystickInstanceID(m_apkJoysticks[iDx].pkJoystick) == a_iId)
		{
			//std::cout<<"Controller "<<iDx<<" Activity"<<std::endl;
			return &m_apkJoysticks[iDx];
		}
	}

	return NULL;
}

int InputManager::GetControllerIdByJoystickId(int a_iId)
{
	for(unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		if(SDL_JoystickInstanceID(m_apkJoysticks[iDx].pkJoystick) == a_iId)
		{
			return iDx;
		}
	}

	return NULL;
}

//Clears the current states but not the bind.
void InputManager::ClearControllerStates()
{
	for(unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		m_apkJoysticks[iDx].fAxis1X = 0.0f;
		m_apkJoysticks[iDx].fAxis1Y = 0.0f;
		m_apkJoysticks[iDx].bJumpPressed = false;
		m_apkJoysticks[iDx].bAttackPressed = false;
		m_apkJoysticks[iDx].bSpecialPressed = false;
	}
}

void InputManager::ClearJumpButtons()
{
	for(unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		m_apkJoysticks[iDx].bJumpPressed = false;
		m_apkJoysticks[iDx].bAttackPressed = false;
		m_apkJoysticks[iDx].bSpecialPressed = false;
	}
}