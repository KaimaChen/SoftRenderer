#pragma once

#include <cmath>
#include <iostream>

class Matrix4x4;

//Ä¬ÈÏw = 1
class Vector4
{
public:
	static Vector4 zero;
	static Vector4 one;
public:
	Vector4() : x(0), y(0), z(0), w(1) {}
	explicit Vector4(float v) : x(v), y(v), z(v), w(v) {}
	Vector4(float px, float py, float pz) : x(px), y(py), z(pz), w(1) {}
	Vector4(float px, float py, float pz, float pw) : x(px), y(py), z(pz), w(pw) {}

	float Magnitude() const { return sqrt(x * x + y * y + z * z); }
	float SqrMagnitude() const { return x * x + y * y + z * z; }
	Vector4 &Normalize();
	float Dot(const Vector4 &other) const;
	Vector4 Cross(const Vector4 &other) const;

	Vector4 operator+(const Vector4 &other) const;
	Vector4 operator-(const Vector4 &other) const;
	Vector4 operator*(const Vector4 &other) const;
	Vector4 operator*(const Matrix4x4 &other) const;
	Vector4 operator*(const float other) const;
	Vector4 operator/(const Vector4 &other) const;
	Vector4 operator/(const float other) const;
	Vector4 operator/=(const Vector4 &other);
	Vector4 operator/=(const float other);
	const float &operator[](unsigned i) const { return (&x)[i]; }
	float &operator[](unsigned i) { return (&x)[i]; }

	static float Dot(const Vector4 &lhs, const Vector4 &rhs);
	static Vector4 Cross(const Vector4 &lhs, const Vector4 &rhs);
	static Vector4 Reflect(const Vector4 &in, const Vector4 &normal);

	friend std::ostream &operator<<(std::ostream &s, const Vector4 &v) { return s << "(" << v.x << " " << v.y << " " << v.z << " " << v.w << ")"; }

	float x, y, z, w;
};