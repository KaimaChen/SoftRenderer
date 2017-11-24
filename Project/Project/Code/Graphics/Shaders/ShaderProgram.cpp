#include "ShaderProgram.h"

//*****************************************************************************
ShaderProgram::~ShaderProgram()
{
	SAFE_DELETE(mVertexShader);
	SAFE_DELETE(mFragmentShader);
}

//*****************************************************************************
void ShaderProgram::Attach(VertexShader *shader)
{
	shader->RegisterUniforms();
	shader->RegisterAttribs();
	mVertexShader = shader; 
	shader->SetProgram(this);
}

//*****************************************************************************
void ShaderProgram::Attach(FragmentShader *shader)
{
	shader->RegisterUniforms();
	mFragmentShader = shader;
	shader->SetProgram(this);
}

//*****************************************************************************
void ShaderProgram::Link()
{
	if (mVertexShader != nullptr)
	{
		std::vector<int> locations = mVertexShader->GetVector4Locations();
		for (int i = 0; i < locations.size(); ++i)
		{
			mUniform4f.insert(std::make_pair(locations[i], fvec4()));
		}

		locations = mVertexShader->GetColorLocation();
		for (int i = 0; i < locations.size(); ++i)
		{
			mColorUniforms.insert(std::make_pair(locations[i], Color::white));
		}

		std::vector<GLuint> indices = mVertexShader->GetVector4Indices();
		for (int i = 0; i < indices.size(); ++i)
		{
			mAttrib4f.insert(std::make_pair(indices[i], fvec4()));
		}
	}

	if (mFragmentShader != nullptr)
	{
		std::vector<int> locations = mFragmentShader->GetVector4Locations();
		for (int i = 0; i < locations.size(); ++i)
		{
			mUniform4f.insert(std::make_pair(locations[i], fvec4()));
		}

		locations = mFragmentShader->GetColorLocation();
		for (int i = 0; i < locations.size(); ++i)
		{
			mColorUniforms.insert(std::make_pair(locations[i], Color::white));
		}
	}
}

//*****************************************************************************
void ShaderProgram::InitShaderUniforms()
{
	if (mVertexShader != nullptr)
		mVertexShader->GetUniforms();
	if (mFragmentShader != nullptr)
		mFragmentShader->GetUniforms();
}

//*****************************************************************************
void ShaderProgram::InitShaderAttribs()
{
	if (mVertexShader != nullptr)
		mVertexShader->GetAttribs();
}

//*****************************************************************************
void ShaderProgram::SetMatrix(const Matrix4x4 &mat, int id)
{
	if (id > MAX_SHADER_UNIFORM_MATRIX)
		return;

	mUniformMatrix[id] = mat;
}

//*****************************************************************************
Matrix4x4 ShaderProgram::GetMatrix(int id) const
{
	if (id > MAX_SHADER_UNIFORM_MATRIX)
		return Matrix4x4::zero;

	return mUniformMatrix[id];
}

//*****************************************************************************
void ShaderProgram::Clear()
{
	mUniform1f.clear();
	mUniform2f.clear();
	mUniform3f.clear();
	mUniform4f.clear();
	mColorUniforms.clear();
}

//*****************************************************************************
bool ShaderProgram::GetUniform1f(int location, float &result)
{
	return Get<float, int>(location, mUniform1f, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform2f(int location, fvec2 &result)
{
	return Get<fvec2, int>(location, mUniform2f, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform3f(int location, fvec3 &result)
{
	return Get<fvec3, int>(location, mUniform3f, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform4f(int location, fvec4 &result)
{
	return Get<fvec4, int>(location, mUniform4f, result);
}

//*****************************************************************************
bool ShaderProgram::GetColorUniform(int location, Color &result)
{
	return Get<Color, int>(location, mColorUniforms, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform1i(int location, int &result)
{
	return Get<int, int>(location, mUniform1i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform2i(int location, ivec2 &result)
{
	return Get<ivec2, int>(location, mUniform2i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform3i(int location, ivec3 &result)
{
	return Get<ivec3, int>(location, mUniform3i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform4i(int location, ivec4 &result)
{
	return Get<ivec4, int>(location, mUniform4i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform1ui(int location, uint &result)
{
	return Get<uint, int>(location, mUniform1ui, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform2ui(int location, uivec2 &result)
{
	return Get<uivec2, int>(location, mUniform2ui, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform3ui(int location, uivec3 &result)
{
	return Get<uivec3, int>(location, mUniform3ui, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform4ui(int location, uivec4 &result)
{
	return Get<uivec4, int>(location, mUniform4ui, result);
}

//*****************************************************************************
bool ShaderProgram::GetAttrib1f(GLuint index, float &result)
{
	return Get<float, GLuint>(index, mAttrib1f, result);
}

//*****************************************************************************
bool ShaderProgram::GetAttrib2f(GLuint index, fvec2 &result)
{
	return Get<fvec2, GLuint>(index, mAttrib2f, result);
}

//*****************************************************************************
bool ShaderProgram::GetAttrib3f(GLuint index, fvec3 &result)
{
	return Get<fvec3, GLuint>(index, mAttrib3f, result);
}

//*****************************************************************************
bool ShaderProgram::GetAttrib4f(GLuint index, fvec4 &result)
{
	return Get<fvec4, GLuint>(index, mAttrib4f, result);
}

//*****************************************************************************
bool ShaderProgram::SetUniform1f(int location, float val)
{
	return Set<float, int>(location, mUniform1f, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform2f(int location, const fvec2 &val)
{
	return Set<fvec2, int>(location, mUniform2f, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform3f(int location, const fvec3 &val)
{
	return Set<fvec3, int>(location, mUniform3f, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform4f(int location, const fvec4 &val)
{
	return Set<fvec4, int>(location, mUniform4f, val);
}

//*****************************************************************************
bool ShaderProgram::SetColorUniform(int location, const Color &val)
{
	return Set<Color, int>(location, mColorUniforms, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform1i(int location, int val)
{
	return Set<int, int>(location, mUniform1i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform2i(int location, const ivec2 &val)
{
	return Set<ivec2, int>(location, mUniform2i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform3i(int location, const ivec3 &val)
{
	return Set<ivec3, int>(location, mUniform3i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform4i(int location, const ivec4 &val)
{
	return Set<ivec4, int>(location, mUniform4i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform1ui(int location, uint val)
{
	return Set<uint, int>(location, mUniform1ui, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform2ui(int location, const uivec2 &val)
{
	return Set<uivec2, int>(location, mUniform2ui, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform3ui(int location, const uivec3 &val)
{
	return Set<uivec3, int>(location, mUniform3ui, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform4ui(int location, const uivec4 &val)
{
	return Set<uivec4, int>(location, mUniform4ui, val);
}

//*****************************************************************************
bool ShaderProgram::SetAttrib1f(GLuint index, float val)
{
	return Set<float, GLuint>(index, mAttrib1f, val);
}

//*****************************************************************************
bool ShaderProgram::SetAttrib2f(GLuint index, const fvec2 &val)
{
	return Set<fvec2, GLuint>(index, mAttrib2f, val);
}

//*****************************************************************************
bool ShaderProgram::SetAttrib3f(GLuint index, const fvec3 &val)
{
	return Set<fvec3, GLuint>(index, mAttrib3f, val);
}

//*****************************************************************************
bool ShaderProgram::SetAttrib4f(GLuint index, const fvec4 &val)
{
	return Set<fvec4, GLuint>(index, mAttrib4f, val);
}