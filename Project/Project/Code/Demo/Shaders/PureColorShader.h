#pragma once

#include "Graphics\Shaders\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"

class PureColorVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mProgram->GetMatrix(MVP_MAT_INDEX);
		return v2f;
	}
};

class PureColorFragmentShader : public FragmentShader
{
public:
	Color Execute(const VertexOut &v2f) override
	{
		return mProgram->GetColor();
	}
};