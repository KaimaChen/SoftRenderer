#pragma once

#include "DataStructure\Color.h"
#include "DataStructure\Vertex.h"

class ShaderProgram;

class VertexShader
{
public:
	virtual VertexOut Execute(const VertexIn &appdata) = 0;
	void SetProgram(ShaderProgram *program) { mProgram = program; }
protected:
	ShaderProgram *mProgram;
};