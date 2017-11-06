#pragma once

#include "Graphics\Shader.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\RenderManager.h"

class DiffuseShader : public Shader
{
public:
	VertexOut VertexShader(const VertexIn &appdata)
	{
		VertexOut v2f = VertexOut();
		v2f.worldPos = appdata.position * mWorldMat;
		v2f.clipPos = appdata.position * mMVP;
		v2f.color = appdata.color;
		v2f.uv = appdata.uv;
		v2f.normal = appdata.normal * mITWorldMat;
		return v2f;
	}

	Color FragmentShader(VertexOut &v2f)
	{
		Vector4 worldNormal = Vector4::Normalize(v2f.normal);
		Vector4 worldLightDir = Vector4::Normalize(mLight.position - v2f.worldPos);

		Color albedo = RenderManager::Instance()->GetTexture0()->Read(v2f.uv);
		Color ambient = albedo * Color(0.3f, 0.3f, 0.3f, 1.0f);
		Color diffuse = albedo * mLight.color * Math::Clamp01(Vector4::Dot(worldNormal, worldLightDir));
		Color finalColor = ambient + diffuse;

		return finalColor;
	}
};