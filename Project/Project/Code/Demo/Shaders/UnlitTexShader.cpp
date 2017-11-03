#include "UnlitTexShader.h"
#include "Managers\RenderManager.h"

VertexOut UnlitTexShader::VertexShader(const VertexIn &appdata)
{
	VertexOut v2f = VertexOut();
	//v2f.clipPos = appdata.position * mMVP;
	v2f.clipPos = appdata.position;
	v2f.uv = appdata.uv;
	return v2f;
}

Color UnlitTexShader::FragmentShader(VertexOut &v2f)
{
	Color albedo = RenderManager::Instance()->GetCurrentTexture()->Read(v2f.uv);
	return albedo;
}