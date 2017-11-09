#pragma once

#include "Graphics\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\Context.h"

class DiffuseVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.worldPos = appdata.position * mProgram->GetWorldMat();
		v2f.clipPos = appdata.position * mProgram->GetMVP();
		v2f.color = appdata.color;
		v2f.uv = appdata.uv;
		v2f.normal = appdata.normal * mProgram->GetITWorldMat();
		return v2f;
	}
};

class DiffuseFragmentShader : public FragmentShader
{
public:
	Color Execute(const VertexOut &v2f) override
	{
		Vector4 worldNormal = Vector4::Normalize(v2f.normal);
		Vector4 worldLightDir = Vector4::Normalize(mProgram->GetLight().position - v2f.worldPos);

		Color albedo = Context::Instance()->GetTexture0()->Read(v2f.uv);
		Color ambient = albedo * Color(0.3f, 0.3f, 0.3f, 1.0f);
		Color diffuse = albedo * mProgram->GetLight().color * Math::Clamp01(Vector4::Dot(worldNormal, worldLightDir));
		Color finalColor = ambient + diffuse;

		return finalColor;
	}
};