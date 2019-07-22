#include "InputManager.h"

#include "Vector.h"

#include "Camera.h"

#include "SceneManager.h"

#include <cstdio>

InputManager::InputManager()
{
	std::cout<<"InputManager Created."<<std::endl;
}

InputManager::~InputManager()
{
	std::cout<<"Input Manager closed successfully."<<std::endl;
}

bool InputManager::Update(float a_fDeltaTime)
{		
	return true;
}

bool InputManager::GetIsKeyDown(int a_iKeycode)
{
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
	return true;
}

void InputManager::RemoveGameController(int a_iId)
{
}

int InputManager::GetNumConnectedControllers()
{
	return 0;
}

//Returns a number for a controller, otherwise -1 if none are available.
int InputManager::GetControllerForPlayer()
{
	return -1;
}

void InputManager::ReleaseControllerForPlayer(int a_iControllerNumber)
{

}

bool InputManager::GetIsControllerConnected(int a_iId)
{
	return false;
}

int InputManager::GetControllerIdByJoystickId(int a_iId)
{
	return -1;
}

//Clears the current states but not the bind.
void InputManager::ClearControllerStates()
{
}

void InputManager::ClearJumpButtons()
{
}