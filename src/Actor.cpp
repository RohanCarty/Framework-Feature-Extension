#include "Actor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"

Actor::Actor(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Actor created. Pointer: "<<this<<std::endl;

	//Actors have velocity over an object, objects being static.
    m_pVelocity = new Vector();
}

Actor::~Actor()
{
    //std::cout<<"Actor Destroyed. Pointer: "<<this<<std::endl;

    delete m_pVelocity;
}

bool Actor::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Actor Tick: "<<this<<std::endl;
    #endif

    Object::Update(a_fDeltaTime);

	SetLocation(GetLocation()->x + (m_pVelocity->x * a_fDeltaTime), GetLocation()->y + (m_pVelocity->y * a_fDeltaTime), GetLocation()->z + (m_pVelocity->z * a_fDeltaTime));

    return true;
}


void Actor::SetVelocity(Vector a_NewVelocity)
{
    m_pVelocity->x = a_NewVelocity.x;
    m_pVelocity->y = a_NewVelocity.y;
	m_pVelocity->z = a_NewVelocity.z;
}

void Actor::SetVelocity(double a_x, double a_y, double a_z)
{
    m_pVelocity->x = a_x;
    m_pVelocity->y = a_y;
	m_pVelocity->z = a_z;
}
