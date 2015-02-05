#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_
//Dem debug things
#ifdef _WIN32
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <malloc.h>
		#include <crtdbg.h>
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
	   #endif
	#endif
#endif

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

//Forward declare in order to use the pointer.
class Vector;
class Vertex;
class Texture;
class Camera;

struct stGameControllerDetails
{
	SDL_Joystick* pkJoystick;
	SDL_GameController* pkGameController;
	float fAxis1X;
	float fAxis1Y;
	bool bJumpPressed;
};

class InputManager
{
public:
    InputManager();
    virtual ~InputManager();

    virtual bool Update(float a_fDeltaTime);
	
	bool GetIsKeyDown(int a_iKeycode);

	void SetCameraObject(Camera* a_pkCamera);
    Camera* GetCameraObject();
	Vector* GetMousePosition();
	Vector GetMouseWorldPosition();
	int GetMouseWheelDelta();
	bool GetWindowIsInFocus();

	bool AddGameController(int a_iId);
	void RemoveGameController(int a_iId);

	int GetNumConnectedControllers();

	bool GetIsControllerConnected(int a_iId);

	stGameControllerDetails GetControllerState(int a_iId);

	stGameControllerDetails* GetControllerByJoystickId(int a_iId);
	int GetControllerIdByJoystickId(int a_iId);

	void ClearControllerStates();
	void ClearJumpButtons();

private:
	Vector* m_pkMousePosition;
	Camera* m_pkCamera;
	std::vector<int> m_aiKeyCodes;
	bool m_bIsWindowInFocus;
	int m_iMouseWheelDelta;

	std::vector<stGameControllerDetails> m_apkJoysticks;

	//Joystick/Gamepad variables
	int m_iJoystickDeadzone;
	unsigned int m_uiCurrentNumOfJoysticks;
};

#endif
