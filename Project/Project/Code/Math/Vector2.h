#pragma once

#include <iostream>
#include "Math.h"

class Vector2
{
public:
	static Vector2 zero;
	static Vector2 one;
public:
	Vector2() : x(0), y(0) {}
	Vector2(float px, float py) : x(px), y(py) {}

	Vector2 &operator*= (float v);

	friend std::ostream &operator<<(std::ostream &s, const Vector2 &v) { return s << "(" << v.x << " " << v.y << ")"; }

	static Vector2 Interpolate(const Vector2 &v0, const Vector2 &v1, float v);

	float x, y;
};