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
	Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}
	explicit Color(float val) : r(val), g(val), b(val), a(val) {}
	explicit Color(bool valid) : isValid(valid) {}

	void Clamp();

	Color operator* (const Color &other) const;
	Color operator* (float v) const;
	Color operator+ (const Color &other) const;
	Color operator/ (const Color &other) const;
	Color &operator*= (float v);
	const float &operator[](unsigned i) const { return (&r)[i]; }
	float &operator[](unsigned i) { return (&r)[i]; }

	friend std::ostream &operator<<(std::ostream &s, const Color &v) { return s << "(" << v.r << " " << v.g << " " << v.b << " " << v.a << ")"; }

	static Color Interpolate(const Color &color0, const Color &color1, float v);
	static Color Interpolate(const Color &c0, const Color &c1, const Color &c2, float w0, float w1, float w2);

	float r, g, b, a;
	bool isValid = true;
};

//*****************************************************************************
inline Color operator-(float k, const Color &c)
{
	return Color(k - c.r, k - c.g, k - c.b, k - c.a);
}