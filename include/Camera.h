#ifndef _CAMERA_H_
#define _CAMERA_H_
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

class Camera : public Object
{
public:
	Camera(Scene* a_pkScene);
	virtual ~Camera();

	virtual bool Update(float a_fDeltaTime);

	virtual float GetViewportX();
	virtual float GetViewportY();
protected:
	Matrix* m_pViewMatrix;

	float m_fFoV;
	float m_fNear;
	float m_fFar;
	float m_fAspect;

	float m_fViewportX, m_fViewportY;

	double m_dTargetZoom;
};

#endif