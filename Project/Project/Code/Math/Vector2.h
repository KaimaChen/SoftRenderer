#pragma once

class Vector2
{
public:
	static Vector2 zero;
	static Vector2 one;
public:
	Vector2() : x(0), y(0) {}
	Vector2(float px, float py) : x(px), y(py) {}
	float x, y;
};