#include "VECTOR3D.h"

float VECTOR3D::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

float VECTOR3D::InnerProduct(const VECTOR3D& v)
{
	return (x * v.x + y * v.y + z * v.z);
}

VECTOR3D VECTOR3D::CrossProduct(const VECTOR3D& v)
{
	VECTOR3D result;
	result.x = y * v.z - z * v.y;
	result.y = z * v.x - x * v.z;
	result.z = x * v.y - y * v.x;

	return result;
}

VECTOR3D VECTOR3D::Normalize()
{
	float w = Magnitude();
	if (w < 0.00001) return *this;
	x /= w;
	y /= w;
	z /= w;
	return *this;
}

VECTOR3D VECTOR3D::operator+(const VECTOR3D& v)
{
	VECTOR3D result = (*this);
	result.x += v.x;
	result.y += v.y;
	result.z += v.z;

	return result;
}

void VECTOR3D::operator+=(const VECTOR3D& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

VECTOR3D VECTOR3D::operator-(const VECTOR3D& v)
{
	VECTOR3D result = (*this);
	result.x -= v.x;
	result.y -= v.y;
	result.z -= v.z;

	return result;
}

VECTOR3D VECTOR3D::operator-()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;
	return (*this);
}

VECTOR3D VECTOR3D::operator*(const float& val)
{
	VECTOR3D result = (*this);
	result.x *= val;
	result.y *= val;
	result.z *= val;

	return result;
}

VECTOR3D VECTOR3D::operator/(const float& val)
{
	VECTOR3D result = (*this);
	result.x /= val;
	result.y /= val;
	result.z /= val;

	return result;
}

VECTOR3D VECTOR3D::operator/=(const float& val)
{
	this->x /= val;
	this->y /= val;
	this->z /= val;

	return *this;
}