#pragma once

#include <iostream>
#include <iomanip>
#include "Vector4.h"

//Row Major
class Matrix4x4
{
public:
	static Matrix4x4 zero;
	static Matrix4x4 identity;
	
public:
	Matrix4x4() {}	
	Matrix4x4(float v00, float v01, float v02, float v03,
					float v10, float v11, float v12, float v13,
					float v20, float v21, float v22, float v23,
					float v30, float v31, float v32, float v33);

	const float* operator[] (unsigned i) const { return m[i]; }
	float* operator[] (unsigned i) { return m[i]; }
	Matrix4x4 operator*(const Matrix4x4 &rhs) const;

	Matrix4x4 Transposed() const;
	Matrix4x4 Transpose();
	Matrix4x4 Inverse() const;
	Matrix4x4 InverseTranspose() const;
	const Matrix4x4 &Invert() { *this = Inverse(); return *this; }

	static Matrix4x4 Scale(float sx, float sy, float sz);
	static Matrix4x4 Translate(float tx, float ty, float tz);
	static Matrix4x4 RotateX(float radians);
	static Matrix4x4 RotateY(float radians);
	static Matrix4x4 RotateZ(float radians);
	static Matrix4x4 LookAtLH(const Vector4 &eye, const Vector4 &at, const Vector4 &up);
	static Matrix4x4 LookAtRH(const Vector4 &eye, const Vector4 &at, const Vector4 &up);
	static Matrix4x4 PerspectiveLH(float w, float h, float zn, float zf);
	static Matrix4x4 PerspectiveRH(float w, float h, float zn, float zf);
	static Matrix4x4 PerspectiveFovLH(float fov, float aspect, float zn, float zf);
	static Matrix4x4 PerspectiveFovRH(float fov, float aspect, float zn, float zf);
	static Matrix4x4 ScreenTransform(int screenWidth, int screenHeight);

	friend std::ostream &operator<<(std::ostream &s, const Matrix4x4 &m);

private:
	float m[4][4] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} };
};