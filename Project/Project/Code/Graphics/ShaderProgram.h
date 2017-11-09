#pragma once

#include "VertexShader.h"
#include "FragmentShader.h"
#include "DataStructure\Vertex.h"
#include "Math\Matrix4x4.h"
#include "Graphics\DataStructure\Light.h"

class ShaderProgram
{
public:
	~ShaderProgram();
	void Attach(VertexShader *shader) { mVertexShader = shader; shader->SetProgram(this); }
	void Attach(FragmentShader *shader) { mFragmentShader = shader; shader->SetProgram(this); }
	void Link() {}
	VertexOut ExecuteVertexShader(const VertexIn &appdata) { return mVertexShader->Execute(appdata); }
	Color ExecuteFragmentShader(const VertexOut &v2f) { return mFragmentShader->Execute(v2f); }



	void SetWorldMat(Matrix4x4 mat) { mWorldMat = mat; }
	void SetITWorldMat(Matrix4x4 mat) { mITWorldMat = mat; }
	void SetMVP(Matrix4x4 mvp) { mMVP = mvp; }
	void SetLight(Light light) { mLight = light; }
	void SetFragCoord(Vector4 coord) { mFragCoord = coord; }
	void SetColor(Color color) { mColor = color; }

	Matrix4x4 GetWorldMat() { return mWorldMat; }
	Matrix4x4 GetMVP() { return mMVP; }
	Matrix4x4 GetITWorldMat() { return mITWorldMat; }
	Light GetLight() { return mLight; }
	Vector4 GetFragCoord() { return mFragCoord; }
	Color GetColor() { return mColor; }

private:
	VertexShader *mVertexShader;
	FragmentShader *mFragmentShader;

	Vector4 mFragCoord;
	Matrix4x4 mWorldMat;
	Matrix4x4 mITWorldMat;
	Matrix4x4 mMVP;
	Light mLight;
	Color mColor;
};
