#ifndef _PARTICLE_H_
#define _PARTICLE_H_
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


//Tiles are the terrain underneath the units,

#include "Object.h"

class Particle : public Object
{
public:
    Particle(Scene* a_pkScene);
    virtual ~Particle();

    virtual bool Update(float a_fDeltaTime);

    void SetCoordinate(Vector a_vCoordinates);
    Vector GetCoordinate();
protected:
	int m_iFallSpeed;
	bool m_bIsGravityOn;
private:
    Vector m_vCoordinates;
	Vector m_vVelocity;
	float m_fRotationSpeed;
	float m_fLifetime;
};

#endif
