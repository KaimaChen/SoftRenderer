#pragma once
#pragma once

#include "Graphics\Shader.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\RenderManager.h"

class GouraudShader : public Shader
{
public:
	VertexOut VertexShader(const VertexIn &appdata)
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mMVP;

		Vector4 worldPos = appdata.position * mWorldMat;
		Vector4 worldNormal = Vector4::Normalize(appdata.normal * mITWorldMat);
		Vector4 worldLightDir = Vector4::Normalize(mLight.position - worldPos);
		Color albedo = Color::yellow;
		Color ambient = albedo * Color(0.3f, 0.3f, 0.3f, 1.0f);
		Color diffuse = albedo * mLight.color * Math::Clamp01(Vector4::Dot(worldNormal, worldLightDir));
		v2f.color = ambient + diffuse;

		return v2f;
	}

	Color FragmentShader(VertexOut &v2f)
	{
		return v2f.color;
	}
};