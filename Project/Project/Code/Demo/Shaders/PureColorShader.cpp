#include "PureColorShader.h"

VertexOut PureColorShader::VertexShader(const VertexIn &appdata)
{
	VertexOut v2f = VertexOut();
	v2f.clipPos = appdata.position * mMVP;
	return v2f;
}

Color PureColorShader::FragmentShader(VertexOut &v2f)
{
	return Color::blue;
}