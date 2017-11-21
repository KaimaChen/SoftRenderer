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
			mVector4Uniforms.insert(std::make_pair(locations[i], Vector4::zero));
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
			mVector4Uniforms.insert(std::make_pair(locations[i], Vector4::zero));
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
	mVector4Uniforms.clear();
	mColorUniforms.clear();
}

//*****************************************************************************
bool ShaderProgram::GetFloatUniform(int location, float &result)
{
	return GetUniform<float>(location, mFloatUniforms, result);
}

//*****************************************************************************
bool ShaderProgram::GetVector4Uniform(int location, Vector4 &result)
{
	return GetUniform<Vector4>(location, mVector4Uniforms, result);
}

//*****************************************************************************
bool ShaderProgram::GetColorUniform(int location, Color &result)
{
	return GetUniform<Color>(location, mColorUniforms, result);
}

//*****************************************************************************
bool ShaderProgram::SetFloatUniform(int location, float val)
{
	return SetUniform<float>(location, mFloatUniforms, val);
}

//*****************************************************************************
bool ShaderProgram::SetVector4Uniform(int location, const Vector4 &val)
{
	return SetUniform<Vector4>(location, mVector4Uniforms, val);
}

//*****************************************************************************
bool ShaderProgram::SetColorUniform(int location, const Color &val)
{
	return SetUniform<Color>(location, mColorUniforms, val);
}