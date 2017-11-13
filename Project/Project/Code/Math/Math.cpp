#include "Math.h"

//*****************************************************************************
float Math::Clamp(float value, float min, float max)
{
	if (value <= min)
		return min;
	else if (value >= max)
		return max;
	else
		return value;
}

//*****************************************************************************
float Math::Saturate(float value)
{
	return Clamp(value, 0, 1);
}

//*****************************************************************************
float Math::Interpolate(float min, float max, float gradient)
{
	return min + (max - min) * Saturate(gradient);
}

//*****************************************************************************
bool Math::Approximate(float v1, float v2)
{
	if (abs(v1 - v2) <= DELTA)
		return true;
	else
		return false;
}

//*****************************************************************************
float Math::Frac(float v)
{
	int i = (int)floor(v);
	return v - i;
}

//*****************************************************************************
float Math::Min(float x, float y)
{
	return (x < y) ? x : y;
}

//*****************************************************************************
float Math::Max(float x, float y)
{
	return (x > y) ? x : y;
}

//*****************************************************************************
float Math::Between(float value, float min, float max)
{
	return (value >= min && value <= max);
}

//*****************************************************************************
float Math::RadToDeg(float r)
{
	return 180.0f * r / PI;
}

//*****************************************************************************
float Math::DegToRad(float d)
{
	return PI * d / 180.0f;
}