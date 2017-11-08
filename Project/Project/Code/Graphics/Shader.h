#pragma once

#include "DataStructure\Vertex.h"
#include "Math\Matrix4x4.h"
#include "Graphics\DataStructure\Light.h"

#define discard return Color::notValid
//#define gl_FragCoord return mFragCoord;

class Shader
{
public:
	Shader() {}
	~Shader() {}

	virtual VertexOut VertexShader(const VertexIn &appdata) = 0;
	virtual Color FragmentShader(VertexOut &v2f) = 0;

	void SetWorldMat(Matrix4x4 mat) { mWorldMat = mat; }
	void SetITWorldMat(Matrix4x4 mat) { mITWorldMat = mat; }
	void SetMVP(Matrix4x4 mvp) { mMVP = mvp; }
	void SetLight(Light light) { mLight = light; }

protected:
	Matrix4x4 mWorldMat;
	Matrix4x4 mITWorldMat;
	Matrix4x4 mMVP;
	Light mLight;
	//Vector4 mFragCoord;
};