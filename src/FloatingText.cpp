#include "FloatingText.h"
#include "TextLibrary.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Mesh.h"
#include "Object.h"

FloatingText::FloatingText(Scene* a_pkScene) : Particle(a_pkScene)
{
    //std::cout<<"Floating Text created. Pointer: "<<this<<std::endl;

	m_apkRenderables[0].m_bIsHidden = true;

	m_vVelocity.x = 0.0;
	m_vVelocity.y = -500.0;

	m_fRotationSpeed = 0.0f;

	//std::cout<<"Particle created with velocity vector of: "<<m_vVelocity<<" and a rotation speed of: "<<m_fRotationSpeed<<std::endl;

	m_fLifetime = 1.0f;

	m_bIsGravityOn = false;
}

FloatingText::~FloatingText()
{
    //std::cout<<"Particle Destroyed. Pointer: "<<this<<std::endl;
}

bool FloatingText::Update(float a_fDeltaTime)
{
#ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Particle Tick: "<<this<<std::endl;
#endif

    if(!Particle::Update(a_fDeltaTime))
	{
		return false;
	}

	SceneManager::GetDisplayManager()->SetShaderProgram(SceneManager::GetDisplayManager()->GetDefaultShader());

						// Possible dynamic sizing, worry about it later
	int iFontSize = 36;//SceneManager::GetDisplayManager()->GetXScreenResolution() * 11;

	//TODO: Implement Colour changing.
	GetTextLibrary()->PrintString(m_szTempString, GetLocation()->x, GetLocation()->y, iFontSize);

    return true;
}

void FloatingText::SetParticleManager(ParticleManager* a_pkParticleManagerReference)
{
	m_pkParticleManagerReference = a_pkParticleManagerReference;
}

TextLibrary* FloatingText::GetTextLibrary()
{
	return m_pkParticleManagerReference->GetTextLibrary();
}

void FloatingText::SetText(std::string a_szNewText)
{
	m_szTempString = a_szNewText;
}