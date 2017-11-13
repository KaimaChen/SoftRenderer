#pragma once

#include <iostream>
#include "Math\Math.h"

class Color
{
public:
	static Color white;
	static Color black;
	static Color red;
	static Color green;
	static Color blue;	
	static Color yellow;
	static Color error;
	static Color notValid;
public:
	Color() : r(0), g(0), b(0), a(0) {}
	Color(float pr, float pg, float pb, float pa) : r(pr), g(pg), b(pb), a(pa) {}
	explicit Color(float val) : r(val), g(val), b(val), a(val) {}
	explicit Color(bool valid) : isValid(valid) {}

	void Clamp();

	Color operator* (const Color &other) const;
	Color operator* (float v) const;
	Color operator+ (const Color &other) const;
	Color operator/ (const Color &other) const;
	Color &operator*= (float v);

	friend std::ostream &operator<<(std::ostream &s, const Color &v) { return s << "(" << v.r << " " << v.g << " " << v.b << " " << v.a << ")"; }

	static Color Interpolate(const Color &color0, const Color &color1, float v);

	float r, g, b, a;
	bool isValid = true;
};