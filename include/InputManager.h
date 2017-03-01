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
	float fAxis1X;//Left stick
	float fAxis1Y;
	float fAxis2X;//Right stick
	float fAxis2Y;
	bool bJumpPressed;
	bool bAttackPressed;
	bool bSpecial1Pressed;
	bool bSpecial2Pressed;
	bool bViewPressed;
	bool bOptionPressed;
	bool bDPadUp;
	bool bDPadDown;
	bool bDPadLeft;
	bool bDPadRight;
	bool bIsBound;
	bool bIsKeyboard;
};

class InputManager
{
public:
    InputManager();
    virtual ~InputManager();

    virtual bool Update(float a_fDeltaTime);
	
	virtual bool GetIsKeyDown(int a_iKeycode);

	virtual void SetCameraObject(Camera* a_pkCamera);
    virtual Camera* GetCameraObject();
	virtual Vector* GetMousePosition();
	virtual Vector GetMouseWorldPosition();
	virtual int GetMouseWheelDelta();
	virtual bool GetWindowIsInFocus();

	virtual bool AddGameController(int a_iId);
	virtual void RemoveGameController(int a_iId);

	virtual int GetNumConnectedControllers();
	virtual int GetControllerForPlayer();
	virtual void ReleaseControllerForPlayer(int a_iControllerNumber);

	virtual bool GetIsControllerConnected(int a_iId);

	virtual stGameControllerDetails GetControllerState(int a_iId) = 0; //Pure virtual

	virtual void SetControllerState(int a_iId, stGameControllerDetails a_stGameControllerDetails) = 0;

	virtual stGameControllerDetails* GetControllerByJoystickId(int a_iId) = 0; //Pure virtual
	virtual int GetControllerIdByJoystickId(int a_iId);

	virtual void ClearControllerStates();
	virtual void ClearJumpButtons();

protected:
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
