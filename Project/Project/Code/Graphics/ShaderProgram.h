#pragma once

#include "VertexShader.h"
#include "FragmentShader.h"
#include "DataStructure\Vertex.h"
#include "Math\Matrix4x4.h"
#include "Graphics\DataStructure\Light.h"
#include "Misc\Settings.h"

class ShaderProgram
{
public:
	~ShaderProgram();
	void Attach(VertexShader *shader) { mVertexShader = shader; shader->SetProgram(this); }
	void Attach(FragmentShader *shader) { mFragmentShader = shader; shader->SetProgram(this); }
	void Link() {}
	VertexOut ExecuteVertexShader(const VertexIn &appdata) { return mVertexShader->Execute(appdata); }
	Color ExecuteFragmentShader(const VertexOut &v2f) { return mFragmentShader->Execute(v2f); }



	void SetLight(Light light) { mLight = light; }
	void SetFragCoord(Vector4 coord) { mFragCoord = coord; }
	void SetColor(Color color) { mColor = color; }

	void SetMatrix(const Matrix4x4 &mat, int id);

	Light GetLight() { return mLight; }
	Vector4 GetFragCoord() { return mFragCoord; }
	Color GetColor() { return mColor; }

	Matrix4x4 GetMatrix(int id) const;

private:
	VertexShader *mVertexShader;
	FragmentShader *mFragmentShader;

	Vector4 mFragCoord;
	Matrix4x4 mAfterMVP;
	Matrix4x4 mUniformMatrix[MAX_SHADER_UNIFORM_MATRIX];
	Light mLight;
	Color mColor;
};
