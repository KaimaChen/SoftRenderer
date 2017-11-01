#pragma once

#include <iostream>

class Color
{
public:
	static Color white;
	static Color black;
	static Color red;
	static Color green;
	static Color blue;	
	static Color yellow;
public:
	Color() : r(0), g(0), b(0), a(0) {}
	Color(float v) : r(v), g(v), b(v), a(v) {}
	Color(float pr, float pg, float pb, float pa) : r(pr), g(pg), b(pb), a(pa) {}

	Color operator* (const Color &other) const;
	Color operator* (const float v) const;
	Color operator+ (const Color &other) const;

	friend std::ostream &operator<<(std::ostream &s, const Color &v) { return s << "(" << v.r << " " << v.g << " " << v.b << " " << v.a << ")"; }

	float r, g, b, a;
};