#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_
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

//Object Manager keeps track of I.D.'s of objects,

#include <vector>

//Forward Declare
class Object;

class ObjectManager
{
public:
    ObjectManager();
    virtual ~ObjectManager();

	static int GetPassport(Object* a_pkObject);

	static Object* GetObjectID(int a_iObjectPassport);

	static std::vector<Object*> m_apkObjectPassports;
};

#endif
