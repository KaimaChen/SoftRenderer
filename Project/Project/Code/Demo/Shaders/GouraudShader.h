#pragma once
#pragma once

#include "Graphics\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\Context.h"

class GouraudVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mProgram->GetMVP();

		Vector4 worldPos = appdata.position * mProgram->GetWorldMat();
		Vector4 worldNormal = Vector4::Normalize(appdata.normal * mProgram->GetITWorldMat());
		Vector4 worldLightDir = Vector4::Normalize(mProgram->GetLight().position - worldPos);
		Color albedo = Color::yellow;
		Color ambient = albedo * Color(0.3f, 0.3f, 0.3f, 1.0f);
		Color diffuse = albedo * mProgram->GetLight().color * Math::Clamp01(Vector4::Dot(worldNormal, worldLightDir));
		v2f.color = ambient + diffuse;

		return v2f;
	}
};

class GouraudFragmentShader : public FragmentShader
{
public:
	Color Execute(const VertexOut &v2f) override
	{
		return v2f.color;
	}
};