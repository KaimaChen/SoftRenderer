#pragma once

#include "Graphics\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\Context.h"

class PartScreenQuadVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mProgram->GetMatrix(afterMVPMatIndex);
		v2f.uv = appdata.uv;
		return v2f;
	}

	int afterMVPMatIndex = 3;
};

class PartScreenQuadFragmentShader : public FragmentShader
{
public:
	Color Execute(const VertexOut &v2f) override
	{
		Color albedo = Context::Instance()->GetTexture0()->Read(v2f.uv);
		return albedo;
	}
};