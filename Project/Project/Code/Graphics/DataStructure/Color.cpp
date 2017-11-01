#include "Color.h"

Color Color::white = Color(1, 1, 1, 1);
Color Color::black = Color(0, 0, 0, 1);
Color Color::red = Color(1, 0, 0, 1);
Color Color::green = Color(0, 1, 0, 1);
Color Color::blue = Color(0, 0, 1, 1);
Color Color::yellow = Color(1, 1, 0, 1);

Color Color::operator* (const Color &other) const
{
	return Color(
		r * other.r,
		g * other.g,
		b * other.b,
		a * other.a
	);
}

Color Color::operator* (const float v) const
{
	return Color(
		r * v,
		g * v,
		b * v, 
		a * v
	);
}

Color Color::operator+ (const Color &other) const
{
	return Color(
		r + other.r,
		g + other.g,
		b + other.b,
		a + other.a
	);
}