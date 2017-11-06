#pragma once

#include "Graphics\Shader.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\RenderManager.h"

class AlphaBlendShader : public Shader
{
public:
	VertexOut VertexShader(const VertexIn &appdata)
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mMVP;
		return v2f;
	}

	Color FragmentShader(VertexOut &v2f)
	{
		return Color(1, 0, 0, 0.5);
	}
};