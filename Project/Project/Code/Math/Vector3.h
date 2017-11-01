#pragma once

#include <cmath>
#include <iostream>

class Vector3
{
public:
	static Vector3 zero;
	static Vector3 one;
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float v) : x(v), y(v), z(v) {}
	Vector3(float px, float py, float pz) : x(px), y(py), z(pz) {}

	float Magnitude() const { return sqrt(x * x + y * y + z * z); }
	float SqrMagnitude() const { return x * x + y * y + z * z; }
	Vector3 &Normalize();
	float Dot(const Vector3 &other) const;
	Vector3 Cross(const Vector3 &other) const;

	Vector3 operator+(const Vector3 &other) const;
	Vector3 operator-(const Vector3 &other) const;
	Vector3 operator*(const Vector3 &other) const;
	Vector3 operator/(const Vector3 &other) const;
	const float &operator[](unsigned i) const { return (&x)[i]; }
	float &operator[](unsigned i) { return (&x)[i]; }

	static float Dot(const Vector3 &lhs, const Vector3 &rhs);

	friend std::ostream &operator<<(std::ostream &s, const Vector3 &v) { return s << "(" << v.x << " " << v.y << " " << v.z << ")"; }

	float x, y, z;
};