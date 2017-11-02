#pragma once

#include "Graphics\ShaderBase.h"
#include "Math\Matrix4x4.h"

class PureColorShader : public ShaderBase
{
public:
	void SetMVP(Matrix4x4 mvp) { mMVP = mvp; }

	VertexOut VertexShader(const VertexIn &appdata);
	Color FragmentShader(VertexOut &v2f);

private:
	Matrix4x4 mMVP;
};