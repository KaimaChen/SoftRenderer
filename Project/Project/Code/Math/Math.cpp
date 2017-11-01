#include "Math.h"

float Math::Clamp(float value, float min, float max)
{
	return fmax(min, fmin(value, max));
}

float Math::Clamp01(float value)
{
	return Clamp(value, 0, 1);
}

float Math::Interpolate(float min, float max, float gradient)
{
	return min + (max - min) * Clamp01(gradient);
}