#pragma once

#include "Graphics\Shader.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"

class UnlitTexShader : public Shader
{
public:
	VertexOut VertexShader(const VertexIn &appdata);
	Color FragmentShader(VertexOut &v2f);
};