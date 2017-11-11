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