#include "Vector2.h"

Vector2 Vector2::zero = Vector2(0, 0);
Vector2 Vector2::one = Vector2(1, 1);

//*****************************************************************************
Vector2 &Vector2::operator*= (float v)
{
	x *= v;
	y *= v;
	return *this;
}

//*****************************************************************************
Vector2 Vector2::Interpolate(const Vector2 &v0, const Vector2 &v1, float v)
{
	return Vector2(
		Math::Interpolate(v0.x, v1.x, v),
		Math::Interpolate(v0.y, v1.y, v)
	);
}

//*****************************************************************************
Vector2 Vector2::Interpolate(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, float w0, float w1, float w2)
{
	float x = v0.x * w0 + v1.x * w1 + v2.x * w2;
	float y = v0.y * w0 + v1.y * w1 + v2.y * w2;
	return Vector2(x, y);
}