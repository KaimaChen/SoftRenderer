#pragma once

#include "Graphics\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\Context.h"

class AlphaBlendVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mProgram->GetMatrix(MVP_MAT_INDEX);
		return v2f;
	}
};

class AlphaBlendFragmentShader : public FragmentShader
{
public:
	Color Execute(const VertexOut &v2f) override
	{
		return Color(1, 0, 0, 0.5);
	}
};