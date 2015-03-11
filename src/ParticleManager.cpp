#include "ParticleManager.h"
#include "Particle.h"
#include "TextLibrary.h"
#include "FloatingText.h"
#include "SceneManager.h"
#include "Resource.h"

ParticleManager::ParticleManager()
{
    //SpawnParticle(Vector(0,0,0));
	//SpawnFloatingText(Vector(0,0,0), "Floating Text Test");
	m_pkTextLibrary = new TextLibrary(SceneManager::GetDisplayManager());
}

ParticleManager::~ParticleManager()
{
	delete m_pkTextLibrary;
}

bool ParticleManager::Update(float a_fDeltaTime)
{
    //Update all the tiles

	//auto spawn particle test.
	if(m_apkParticles.size() < 1)
	{
		//SpawnParticle(Vector(0,0,0));
	}

	for( unsigned int iDx = 0; iDx < m_apkParticles.size(); iDx++ )
    {
        if(!m_apkParticles[iDx]->Update(a_fDeltaTime))
        {
            //if a unit returns false, delete it and wind the loop back one.
            delete m_apkParticles[iDx];
            m_apkParticles.erase(m_apkParticles.begin() + iDx);
            iDx--;
        }
	}

    return true;
}

void ParticleManager::SpawnParticle(Vector a_vDestination)
{
    //TODO: Everything
    
    m_apkParticles.push_back(new Particle(SceneManager::GetCurrentScene()));
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkParticles[m_apkParticles.size() - 1]->SetLocation(a_vDestination);

	m_apkParticles[m_apkParticles.size() - 1]->SetRotation((rand() % 3 + 1) * 3.1415926535f / 2.0f);

    return;
}

void ParticleManager::SpawnFloatingText(Vector a_vDestination, std::string a_szTextToDisplay)
{
	m_apkParticles.push_back(new FloatingText(SceneManager::GetCurrentScene()));
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkParticles[m_apkParticles.size() - 1]->SetLocation(a_vDestination);

	((FloatingText*)m_apkParticles[m_apkParticles.size() - 1])->SetText(a_szTextToDisplay);

	((FloatingText*)m_apkParticles[m_apkParticles.size() - 1])->SetParticleManager(this);
}

void ParticleManager::SpawnFloatingText(Vector a_vDestination, int a_iNumberToDisplay)
{
	m_apkParticles.push_back(new FloatingText(SceneManager::GetCurrentScene()));
    
    //Vector vRandomPosition(rand()%800 - 400,rand()%600 - 300, 0.0);
    
    m_apkParticles[m_apkParticles.size() - 1]->SetLocation(a_vDestination);

	char acTempString[256];

	sprintf(acTempString,"%d", (int) a_iNumberToDisplay);

	((FloatingText*)m_apkParticles[m_apkParticles.size() - 1])->SetText(acTempString);

	((FloatingText*)m_apkParticles[m_apkParticles.size() - 1])->SetParticleManager(this);
}

std::vector<Particle*> ParticleManager::GetParticleList()
{
    return m_apkParticles;
}

TextLibrary* ParticleManager::GetTextLibrary()
{
	return m_pkTextLibrary;
}