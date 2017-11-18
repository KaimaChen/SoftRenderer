#include "ShaderProgram.h"

//*****************************************************************************
ShaderProgram::~ShaderProgram()
{
	SAFE_DELETE(mVertexShader);
	SAFE_DELETE(mFragmentShader);
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