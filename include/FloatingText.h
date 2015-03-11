#ifndef _FLOATINGTEXT_H_
#define _FLOATINGTEXT_H_
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
class TextLibrary;
class ParticleManager;

//Floating Text for damage and the like

#include "Particle.h"

class FloatingText : public Particle
{
public:
    FloatingText(Scene* a_pkScene);
    virtual ~FloatingText();

    virtual bool Update(float a_fDeltaTime);	
	TextLibrary* GetTextLibrary();
	void SetParticleManager(ParticleManager* a_pkParticleManagerReference);

	void SetText(std::string a_szNewText);

private:
    Scene* m_pkScene;
    std::string m_szTempString;
	ParticleManager* m_pkParticleManagerReference;
};

#endif
