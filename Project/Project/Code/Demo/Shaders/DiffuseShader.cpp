#include "DiffuseShader.h"
#include "Managers\RenderManager.h"

VertexOut DiffuseShader::VertexShader(const VertexIn &appdata)
{
	VertexOut v2f = VertexOut();
	v2f.worldPos = appdata.position * mWorldMat;
	v2f.clipPos = appdata.position * mMVP;
	v2f.color = appdata.color;
	v2f.uv = appdata.uv;
	v2f.normal = appdata.normal * mITWorldMat;
	return v2f;
}

Color DiffuseShader::FragmentShader(VertexOut &v2f)
{
	Vector4 worldNormal = Vector4::Normalize(v2f.normal);
	Vector4 worldLightDir = Vector4::Normalize(mLight.position - v2f.worldPos);

	Color albedo = RenderManager::Instance()->GetCurrentTexture()->Read(v2f.uv);
	Color ambient = albedo * Color(0.3f);
	Color diffuse = albedo * mLight.color * Math::Clamp01(Vector4::Dot(worldNormal, worldLightDir));
	Color finalColor = ambient + diffuse;

	return finalColor;
}