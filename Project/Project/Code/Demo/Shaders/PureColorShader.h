#pragma once

#include "Graphics\Shader.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"

class PureColorShader : public Shader
{
public:
	PureColorShader(Color color = Color::white) : mColor(color) {}

	VertexOut VertexShader(const VertexIn &appdata)
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mMVP;
		return v2f;
	}

	Color FragmentShader(VertexOut &v2f)
	{
		return mColor;
	}

private:
	Color mColor;
};