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

	bool GetFloatUniform(int location, float &result);
	bool GetVector4Uniform(int location, Vector4 &result);
	bool GetColorUniform(int location, Color &result);

	bool SetFloatUniform(int location, float val);
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

	template<typename T>
	bool SetUniform(int location, std::map<int, T> &collections, const T &val);

	template<typename T>
	bool GetUniform(int location, std::map<int, T> &collections, T &result);

private:
	VertexShader *mVertexShader;
	FragmentShader *mFragmentShader;

	Vector4 mFragCoord;
	Matrix4x4 mAfterMVP;
	Matrix4x4 mUniformMatrix[MAX_SHADER_UNIFORM_MATRIX];
	Light mLight;
	Color mColor;

	bool mMighChangeZ = false; //着色器是否会修改Z值，会影响提前的Z测试

	std::map<int, float> mFloatUniforms;
	std::map<int, Vector4> mVector4Uniforms;
	std::map<int, Color> mColorUniforms;
};

//*****************************************************************************
template<typename T>
bool ShaderProgram::SetUniform(int location, std::map<int, T> &collections, const T &val)
{
	auto it = collections.find(location);
	if (it != collections.end())
	{
		it->second = val;
		return true;
	}

	return false;
}

//*****************************************************************************
template<typename T>
bool ShaderProgram::GetUniform(int location, std::map<int, T> &collections, T &result)
{
	auto it = collections.find(location);
	if (it != collections.end())
	{
		result = it->second;
		return true;
	}

	return false;
}