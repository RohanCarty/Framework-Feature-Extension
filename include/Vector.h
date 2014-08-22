#ifndef _VECTOR_H_
#define _VECTOR_H_
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

#include <math.h>

//Class used to store 2D vectors as well as all functions related to manipulating them.
class Vector
{
public:
	Vector();
	Vector(double a_x, double a_y, double a_z);
	~Vector();

	double GetMagnitude(Vector a_vVector);

	double CheckDistance(Vector a_vOtherVector);

	Vector Unitise();

	float DotProduct(Vector left, Vector right);
	
	bool WithinBox(Vector a_vOtherVector, float a_fSize);

	float Calculate2DAngle(Vector a_vOtherVector);

    Vector operator+(Vector a_Param);
	Vector operator-(Vector a_Param);
	Vector operator*(double a_Param);
	Vector operator/(double a_Param);
	void operator=(Vector a_Param);
	void operator*=(int a_iParam);
	void operator*=(float a_fParam);
	void operator*=(double a_fParam);

	

	double x;
	double y;
	double z;
};

#endif
