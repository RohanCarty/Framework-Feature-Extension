#ifndef _ACTOR_H_
#define _ACTOR_H_
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

//Forward declare
class Scene;
class Vector;

#include "Object.h"

class Actor : public Object
{
public:
    Actor(Scene* a_pkScene);
    virtual ~Actor();

    bool Update(float a_fDeltaTime);

    void SetVelocity(Vector a_Vector);
    void SetVelocity(double a_x, double a_y, double a_z);

	void ApplyGravity(float a_fDeltaTime);

	bool IsCollidingWithTileNextFrame(float a_fDeltaTime);
    bool IsCollidingWithActorNextFrame(float a_fDeltaTime);
    
    Vector GetCollisionVector();

protected:
    Vector* m_pVelocity;
	Vector m_vMaxSpeed;
	int m_iAcceleration;
	int m_iJumpSpeed;
	int m_iFallSpeed;
	bool m_bIsGravityOn;
    int m_iHealth;

private:
	bool m_bIsCollidingTileNextFrame;
    bool m_bIsCollidingActorNextFrame;
	Object* m_pkIsCollidingWithNextFame;
	bool m_bIsCollidingTileNextFrameSet;
    bool m_bIsCollidingActorNextFrameSet;
    Vector m_vCollisionVector;
};

#endif
