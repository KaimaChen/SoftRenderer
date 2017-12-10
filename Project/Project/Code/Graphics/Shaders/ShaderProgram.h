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
	void InitShaderAttribs();
	void PreExecute() { mVertexShader->PreExecute(); mFragmentShader->PreExecute(); }
	VertexOut ExecuteVertexShader(const VertexIn &appdata) { return mVertexShader->Execute(appdata); }
	Color ExecuteFragmentShader(const VertexOut &v2f) { return mFragmentShader->Execute(v2f); }

	//*****************************************************************************
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

	bool GetAttrib1f(GLuint index, float &result);
	bool GetAttrib2f(GLuint index, fvec2 &result);
	bool GetAttrib3f(GLuint index, fvec3 &result);
	bool GetAttrib4f(GLuint index, fvec4 &result);

	bool GetAttrib4i(GLuint index, ivec4 &result);
	bool GetAttrib4ui(GLuint index, uivec4 &result);

	//*****************************************************************************
	bool SetUniform1f(int location, float val);
	bool SetUniform2f(int location, const fvec2 &val);
	bool SetUniform3f(int location, const fvec3 &val);
	bool SetUniform4f(int location, const fvec4 &val);
	bool SetColorUniform(int location, const Color &val);

	bool SetUniform1i(int location, int val);
	bool SetUniform2i(int location, const ivec2 &val);
	bool SetUniform3i(int location, const ivec3 &val);
	bool SetUniform4i(int location, const ivec4 &val);

	bool SetUniform1ui(int location, uint val);
	bool SetUniform2ui(int location, const uivec2 &val);
	bool SetUniform3ui(int location, const uivec3 &val);
	bool SetUniform4ui(int location, const uivec4 &val);

	bool SetAttrib1f(GLuint index, float val);
	bool SetAttrib2f(GLuint index, const fvec2 &val);
	bool SetAttrib3f(GLuint index, const fvec3 &val);
	bool SetAttrib4f(GLuint index, const fvec4 &val);

	bool SetAttrib4i(GLuint index, const ivec4 &val);

	bool SetAttrib4ui(GLuint index, const uivec4 &val);
	
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

	template<typename T, typename T2>
	bool Set(T2 location, std::map<T2, T> &collections, const T &val);

	template<typename T, typename T2>
	bool Get(T2 location, std::map<T2, T> &collections, T &result);

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

	std::map<GLuint, float> mAttrib1f;
	std::map<GLuint, fvec2> mAttrib2f;
	std::map<GLuint, fvec3> mAttrib3f;
	std::map<GLuint, fvec4> mAttrib4f;

	std::map<GLuint, ivec4> mAttrib4i;
	std::map<GLuint, uivec4> mAttrib4ui;
};

//*****************************************************************************
template<typename T, typename T2>
bool ShaderProgram::Set(T2 location, std::map<T2, T> &collections, const T &val)
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
template<typename T, typename T2>
bool ShaderProgram::Get(T2 location, std::map<T2, T> &collections, T &result)
{
	auto it = collections.find(location);
	if (it != collections.end())
	{
		result = it->second;
		return true;
	}

	return false;
}