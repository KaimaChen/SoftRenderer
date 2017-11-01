#pragma once

#include <cmath>
#include <cfloat>

#define PI 3.14159265359f

class Math
{
public:
	static float Clamp(float value, float min, float max);
	static float Clamp01(float value);
	static float Interpolate(float min, float max, float gradient);
};