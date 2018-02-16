#include "Matrix.h"

#include <cmath>

//Matrix Functions
Matrix::Matrix()
{
	CreateIdentity();
}

Matrix::~Matrix()
{

}

void Matrix::CreateIdentity()
{
	CoOrd[0][0] = 1;
	CoOrd[0][1] = 0;
	CoOrd[0][2] = 0;
	CoOrd[0][3] = 0;
	CoOrd[1][0] = 0;
	CoOrd[1][1] = 1;
	CoOrd[1][2] = 0;
	CoOrd[1][3] = 0;
	CoOrd[2][0] = 0;
	CoOrd[2][1] = 0;
	CoOrd[2][2] = 1;
	CoOrd[2][3] = 0;
	CoOrd[3][0] = 0;
	CoOrd[3][1] = 0;
	CoOrd[3][2] = 0;
	CoOrd[3][3] = 1;
}

Matrix Matrix::GetIdentity()
{
	Matrix temp;
	temp.CoOrd[0][0] = 1;
	temp.CoOrd[0][1] = 0;
	temp.CoOrd[0][2] = 0;
	temp.CoOrd[0][3] = 0;
	temp.CoOrd[1][0] = 0;
	temp.CoOrd[1][1] = 1;
	temp.CoOrd[1][2] = 0;
	temp.CoOrd[1][3] = 0;
	temp.CoOrd[2][0] = 0;
	temp.CoOrd[2][1] = 0;
	temp.CoOrd[2][2] = 1;
	temp.CoOrd[2][3] = 0;
	temp.CoOrd[3][0] = 0;
	temp.CoOrd[3][1] = 0;
	temp.CoOrd[3][2] = 0;
	temp.CoOrd[3][3] = 1;

	return temp;
}

void Matrix::XAxisRotation(float a_fRadians)
{
	//    X  Y
	CoOrd[1][1] = cosf(a_fRadians);
	CoOrd[2][1] = -sinf(a_fRadians);
	CoOrd[1][2] = sinf(a_fRadians);
	CoOrd[2][2] = cosf(a_fRadians);
}

void Matrix::YAxisRotation(float a_fRadians)
{
	//    X  Y
	CoOrd[0][0] = cosf(a_fRadians);
	CoOrd[2][0] = -sinf(a_fRadians);
	CoOrd[0][2] = sinf(a_fRadians);
	CoOrd[2][2] = cosf(a_fRadians);
}

void Matrix::ZAxisRotation(float a_fRadians)
{
	//    X  Y
	CoOrd[0][0] = cosf(a_fRadians);
	CoOrd[1][0] = -sinf(a_fRadians);
	CoOrd[0][1] = sinf(a_fRadians);
	CoOrd[1][1] = cosf(a_fRadians);
}

//Scale by percentage (1.0 is 100% of scale)
void Matrix::UniformScale(float a_fScale)
{
	CoOrd[0][0] *= a_fScale;
	CoOrd[0][1] *= a_fScale;
	CoOrd[0][2] *= a_fScale;
	CoOrd[0][3] *= a_fScale;
	CoOrd[1][0] *= a_fScale;
	CoOrd[1][1] *= a_fScale;
	CoOrd[1][2] *= a_fScale;
	CoOrd[1][3] *= a_fScale;
	CoOrd[2][0] *= a_fScale;
	CoOrd[2][1] *= a_fScale;
	CoOrd[2][2] *= a_fScale;
	CoOrd[2][3] *= a_fScale;
	CoOrd[3][0] *= a_fScale;
	CoOrd[3][1] *= a_fScale;
	CoOrd[3][2] *= a_fScale;
	CoOrd[3][3] *= a_fScale;
}

void Matrix::SetTranslation(Vector a_vLocation)
{
    CoOrd[3][0] = a_vLocation.x;
    CoOrd[3][1] = a_vLocation.y;
	CoOrd[3][2] = a_vLocation.z;
}

Vector Matrix::GetTranslation()
{
	Vector vTemp;
    vTemp.x = CoOrd[3][0];
    vTemp.y = CoOrd[3][1];
	vTemp.z = CoOrd[3][2];

	return vTemp;
}

void Matrix::SetFrustrum(float a_fLeft, float a_fRight, float a_fBottom, float a_fTop, float a_fNear, float a_fFar)
{
	CoOrd[0][0] = (2.0f * a_fNear) / (a_fRight - a_fLeft);
	CoOrd[1][1] = (2.0f * a_fNear) / (a_fTop - a_fBottom);
	CoOrd[2][0] = (a_fRight + a_fLeft) / (a_fRight - a_fLeft);
	CoOrd[2][1] = (a_fTop + a_fBottom) / (a_fTop - a_fBottom);
	CoOrd[2][2] = -(a_fFar + a_fNear) / (a_fFar - a_fNear);
	CoOrd[2][3] = -1.0;
	CoOrd[3][2] = -(2.0f * a_fFar * a_fNear) / (a_fFar - a_fNear);
	CoOrd[3][3] = 0.0;
}
								//degrees
void Matrix::SetPersepective(float a_fFoVY, float a_fAspect, float a_fNear, float a_fFar)
{
	float top = a_fNear * (float)tan((0.5f * a_fFoVY) * (3.14159f / 180.0f));
	float right = top * a_fAspect;
	SetFrustrum(-right, right, -top, top, a_fNear, a_fFar);
}

void Matrix::SetOrthographicFrustrum(float a_fLeft, float a_fRight, float a_fBottom, float a_fTop, float a_fNear, float a_fFar)
{
    //TODO: Relook up the maths for this, because something's wrong here (5x5 matrix?)
	//CoOrd[0][0] = 1 / ((a_fLeft + a_fRight) / 2);
	//CoOrd[1][1] = 1 / ((a_fBottom + a_fTop) / 2);
	//CoOrd[2][2] = -(1 / (a_fFar - a_fNear) / 2);
	//CoOrd[4][3] = -(a_fNear + a_fFar) / a_fFar - a_fNear;
	//CoOrd[4][4] = 1.0;
}

void Matrix::TransformVertices(Vertex* a_apVertices, unsigned int a_uiNumberOfVertices)
{
    //TODO


	for(unsigned int iDx = 0; iDx < a_uiNumberOfVertices; iDx++)
	{
		*a_apVertices[iDx].GetLocation() = MultiplyVector(a_apVertices[iDx].GetLocation()); //Apply Rotation
		a_apVertices[iDx].GetLocation()->x = a_apVertices[iDx].GetLocation()->x +CoOrd[3][0]; //apply Translation
		a_apVertices[iDx].GetLocation()->y = a_apVertices[iDx].GetLocation()->y +CoOrd[3][1];
	}
	/*
    *a_apVertices[1].GetLocation() = MultiplyVector(a_apVertices[1].GetLocation()); //Apply Rotation
	a_apVertices[1].GetLocation()->x = a_apVertices[1].GetLocation()->x +CoOrd[3][0]; //apply Translation
	a_apVertices[1].GetLocation()->y = a_apVertices[1].GetLocation()->y +CoOrd[3][1];

    *a_apVertices[2].GetLocation() = MultiplyVector(a_apVertices[2].GetLocation()); //Apply Rotation
	a_apVertices[2].GetLocation()->x = a_apVertices[2].GetLocation()->x +CoOrd[3][0]; //apply Translation
	a_apVertices[2].GetLocation()->y = a_apVertices[2].GetLocation()->y +CoOrd[3][1];

    *a_apVertices[3].GetLocation() = MultiplyVector(a_apVertices[3].GetLocation()); //Apply Rotation
	a_apVertices[3].GetLocation()->x = a_apVertices[3].GetLocation()->x +CoOrd[3][0]; //apply Translation
	a_apVertices[3].GetLocation()->y = a_apVertices[3].GetLocation()->y +CoOrd[3][1];*/

	return;
}

Vector Matrix::MultiplyVector (Vector* param)
{
	Vector Temp;

	/*Temp.x = ((param->x * CoOrd[0][0]) - (param->y * CoOrd[1][0]));
	Temp.y = ((param->x * CoOrd[1][0]) + (param->y * CoOrd[1][1]));*/
	Temp.x = ((param->x * CoOrd[0][0] + (param->y * CoOrd[1][0]) + (param->z * CoOrd[2][0])));
	Temp.y = ((param->x * CoOrd[0][1] + (param->y * CoOrd[1][1]) + (param->z * CoOrd[2][1])));
	Temp.z = ((param->x * CoOrd[0][2] + (param->y * CoOrd[1][2]) + (param->z * CoOrd[2][2])));

	return Temp;
}

Vector Matrix::operator * (const Vector& param)
{
	Vector Temp;

	Temp.x = ((param.x * CoOrd[0][0]) + (param.y * CoOrd[1][0]));
	Temp.y = ((param.x * CoOrd[1][0]) + (param.y * CoOrd[1][1]));

	return Temp;
}

void Matrix::operator = (const Matrix& param)
{
	CoOrd[0][0] = param.CoOrd[0][0];
	CoOrd[1][0] = param.CoOrd[1][0];
	CoOrd[2][0] = param.CoOrd[2][0];
	CoOrd[3][0] = param.CoOrd[3][0];
	CoOrd[0][1] = param.CoOrd[0][1];
	CoOrd[1][1] = param.CoOrd[1][1];
	CoOrd[2][1] = param.CoOrd[2][1];
	CoOrd[3][1] = param.CoOrd[3][1];
	CoOrd[0][2] = param.CoOrd[0][2];
	CoOrd[1][2] = param.CoOrd[1][2];
	CoOrd[2][2] = param.CoOrd[2][2];
	CoOrd[3][2] = param.CoOrd[3][2];
	CoOrd[0][3] = param.CoOrd[0][3];
	CoOrd[1][3] = param.CoOrd[1][3];
	CoOrd[2][3] = param.CoOrd[2][3];
	CoOrd[3][3] = param.CoOrd[3][3];
}

#ifdef _WIN32
void Matrix::operator = (const DirectX::XMFLOAT4X4& param)
{
	CoOrd[0][0] = param._11;
	CoOrd[1][0] = param._21;
	CoOrd[2][0] = param._31;
	CoOrd[3][0] = param._41;
	CoOrd[0][1] = param._12;
	CoOrd[1][1] = param._22;
	CoOrd[2][1] = param._32;
	CoOrd[3][1] = param._42;
	CoOrd[0][2] = param._13;
	CoOrd[1][2] = param._23;
	CoOrd[2][2] = param._33;
	CoOrd[3][2] = param._43;
	CoOrd[0][3] = param._14;
	CoOrd[1][3] = param._24;
	CoOrd[2][3] = param._34;
	CoOrd[3][3] = param._44;
}

Matrix::operator DirectX::XMFLOAT4X4()
{
	DirectX::XMFLOAT4X4 tempMatrix;

	tempMatrix._11 = CoOrd[0][0];
	tempMatrix._21 = CoOrd[1][0];
	tempMatrix._31 = CoOrd[2][0];
	tempMatrix._41 = CoOrd[3][0];
	tempMatrix._12 = CoOrd[0][1];
	tempMatrix._22 = CoOrd[1][1];
	tempMatrix._32 = CoOrd[2][1];
	tempMatrix._42 = CoOrd[3][1];
	tempMatrix._13 = CoOrd[0][2];
	tempMatrix._23 = CoOrd[1][2];
	tempMatrix._33 = CoOrd[2][2];
	tempMatrix._43 = CoOrd[3][2];
	tempMatrix._14 = CoOrd[0][3];
	tempMatrix._24 = CoOrd[1][3];
	tempMatrix._34 = CoOrd[2][3];
	tempMatrix._44 = CoOrd[3][3];

	return tempMatrix;
}
#endif //_WIN32