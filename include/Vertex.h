#ifndef _VERTEX_H_
#define _VERTEX_H_
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

#include "Vector.h"

class Texture;

//Class used to store 2D Vertexs as well as all functions related to manipulating them.
class Vertex
{
public:
	Vertex();
	~Vertex();

	Vector* GetLocation();
	void SetLocation(Vector a_vNewLocation);
private:
    Vector* m_pLocation;
};

#endif
