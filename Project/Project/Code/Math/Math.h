#pragma once

#include <cmath>
#include <cfloat>

#define PI 3.14159265359f
#define DELTA 0.000001f

class Math
{
public:
	static float Clamp(float value, float min, float max);
	static float Clamp01(float value);
	static float Interpolate(float min, float max, float gradient);
	static bool Approximate(float v1, float v2);
	static float Frac(float v);
	static float Min(float x, float y);
	static float Max(float x, float y);
};