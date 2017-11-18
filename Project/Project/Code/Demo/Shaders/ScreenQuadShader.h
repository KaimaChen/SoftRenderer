#pragma once

#include "Graphics\Shaders\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\Context.h"

class ScreenQuadVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position;
		v2f.uv = appdata.uv;
		return v2f;
	}
};

class ScreenQuadFragmentShader : public FragmentShader
{
public:
	Color Execute(const VertexOut &v2f) override
	{
		Color albedo = Context::Instance()->GetTexture0()->Read(v2f.uv);
		return albedo;
	}
};