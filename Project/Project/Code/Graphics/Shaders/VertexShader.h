#pragma once

#include <vector>
#include "Graphics\DataStructure\Color.h"
#include "Graphics\Shaders\Vertex.h"
#include "Graphics\Shaders\BaseShader.h"

class ShaderProgram;

class VertexShader : public BaseShader
{
public:
	virtual VertexOut Execute(const VertexIn &appdata) = 0;
	void SetProgram(ShaderProgram *program) { mProgram = program; }

protected:
	ShaderProgram *mProgram;
};