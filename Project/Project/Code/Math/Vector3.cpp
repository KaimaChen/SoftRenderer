#include "Vector3.h"

Vector3 &Vector3::Normalize()
{
	float mag = SqrMagnitude();
	if (mag > 0)
	{
		float invMag = 1.0f / sqrt(mag);
		x *= invMag;
		y *= invMag;
		z *= invMag;
	}
	return *this;
}

float Vector3::Dot(const Vector3 &other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::Cross(const Vector3 &other) const
{
	return Vector3(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}

Vector3 Vector3::operator+(const Vector3 &other) const
{
	return Vector3(
		x + other.x,
		y + other.y,
		z + other.z);
}

Vector3 Vector3::operator-(const Vector3 &other) const
{
	return Vector3(
		x - other.x,
		y - other.y,
		z - other.z);
}

Vector3 Vector3::operator*(const Vector3 &other) const
{
	return Vector3(
		x * other.x,
		y * other.y,
		z * other.z);
}