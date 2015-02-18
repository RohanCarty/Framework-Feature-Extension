#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_
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

//Tile Manager deals with generating/decoding the level and spawning the tiles and resources

#include <vector>

#include "Vector.h"

//Forward Declare
class Object;
class Particle;
class Resource;

class ParticleManager
{
public:
    ParticleManager();
    virtual ~ParticleManager();
    
    bool Update(float a_fDeltaTime);
    void SpawnParticle(Vector a_vDestination);
	void SpawnFloatingText(Vector a_vDestination, std::string a_szTextToDisplay);
	void SpawnFloatingText(Vector a_vDestination, int a_iNumberToDisplay);

    std::vector<Particle*> GetParticleList();
private:
    std::vector<Particle*> m_apkParticles;
};

#endif
