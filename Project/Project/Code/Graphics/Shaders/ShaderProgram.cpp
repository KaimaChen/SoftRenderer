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
	return GetUniform<float>(location, mUniform1f, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform2f(int location, fvec2 &result)
{
	return GetUniform<fvec2>(location, mUniform2f, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform3f(int location, fvec3 &result)
{
	return GetUniform<fvec3>(location, mUniform3f, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform4f(int location, fvec4 &result)
{
	return GetUniform<fvec4>(location, mUniform4f, result);
}

//*****************************************************************************
bool ShaderProgram::GetColorUniform(int location, Color &result)
{
	return GetUniform<Color>(location, mColorUniforms, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform1i(int location, int &result)
{
	return GetUniform<int>(location, mUniform1i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform2i(int location, ivec2 &result)
{
	return GetUniform<ivec2>(location, mUniform2i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform3i(int location, ivec3 &result)
{
	return GetUniform<ivec3>(location, mUniform3i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform4i(int location, ivec4 &result)
{
	return GetUniform<ivec4>(location, mUniform4i, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform1ui(int location, uint &result)
{
	return GetUniform<uint>(location, mUniform1ui, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform2ui(int location, uivec2 &result)
{
	return GetUniform<uivec2>(location, mUniform2ui, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform3ui(int location, uivec3 &result)
{
	return GetUniform<uivec3>(location, mUniform3ui, result);
}

//*****************************************************************************
bool ShaderProgram::GetUniform4ui(int location, uivec4 &result)
{
	return GetUniform<uivec4>(location, mUniform4ui, result);
}

//*****************************************************************************
bool ShaderProgram::SetUniform1f(int location, float val)
{
	return SetUniform<float>(location, mUniform1f, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform2f(int location, const fvec2 &val)
{
	return SetUniform<fvec2>(location, mUniform2f, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform3f(int location, const fvec3 &val)
{
	return SetUniform<fvec3>(location, mUniform3f, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform4f(int location, const fvec4 &val)
{
	return SetUniform<fvec4>(location, mUniform4f, val);
}

//*****************************************************************************
bool ShaderProgram::SetColorUniform(int location, const Color &val)
{
	return SetUniform<Color>(location, mColorUniforms, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform1i(int location, int val)
{
	return SetUniform<int>(location, mUniform1i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform2i(int location, const ivec2 &val)
{
	return SetUniform<ivec2>(location, mUniform2i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform3i(int location, const ivec3 &val)
{
	return SetUniform<ivec3>(location, mUniform3i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform4i(int location, const ivec4 &val)
{
	return SetUniform<ivec4>(location, mUniform4i, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform1ui(int location, uint val)
{
	return SetUniform<uint>(location, mUniform1ui, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform2ui(int location, const uivec2 &val)
{
	return SetUniform<uivec2>(location, mUniform2ui, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform3ui(int location, const uivec3 &val)
{
	return SetUniform<uivec3>(location, mUniform3ui, val);
}

//*****************************************************************************
bool ShaderProgram::SetUniform4ui(int location, const uivec4 &val)
{
	return SetUniform<uivec4>(location, mUniform4ui, val);
}