#include "Color.h"

Color Color::white = Color(1, 1, 1, 1);
Color Color::black = Color(0, 0, 0, 1);
Color Color::red = Color(1, 0, 0, 1);
Color Color::green = Color(0, 1, 0, 1);
Color Color::blue = Color(0, 0, 1, 1);
Color Color::yellow = Color(1, 1, 0, 1);
Color Color::error = Color(1, 0, 1, 1);
Color Color::notValid = Color(false);

//*****************************************************************************
void Color::Clamp()
{
	r = Math::Saturate(r);
	g = Math::Saturate(g);
	b = Math::Saturate(b);
	a = Math::Saturate(a);
}

//*****************************************************************************
Color Color::operator* (const Color &other) const
{
	return Color(
		r * other.r,
		g * other.g,
		b * other.b,
		a * other.a
	);
}

//*****************************************************************************
Color Color::operator* (float v) const
{
	return Color(
		r * v,
		g * v,
		b * v, 
		a * v
	);
}

//*****************************************************************************
Color Color::operator+ (const Color &other) const
{
	return Color(
		r + other.r,
		g + other.g,
		b + other.b,
		a + other.a
	);
}

//*****************************************************************************
Color Color::operator/ (const Color &other) const
{
	return Color(
		r / other.r,
		g / other.g,
		b * other.b,
		a * other.a
	);
}

//*****************************************************************************
Color &Color::operator*= (float v)
{
	r *= v;
	g *= v;
	b *= v;
	a *= v;
	return *this;
}

//*****************************************************************************
Color Color::Interpolate(const Color &color0, const Color &color1, float v)
{
	return Color(
		Math::Interpolate(color0.r, color1.r, v),
		Math::Interpolate(color0.g, color1.g, v),
		Math::Interpolate(color0.b, color1.b, v),
		Math::Interpolate(color0.a, color1.a, v)
	);
}

//*****************************************************************************
Color Color::Interpolate(const Color &c0, const Color &c1, const Color &c2, float w0, float w1, float w2)
{
	float r = c0.r * w0 + c1.r * w1 + c2.r * w2;
	float g = c0.g * w0 + c1.g * w1 + c2.g * w2;
	float b = c0.b * w0 + c1.b * w1 + c2.b * w2;
	float a = c0.a * w0 + c1.a * w1 + c2.a * w2;
	return Color(r, g, b, a);
}