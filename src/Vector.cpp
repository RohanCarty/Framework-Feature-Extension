#include "Vector.h"

#include <cmath>

//Vector Functions
Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(double a_x, double a_y, double a_z)
{
	x = a_x;
	y = a_y;
	z = a_z;
}

Vector::~Vector()
{

}

double Vector::GetMagnitude(Vector a_vVector)
{
	return sqrt( ( a_vVector.x * a_vVector.x ) + ( a_vVector.y * a_vVector.y ) + (a_vVector.z * a_vVector.z));
}

double Vector::CheckDistance(Vector a_vOtherVector)
{
	Vector TempVector;

	TempVector.x = x - a_vOtherVector.x;
	TempVector.y = y - a_vOtherVector.y;
	TempVector.z = z - a_vOtherVector.z;

	return(GetMagnitude(TempVector));
}

Vector Vector::Unitise()
{
    Vector TempVector;

	double dLength = GetMagnitude(*this);

    TempVector.x = x / dLength;
    TempVector.y = y / dLength;
	TempVector.z = z / dLength;

    return TempVector;
}

double Vector::DotProduct(Vector left, Vector right)
{
	Vector TempOutput;

	TempOutput.x = (left.x * right.x);
	TempOutput.y = (left.y * right.y);
	TempOutput.z = (left.z * right.z);

	return (TempOutput.x + TempOutput.y + TempOutput.z);
}

//Only checks X and Y, for 2D use only
bool Vector::WithinBox2D(Vector a_vOtherVector, Vector a_vSize)
{
	a_vSize = a_vSize / 2;
	if(x > a_vOtherVector.x - a_vSize.x && x < a_vOtherVector.x + a_vSize.x &&
		y > a_vOtherVector.y - a_vSize.y && y < a_vOtherVector.y + a_vSize.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Vector::Calculate2DAngle(Vector a_vOtherVector)
{
    float fTemp;
    Vector TempVector;

    TempVector = *this - a_vOtherVector;

	fTemp = (float)atan2(TempVector.x, TempVector.y);
	return -(fTemp + 3.14159f);
}

Vector Vector::operator+(Vector a_Param)
{
    Vector TempVector;

    TempVector.x = a_Param.x + x;
    TempVector.y = a_Param.y + y;
	TempVector.z = a_Param.z + z;

    return TempVector;
}

Vector Vector::operator-(Vector a_Param)
{
    Vector TempVector;

    TempVector.x = a_Param.x - x;
    TempVector.y = a_Param.y - y;
	TempVector.z = a_Param.z - z;

    return TempVector;
}

Vector Vector::operator*(double a_Param)
{
	Vector TempVector;

	TempVector = *this;

	TempVector.x *= a_Param;
	TempVector.y *= a_Param;
	TempVector.z *= a_Param;

	return TempVector;
}

Vector Vector::operator/(double a_Param)
{
	Vector TempVector;

	TempVector = *this;

	TempVector.x /= a_Param;
	TempVector.y /= a_Param;
	TempVector.z /= a_Param;

	return TempVector;
}

void Vector::operator=(Vector a_Param)
{
    x = a_Param.x;
    y = a_Param.y;
	z = a_Param.z;
}

void Vector::operator*=(int a_iParam)
{
    x *= a_iParam;
    y *= a_iParam;
	z *= a_iParam;
}

void Vector::operator*=(float a_fParam)
{
    x *= a_fParam;
    y *= a_fParam;
	z *= a_fParam;
}

void Vector::operator*=(double a_fParam)
{
    x *= a_fParam;
    y *= a_fParam;
	z *= a_fParam;
}

std::ostream& operator<<(std::ostream& os, const Vector& a_Param)
{
    os << "X: " << a_Param.x << " Y: " << a_Param.y << " Z: " << a_Param.z;
    return os;
}
