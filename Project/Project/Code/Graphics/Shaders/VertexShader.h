#pragma once

#include "Graphics\DataStructure\Color.h"
#include "Graphics\Shaders\Vertex.h"

class ShaderProgram;

class VertexShader
{
public:
	virtual VertexOut Execute(const VertexIn &appdata) = 0;
	void SetProgram(ShaderProgram *program) { mProgram = program; }
protected:
	ShaderProgram *mProgram;
};