#ifndef _CURSOR_H_
#define _CURSOR_H_
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

#include "Object.h"
#include "Matrix.h"
#include "OpenGLDisplayManager.h"

#include <sstream>

class Camera;
class Object;

class Cursor : public Object
{
public:
	Cursor(Scene* a_pkScene);
	virtual ~Cursor();

	virtual bool Update(float a_fDeltaTime);

	virtual bool IsOverObject();

	virtual void SetCamera(Camera* a_pkCamera);
protected:
	Camera* m_pkCamera;
	Object* m_pkObjectHoveredOver;
	bool m_bIsObjectHoveredOverABuilding;
};

#endif