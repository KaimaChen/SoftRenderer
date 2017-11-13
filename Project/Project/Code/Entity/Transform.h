#pragma once

#include "Math\Vector4.h"
#include "Math\Quaternion.h"

class Transform
{
public:
	Transform(Vector4 pos, Quaternion rot, Vector4 scale);

private:
	Vector4 mPosition;
	Quaternion mRotation;
	Vector4 mScale;
};