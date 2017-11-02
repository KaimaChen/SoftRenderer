#pragma once

#include "Math\Vector4.h"
#include "Math\Matrix4x4.h"

struct Camera
{
	Matrix4x4 ViewMat() { return Matrix4x4::LookAtRH(eye, at, up); }
	Matrix4x4 PerspectiveMat() { return Matrix4x4::PerspectiveFovRH(fov, aspect, zn, zf); }

	Vector4 eye;
	Vector4 at;
	Vector4 up;

	float fov;
	float aspect;
	float zn;
	float zf;
};