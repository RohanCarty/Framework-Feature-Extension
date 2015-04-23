#include "Particle.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Mesh.h"

#include "AnimatedTexture.h"

Particle::Particle(Scene* a_pkScene) : Object(a_pkScene)
{
    //std::cout<<"Particle created. Pointer: "<<this<<std::endl;
	//m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/RedZone.png", SceneManager::GetDisplayManager());

	m_vVelocity.x = ((double)(rand() % 256 + 0) - 128) * 4.0;
	m_vVelocity.y = ((double)(rand() % 255 + 0) - 255) * 10.0;

	m_fRotationSpeed = ((float)(rand() % 255 + 0) - 128) / 64.0f;

	//std::cout<<"Particle created with velocity vector of: "<<m_vVelocity<<" and a rotation speed of: "<<m_fRotationSpeed<<std::endl;

	m_fLifetime = 1.0f;

	m_iFallSpeed = 100;
	m_bIsGravityOn = true;

    SetScale(0.2f);
    SetSize(Vector(32,32,32));
}

Particle::~Particle()
{
    //std::cout<<"Particle Destroyed. Pointer: "<<this<<std::endl;
}

bool Particle::Update(float a_fDeltaTime)
{
#ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Tile Tick: "<<this<<std::endl;
#endif
	// check for below zero lifetime before decrementing to allow for single frame display.
	if(m_fLifetime < 0.0f)
	{
		return false;
	}

	m_fLifetime -= a_fDeltaTime;

	m_fRotation += m_fRotationSpeed * a_fDeltaTime;

	if(m_bIsGravityOn)
	{
		m_vVelocity.y += m_iFallSpeed;
	}

    Object::Update(a_fDeltaTime);

	SetLocation(GetLocation()->x + (m_vVelocity.x * a_fDeltaTime), GetLocation()->y + (m_vVelocity.y * a_fDeltaTime), GetLocation()->z + (m_vVelocity.z * a_fDeltaTime));

    return true;
}

void Particle::SetCoordinate(Vector a_vCoordinates)
{
    m_vCoordinates = a_vCoordinates;
}

Vector Particle::GetCoordinate()
{
    return m_vCoordinates;
}

void Particle::SetVelocity(Vector a_vNewVelocity)
{
	m_vVelocity = a_vNewVelocity;
}

void Particle::SetLifetime(float a_fNewLifetime)
{
	m_fLifetime = a_fNewLifetime;
}