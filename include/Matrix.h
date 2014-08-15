#ifndef _MATRIX_H_
#define _MATRIX_H_
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

#include <cmath>
#include "Vector.h"
#include "Vertex.h"

//Class used to store 2D vectors as well as all functions related to manipulating them.
class Matrix
{
public:
	Matrix();
	~Matrix();

	void CreateIdentity();

	void XAxisRotation(float a_fRadians);

	void YAxisRotation(float a_fRadians);

	void ZAxisRotation(float a_fRadians);

	//Scale by percentage (1.0 is 100% of scale)
	void UniformScale(float a_fScale);

	void SetTranslation(Vector a_vLocation);
	Vector GetTranslation();

	void SetFrustrum(float a_fLeft, float a_fRight, float a_fBottom, float a_fTop, float a_fNear, float a_fFar);
	void SetPersepective(float a_fFoVY, float a_fAspect, float a_fNear, float a_fFar);

	void SetOrthographicFrustrum(float a_fLeft, float a_fRight, float a_fBottom, float a_fTop, float a_fNear, float a_fFar);

	void TransformVertices(Vertex* a_apVertices, unsigned int a_uiNumberOfVertices);

	Vector MultiplyVector (Vector* param);

	Vector operator * (const Vector& right);

	void operator = (const Matrix& right);

	double CoOrd[4][4];
};

#endif
