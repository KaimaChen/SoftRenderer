#include "Vector4.h"
#include "Matrix4x4.h"

Vector4 Vector4::zero = Vector4(0, 0, 0);
Vector4 Vector4::one = Vector4(1, 1, 1);

Vector4 &Vector4::Normalize()
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

float Vector4::Dot(const Vector4 &other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector4 Vector4::Cross(const Vector4 &other) const
{
	return Vector4(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x,
		0.0f);
}

Vector4 Vector4::operator+(const Vector4 &other) const
{
	return Vector4(
		x + other.x,
		y + other.y,
		z + other.z,
		0.0f); //通常加之后得到的是向量
}

Vector4 Vector4::operator-(const Vector4 &other) const
{
	return Vector4(
		x - other.x,
		y - other.y,
		z - other.z,
		0.0f); //通常减之后得到的是向量
}

Vector4 Vector4::operator*(const Vector4 &other) const
{
	return Vector4(
		x * other.x,
		y * other.y,
		z * other.z,
		w * other.w);
}

Vector4 Vector4::operator*(const Matrix4x4 &other) const
{
	float v0 = x * other[0][0] + y * other[1][0] + z * other[2][0] + w * other[3][0];
	float v1 = x * other[0][1] + y * other[1][1] + z * other[2][1] + w * other[3][1];
	float v2 = x * other[0][2] + y * other[1][2] + z * other[2][2] + w * other[3][2];
	float v3 = x * other[0][3] + y * other[1][3] + z * other[2][3] + w * other[3][3];
	
	return Vector4(v0, v1, v2, v3);
}

Vector4 Vector4::operator*(const float other) const
{
	return Vector4(
		x * other,
		y * other,
		z * other,
		w * other
	);
}

Vector4 Vector4::operator/(const Vector4 &other) const
{
	return Vector4(
		x / other.x,
		y / other.y,
		z / other.z,
		w / other.w);
}

Vector4 Vector4::operator/(const float other) const
{
	return Vector4(
		x / other,
		y / other,
		z / other,
		w / other
	);
}

Vector4 Vector4::operator/=(const Vector4 &other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;
	return *this;
}

Vector4 Vector4::operator/=(const float other)
{
	x /= other;
	y /= other;
	z /= other;
	w /= other;
	return *this;
}

float Vector4::Dot(const Vector4 &lhs, const Vector4 &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector4 Vector4::Cross(const Vector4 &lhs, const Vector4 &rhs)
{
	return Vector4(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x,
		0.0f
	);
}

Vector4 Vector4::Reflect(const Vector4 &in, const Vector4 &normal)
{
	float tmp = 2.0f * (in.Dot(normal));
	return in - normal * tmp;
}