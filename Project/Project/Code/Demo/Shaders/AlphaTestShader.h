#pragma once

#include "Graphics\Shaders\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\Context.h"

class AlphaTestVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mProgram->GetMatrix(MVP_MAT_INDEX);
		v2f.uv = appdata.uv;
		return v2f;
	}
};

class AlphaTestFragmentShader : public FragmentShader
{
public:
	Color Execute(const VertexOut &v2f) override
	{
		Color albedo = Context::Instance()->GetTexture0()->Read(v2f.uv);
		Color noise = Context::Instance()->GetTexture1()->Read(v2f.uv);
		if (noise.r < 0.5f)
			discard;

		return albedo;
	}
};