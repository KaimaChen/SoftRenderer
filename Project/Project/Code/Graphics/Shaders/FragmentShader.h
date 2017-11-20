#pragma once

#include "Graphics\DataStructure\Color.h"
#include "Graphics\Shaders\Vertex.h"
#include "Graphics\Shaders\BaseShader.h"

#define discard return Color::notValid
#define gl_FragCoord mProgram->GetFragCoord()

class ShaderProgram;

class FragmentShader : public BaseShader
{
public:
	virtual Color Execute(const VertexOut &v2f) = 0;
	void SetProgram(ShaderProgram *program) { mProgram = program; }

protected:
	ShaderProgram *mProgram;
};