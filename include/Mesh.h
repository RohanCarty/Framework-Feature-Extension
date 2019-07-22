#ifndef _MESH_H_
#define _MESH_H_
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

#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include "Matrix.h"

class DisplayManager;
class Vertex;
class Texture;
class Matrix;

//Class used to store Textures as well as all functions related to manipulating them.
class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	bool LoadMesh(std::string a_szFileName);

	void SetElementCounts(std::string a_szFileName);

	void Update();

	Vertex* GetVertexArray();
	float* GetTextureUArray();
	float* GetTextureVArray();

	int GetNumberOfVertices();

	//Generates the attributes a four vertex mesh and sets the member variables to reflect it.
	//TODO: allow it to replace a loaded mesh
	void GenerateBasicMesh(float a_fSizeX, float a_fSizeY, float a_fSizeZ);

	void SetTexture(Texture* a_pTexture);

	void SetMatrix(Matrix a_mMatrix);
	Matrix GetMatrix();

private:
    std::stringstream m_szsStringStream;
    
	int m_iNumberOfVertices;

	Vertex* m_pakVertices;
	Vertex* m_pakOriginalVertices;

	Texture* m_pTexture;

	Matrix m_mObjectMatrix;

	float* m_pafU;
	float* m_pafV;
};

#endif
