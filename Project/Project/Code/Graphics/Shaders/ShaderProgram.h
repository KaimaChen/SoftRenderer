#pragma once

#include <map>
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Graphics\Shaders\Vertex.h"
#include "Math\Matrix4x4.h"
#include "Math\TemplateVector.h"
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

	bool GetUniform1f(int location, float &result);
	bool GetUniform2f(int location, fvec2 &result);
	bool GetUniform3f(int location, fvec3 &result);
	bool GetUniform4f(int location, fvec4 &result);
	bool GetColorUniform(int location, Color &result);

	bool GetUniform1i(int location, int &result);
	bool GetUniform2i(int location, ivec2 &result);
	bool GetUniform3i(int location, ivec3 &result);
	bool GetUniform4i(int location, ivec4 &result);

	bool GetUniform1ui(int location, uint &result);
	bool GetUniform2ui(int location, uivec2 &result);
	bool GetUniform3ui(int location, uivec3 &result);
	bool GetUniform4ui(int location, uivec4 &result);

	bool SetUniform1f(int location, float val);
	bool SetUniform2f(int location, const fvec2 &result);
	bool SetUniform3f(int location, const fvec3 &result);
	bool SetUniform4f(int location, const fvec4 &val);
	bool SetColorUniform(int location, const Color &val);

	bool SetUniform1i(int location, int val);
	bool SetUniform2i(int location, const ivec2 &result);
	bool SetUniform3i(int location, const ivec3 &result);
	bool SetUniform4i(int location, const ivec4 &result);

	bool SetUniform1ui(int location, uint val);
	bool SetUniform2ui(int location, const uivec2 &result);
	bool SetUniform3ui(int location, const uivec3 &result);
	bool SetUniform4ui(int location, const uivec4 &result);
	
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

	std::map<int, float> mUniform1f;
	std::map<int, fvec2> mUniform2f;
	std::map<int, fvec3> mUniform3f;
	std::map<int, fvec4> mUniform4f;
	std::map<int, Color> mColorUniforms;
	std::map<int, int> mUniform1i;
	std::map<int, ivec2> mUniform2i;
	std::map<int, ivec3> mUniform3i;
	std::map<int, ivec4> mUniform4i;
	std::map<int, uint> mUniform1ui;
	std::map<int, uivec2> mUniform2ui;
	std::map<int, uivec3> mUniform3ui;
	std::map<int, uivec4> mUniform4ui;
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