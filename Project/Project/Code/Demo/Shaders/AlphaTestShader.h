#pragma once

#include "Graphics\Shader.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\RenderManager.h"

class AlphaTestShader : public Shader
{
public:
	VertexOut VertexShader(const VertexIn &appdata)
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mMVP;
		//v2f.clipPos = appdata.position;
		v2f.uv = appdata.uv;
		return v2f;
	}

	Color FragmentShader(VertexOut &v2f)
	{
		Color albedo = RenderManager::Instance()->GetTexture0()->Read(v2f.uv);
		Color noise = RenderManager::Instance()->GetTexture1()->Read(v2f.uv);
		if (noise.r < 0.5f)
			discard;

		return albedo;
	}
};