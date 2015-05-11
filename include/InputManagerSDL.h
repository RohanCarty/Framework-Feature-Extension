#ifndef _INPUTMANAGERSDL_H_
#define _INPUTMANAGERSDL_H_
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

#include "InputManager.h"

#include <SDL2/SDL.h>

struct stGameControllerDetails
{
	SDL_Joystick* pkJoystick;
	SDL_GameController* pkGameController;
	float fAxis1X;
	float fAxis1Y;
	bool bJumpPressed;
	bool bAttackPressed;
	bool bSpecial1Pressed;
	bool bSpecial2Pressed;
	bool bViewPressed;
	bool bOptionPressed;
	bool bIsBound;
};

//Forward declare in order to use the pointer.
class Vector;
class Vertex;
class Texture;
class Camera;

class InputManagerSDL : public InputManager
{
public:
    InputManagerSDL();
    virtual ~InputManagerSDL();

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
	int GetControllerForPlayer();
	void ReleaseControllerForPlayer(int a_iControllerNumber);

	bool GetIsControllerConnected(int a_iId);

	stGameControllerDetails GetControllerState(int a_iId);

	stGameControllerDetails* GetControllerByJoystickId(int a_iId);
	int GetControllerIdByJoystickId(int a_iId);

	void ClearControllerStates();
	void ClearJumpButtons();

private:
	std::vector<stGameControllerDetails> m_apkJoysticks;
};

#endif
