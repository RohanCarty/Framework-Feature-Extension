#include "InputManagerUWP.h"

#include "InputManager.h"

#include "Vector.h"

#include "Camera.h"

#include "SceneManager.h"

#include <cstdio>



InputManagerUWP::InputManagerUWP()
{
#ifdef __UWP__
	m_pkMousePosition = new Vector();

	m_pkCamera = NULL;

	m_bIsWindowInFocus = true;

	m_iMouseWheelDelta = 0;

	//Setup the joystick subsystem for SDL2

	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0)
	{
		std::cout << "Joystick/Game Controller/Haptic init failure: " << SDL_GetError() << std::endl;
	}

	m_uiCurrentNumOfJoysticks = Gamepad::Gamepads->Size;

	m_fJoystickDeadzone = 0.15;

	if (Gamepad::Gamepads->Size < 1)
	{
		std::cout << "No Joysticks Connected" << std::endl;

		AddKeyboardAsController();
	}
	else
	{
		for (int iDx = 0; iDx < Gamepad::Gamepads->Size; iDx++)
		{
			AddGameController(iDx);
		}
	}

	ClearControllerStates();

	std::cout << "InputManagerUWP Created." << std::endl;
#endif //#ifdef __UWP__
}

InputManagerUWP::~InputManagerUWP()
{
	//TODO: Actually clean up everything created.

	delete m_pkMousePosition;

	while (m_apkJoysticks.size() > 0)
	{
		RemoveGameController((int)m_apkJoysticks.size() - 1);

		m_apkJoysticks.pop_back();
	}

	std::cout << "Input Manager closed successfully." << std::endl;
}

bool InputManagerUWP::Update(float a_fDeltaTime)
{
#ifdef __UWP__
	SDL_Event test_event;

	//Clear everything pressed
	//m_aiKeyCodes.erase(m_aiKeyCodes.begin(), m_aiKeyCodes.begin() + m_aiKeyCodes.size());
	m_iMouseWheelDelta = 0;
	ClearJumpButtons();

	GamepadReading pkTempGamepadReading = Gamepad::Gamepads->GetAt(0)->GetCurrentReading();
	int pkTempGamepadButtons = (int)pkTempGamepadReading.Buttons;

	if (pkTempGamepadReading.LeftThumbstickX < m_fJoystickDeadzone && pkTempGamepadReading.LeftThumbstickX > -m_fJoystickDeadzone )
	{
		m_apkJoysticks[0].fAxis1X = 0.0;
	}
	else
	{
		m_apkJoysticks[0].fAxis1X = pkTempGamepadReading.LeftThumbstickX;
	}

	if (pkTempGamepadButtons & GamepadButtons_A)
	{
		m_apkJoysticks[0].bJumpPressed = true;
	}
	else
	{
		m_apkJoysticks[0].bJumpPressed = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_X)
	{
		m_apkJoysticks[0].bAttackPressed = true;
	}
	else
	{
		m_apkJoysticks[0].bAttackPressed = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_Y)
	{
		m_apkJoysticks[0].bSpecial1Pressed = true;
	}
	else
	{
		m_apkJoysticks[0].bSpecial1Pressed = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_B)
	{
		m_apkJoysticks[0].bSpecial2Pressed = true;
	}
	else
	{
		m_apkJoysticks[0].bSpecial2Pressed = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_Menu)
	{
		m_apkJoysticks[0].bOptionPressed = true;
	}
	else
	{
		m_apkJoysticks[0].bOptionPressed = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_View)
	{
		m_apkJoysticks[0].bViewPressed = true;
	}
	else
	{
		m_apkJoysticks[0].bViewPressed = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_DPadDown)
	{
		m_apkJoysticks[0].bDPadDown = true;
	}
	else
	{
		m_apkJoysticks[0].bDPadDown = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_DPadLeft)
	{
		m_apkJoysticks[0].bDPadLeft = true;
	}
	else
	{
		m_apkJoysticks[0].bDPadLeft = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_DPadRight)
	{
		m_apkJoysticks[0].bDPadRight = true;
	}
	else
	{
		m_apkJoysticks[0].bDPadRight = false;
	}

	if (pkTempGamepadButtons & GamepadButtons_DPadUp)
	{
		m_apkJoysticks[0].bDPadUp = true;
	}
	else
	{
		m_apkJoysticks[0].bDPadUp = false;
	}

	while (SDL_PollEvent(&test_event)) //TODO: receive all inputs.
	{
		switch (test_event.type)
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
		case SDL_MOUSEBUTTONUP:
			for (unsigned int uiDx = 0; uiDx < m_aiKeyCodes.size(); uiDx++)
			{
				if (test_event.button.button == m_aiKeyCodes[uiDx])
				{
					m_aiKeyCodes.erase(m_aiKeyCodes.begin() + uiDx);
				}
			}
			break;
		case SDL_KEYDOWN:
			m_aiKeyCodes.push_back(test_event.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			for (unsigned int uiDx = 0; uiDx < m_aiKeyCodes.size(); uiDx++)
			{
				if (m_aiKeyCodes[uiDx] == test_event.key.keysym.scancode)
				{
					m_aiKeyCodes.erase(m_aiKeyCodes.begin() + uiDx);
					uiDx--;
				}
			}
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
			if (test_event.jaxis.axis == 0) // Left Stick X
			{
				if (test_event.jaxis.value < -m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis1X = -1;
				}
				else if (test_event.jaxis.value > m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis1X = 1;
				}
				else
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis1X = 0;
				}
			}
			else if (test_event.jaxis.axis == 1) // Left Stick Y
			{
				if (test_event.jaxis.value < -m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis1Y = -1;
				}
				else if (test_event.jaxis.value > m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis1Y = 1;
				}
				else
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis1Y = 0;
				}
			}
			else if (test_event.jaxis.axis == 2) // Right Stick X
			{
				if (test_event.jaxis.value < -m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis2X = -1;
				}
				else if (test_event.jaxis.value > m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis2X = 1;
				}
				else
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis2X = 0;
				}
			}
			else if (test_event.jaxis.axis == 3) // Right Stick Y
			{
				if (test_event.jaxis.value < -m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis2Y = -1;
				}
				else if (test_event.jaxis.value > m_fJoystickDeadzone)
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis2Y = 1;
				}
				else
				{
					GetControllerByJoystickId(test_event.jaxis.which)->fAxis2Y = 0;
				}
			}
			break;
		case SDL_JOYBUTTONDOWN: //.button 10 is A (360) .button 11 is B (360) .button 12 is X (360) . button 13 is Y (360)
			if (test_event.jbutton.button == 0)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadUp = true;
			}
			else if (test_event.jbutton.button == 1)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadDown = true;
			}
			else if (test_event.jbutton.button == 2)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadLeft = true;
			}
			else if (test_event.jbutton.button == 3)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadRight = true;
			}
			else if (test_event.jbutton.button == 4)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bOptionPressed = true;
			}
			else if (test_event.jbutton.button == 5)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bViewPressed = true;
			}
			else if (test_event.jbutton.button == 10)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bJumpPressed = true;
			}
			else if (test_event.jbutton.button == 11)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bSpecial2Pressed = true;
			}
			else if (test_event.jbutton.button == 12)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bAttackPressed = true;
			}
			else if (test_event.jbutton.button == 13)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bSpecial1Pressed = true;
			}
			else
			{
				std::cout << "Unknown button pressed on " << test_event.jbutton.which << " button I.D. is: " << ((int)test_event.jbutton.button) << std::endl;
			}
			break;
		case SDL_JOYBUTTONUP:
			if (test_event.jbutton.button == 0)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadUp = false;
			}
			else if (test_event.jbutton.button == 1)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadDown = false;
			}
			else if (test_event.jbutton.button == 2)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadLeft = false;
			}
			else if (test_event.jbutton.button == 3)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bDPadRight = false;
			}
			else if (test_event.jbutton.button == 4)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bOptionPressed = false;
			}
			else if (test_event.jbutton.button == 5)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bViewPressed = false;
			}
			else if (test_event.jbutton.button == 10)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bJumpPressed = false;
			}
			else if (test_event.jbutton.button == 11)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bSpecial2Pressed = false;
			}
			else if (test_event.jbutton.button == 12)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bAttackPressed = false;
			}
			else if (test_event.jbutton.button == 13)
			{
				GetControllerByJoystickId(test_event.jbutton.which)->bSpecial1Pressed = false;
			}
			break;
		case SDL_QUIT:
			return false;//TODO: Proper quitting (I think it's done?)
		case SDL_WINDOWEVENT:
			switch (test_event.window.event)
			{
			case SDL_WINDOWEVENT_ENTER:
				m_bIsWindowInFocus = true;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				m_bIsWindowInFocus = false;
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				std::cout << "Window Focus Gained" << std::endl;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				std::cout << "Window Focus Lost" << std::endl;
				break;
			}
			break;
		default:
			break;

		}
	}
#endif //#ifdef __UWP__
	return true;
}

bool InputManagerUWP::GetIsKeyDown(int a_iKeycode)
{
	for (unsigned int iDx = 0; iDx < m_aiKeyCodes.size(); iDx++)
	{
		if (a_iKeycode == m_aiKeyCodes[iDx])
		{
			//std::cout<<"Found keycode"<<std::endl;
			return true;
		}
	}

	return false;
}

bool InputManagerUWP::GetWindowIsInFocus()
{
	return m_bIsWindowInFocus;
}

void InputManagerUWP::SetCameraObject(Camera* a_pkCamera)
{
	m_pkCamera = a_pkCamera;
}

Camera* InputManagerUWP::GetCameraObject()
{
	return m_pkCamera;
}

Vector* InputManagerUWP::GetMousePosition()
{
	return m_pkMousePosition;
}

Vector InputManagerUWP::GetMouseWorldPosition()
{
	if (m_pkCamera == NULL)
	{
		return *m_pkMousePosition;
	}

	Vector vTemp = *m_pkMousePosition;

	vTemp.x = GetMousePosition()->x - (m_pkCamera->GetViewportX() / 2) + m_pkCamera->GetWorldLocation()->x;
	vTemp.y = GetMousePosition()->y - (m_pkCamera->GetViewportY() / 2) + m_pkCamera->GetWorldLocation()->y;

	return vTemp / m_pkCamera->GetLocation()->z;
}

int InputManagerUWP::GetMouseWheelDelta()
{
	return m_iMouseWheelDelta;
}

//Adds game controller struct to vector of controller mappings.
//	Returns true on success
bool InputManagerUWP::AddGameController(int a_iId)
{
	stGameControllerDetails stTemp;

	stTemp.pkJoystick = SDL_JoystickOpen(a_iId);

	if (stTemp.pkJoystick == NULL)
	{
		std::cout << "Error: Unable to open game controller! SDL Error: " << SDL_GetError() << std::endl;

		return false;
	}

	stTemp.bIsBound = false;

	std::cout << "Adding Game Controller: Name: " << SDL_JoystickName(stTemp.pkJoystick) << " Axes: " << SDL_JoystickNumAxes(stTemp.pkJoystick) << " Balls: " << SDL_JoystickNumBalls(stTemp.pkJoystick)
		<< " Buttons: " << SDL_JoystickNumButtons(stTemp.pkJoystick) << " Hats: " << SDL_JoystickNumHats(stTemp.pkJoystick) << std::endl;

	m_apkJoysticks.push_back(stTemp);

	stTemp.bIsKeyboard = false;

	return true;
}

void InputManagerUWP::AddKeyboardAsController()
{
	stGameControllerDetails stTemp;

	stTemp.pkJoystick = NULL;

	stTemp.bIsBound = false;

	std::cout << "Adding keyboard as game controller." << std::endl;

	stTemp.bIsKeyboard = true;

	m_apkJoysticks.push_back(stTemp);
}


void InputManagerUWP::RemoveGameController(int a_iId)
{
	if (m_apkJoysticks[a_iId].bIsKeyboard)
	{
		std::cout << "Keyboard Removed." << std::endl;

		return;
	}

	std::cout << "Removing Joystick: " << SDL_JoystickName(m_apkJoysticks[a_iId].pkJoystick) << "... ";

	SDL_JoystickClose(m_apkJoysticks[a_iId].pkJoystick);

	std::cout << "Joystick Removed." << std::endl;
}

int InputManagerUWP::GetNumConnectedControllers()
{
	return (int)m_apkJoysticks.size();
}

//Returns a number for a controller, otherwise -1 if none are available.
int InputManagerUWP::GetControllerForPlayer()
{
	for (unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		if (!m_apkJoysticks[iDx].bIsBound)
		{
			m_apkJoysticks[iDx].bIsBound = true;
			std::cout << "Got controller " << iDx << " for player." << std::endl;
			return iDx;
		}
	}

	return -1;
}

void InputManagerUWP::ReleaseControllerForPlayer(int a_iControllerNumber)
{
	m_apkJoysticks[a_iControllerNumber].bIsBound = false;
}

bool InputManagerUWP::GetIsControllerConnected(int a_iId)
{
	if (a_iId < (int)m_apkJoysticks.size())
	{
		return true;
	}
	else
		return false;
}

stGameControllerDetails InputManagerUWP::GetControllerState(int a_iId)
{
	return m_apkJoysticks[a_iId];
}

void InputManagerUWP::SetControllerState(int a_iId, stGameControllerDetails a_stGameControllerDetails)
{
	m_apkJoysticks[a_iId] = a_stGameControllerDetails;
}

stGameControllerDetails* InputManagerUWP::GetControllerByJoystickId(int a_iId)
{
	for (unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		if (SDL_JoystickInstanceID(m_apkJoysticks[iDx].pkJoystick) == a_iId)
		{
			//std::cout<<"Controller "<<iDx<<" Activity"<<std::endl;
			return &m_apkJoysticks[iDx];
		}
	}

	return NULL;
}

int InputManagerUWP::GetControllerIdByJoystickId(int a_iId)
{
	for (unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		if (SDL_JoystickInstanceID(m_apkJoysticks[iDx].pkJoystick) == a_iId)
		{
			return iDx;
		}
	}

	return NULL;
}

//Clears the current states but not the bind.
void InputManagerUWP::ClearControllerStates()
{
	for (unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		m_apkJoysticks[iDx].fAxis1X = 0.0f;
		m_apkJoysticks[iDx].fAxis1Y = 0.0f;
		m_apkJoysticks[iDx].fAxis2X = 0.0f;
		m_apkJoysticks[iDx].fAxis2Y = 0.0f;
		m_apkJoysticks[iDx].bJumpPressed = false;
		m_apkJoysticks[iDx].bAttackPressed = false;
		m_apkJoysticks[iDx].bSpecial1Pressed = false;
		m_apkJoysticks[iDx].bSpecial2Pressed = false;
		m_apkJoysticks[iDx].bViewPressed = false;
		m_apkJoysticks[iDx].bOptionPressed = false;
		m_apkJoysticks[iDx].bDPadUp = false;
		m_apkJoysticks[iDx].bDPadDown = false;
		m_apkJoysticks[iDx].bDPadLeft = false;
		m_apkJoysticks[iDx].bDPadRight = false;
	}
}

void InputManagerUWP::ClearJumpButtons()
{
	for (unsigned int iDx = 0; iDx < m_apkJoysticks.size(); iDx++)
	{
		m_apkJoysticks[iDx].bJumpPressed = false;
		m_apkJoysticks[iDx].bAttackPressed = false;
		/*m_apkJoysticks[iDx].bSpecial1Pressed = false;
		m_apkJoysticks[iDx].bSpecial2Pressed = false;
		m_apkJoysticks[iDx].bViewPressed = false;
		m_apkJoysticks[iDx].bOptionPressed = false;*/
	}
}