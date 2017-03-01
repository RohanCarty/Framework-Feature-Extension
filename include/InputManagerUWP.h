#ifndef _INPUTMANAGERUWP_H_
#define _INPUTMANAGERUWP_H_
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

#ifdef __UWP__
using namespace Windows::Foundation::Collections;
using namespace Windows::Gaming::Input;
#endif

//Forward declare in order to use the pointer.
class Vector;
class Vertex;
class Texture;
class Camera;

class InputManagerUWP : public InputManager
{
public:
	InputManagerUWP();
	virtual ~InputManagerUWP();

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

	virtual stGameControllerDetails GetControllerState(int a_iId);

	virtual void SetControllerState(int a_iId, stGameControllerDetails a_stGameControllerDetails);

	virtual stGameControllerDetails* GetControllerByJoystickId(int a_iId);
	virtual int GetControllerIdByJoystickId(int a_iId);

	virtual void ClearControllerStates();
	virtual void ClearJumpButtons();


	void AddKeyboardAsController();

private:
	std::vector<stGameControllerDetails> m_apkJoysticks;
};
#endif
