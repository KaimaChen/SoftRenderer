#include "ShaderProgram.h"

//*****************************************************************************
ShaderProgram::~ShaderProgram()
{
	if (mVertexShader)
	{
		delete mVertexShader;
		mVertexShader = nullptr;
	}
	if (mFragmentShader)
	{
		delete mFragmentShader;
		mFragmentShader = nullptr;
	}
}