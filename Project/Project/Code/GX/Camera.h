#pragma once

#include "Math/Vector3.h"

class Camera
{
public:
	Vector3 GetEye() const { return mEye; }
	Vector3 GetAt() const { return mAt; }
	Vector3 GetUp() const { return mUp; }
private:
	Vector3 mEye;
	Vector3 mAt;
	Vector3 mUp;
};