#pragma once

#include <map>
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Graphics\Shaders\Vertex.h"
#include "Math\Matrix4x4.h"
#include "Graphics\DataStructure\Light.h"
#include "Misc\Settings.h"
#include "Misc\Defines.h"
#include "Misc\GLSLDefines.h"

class ShaderProgram
{
public:
	~ShaderProgram();
	void Attach(VertexShader *shader);
	void Attach(FragmentShader *shader);
	void Link();
	void InitShaderUniforms();
	VertexOut ExecuteVertexShader(const VertexIn &appdata) { return mVertexShader->Execute(appdata); }
	Color ExecuteFragmentShader(const VertexOut &v2f) { return mFragmentShader->Execute(v2f); }

	bool GetVector4Uniform(int location, Vector4 &result) const;
	bool GetColorUniform(int location, Color &result) const;

	bool SetVector4Uniform(int location, const Vector4 &val);
	bool SetColorUniform(int location, const Color &val);
	
	//*****************************************************************************
	void SetLight(Light light) { mLight = light; }
	void SetFragCoord(Vector4 coord) { mFragCoord = coord; }
	void SetColor(Color color) { mColor = color; }
	void SetMatrix(const Matrix4x4 &mat, int id);
	void SetMighChangeZ(bool migh) { mMighChangeZ = migh; }

	Light GetLight() { return mLight; }
	Vector4 GetFragCoord() { return mFragCoord; }
	Color GetColor() { return mColor; }
	Matrix4x4 GetMatrix(int id) const;
	bool GetMighChangeZ() { return mMighChangeZ; }

private:
	void Clear();

private:
	VertexShader *mVertexShader;
	FragmentShader *mFragmentShader;

	Vector4 mFragCoord;
	Matrix4x4 mAfterMVP;
	Matrix4x4 mUniformMatrix[MAX_SHADER_UNIFORM_MATRIX];
	Light mLight;
	Color mColor;

	bool mMighChangeZ = false; //着色器是否会修改Z值，会影响提前的Z测试

	std::map<int, Vector4> mVector4Uniforms;
	std::map<int, Color> mColorUniforms;
};
