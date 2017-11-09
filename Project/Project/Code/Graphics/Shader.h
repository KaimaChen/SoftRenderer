#pragma once

#include "DataStructure\Vertex.h"
#include "Math\Matrix4x4.h"
#include "Graphics\DataStructure\Light.h"

#define discard return Color::notValid
#define gl_FragCoord mFragCoord //TODO: Find a way to make it not accessable in vertex shader

class Shader
{
public:
	Shader() {}
	~Shader() {}

	virtual VertexOut VertexShader(const VertexIn &appdata) = 0;
	virtual Color FragmentShader(const VertexOut &v2f) = 0;

	void SetWorldMat(Matrix4x4 mat) { mWorldMat = mat; }
	void SetITWorldMat(Matrix4x4 mat) { mITWorldMat = mat; }
	void SetMVP(Matrix4x4 mvp) { mMVP = mvp; }
	void SetLight(Light light) { mLight = light; }
	void SetFragCoord(Vector4 coord) { mFragCoord = coord; }

protected:
	Vector4 mFragCoord;
	Matrix4x4 mWorldMat;
	Matrix4x4 mITWorldMat;
	Matrix4x4 mMVP;
	Light mLight;
};