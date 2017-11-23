#pragma once

#include <vector>
#include "Graphics\DataStructure\Color.h"
#include "Graphics\Shaders\Vertex.h"
#include "Graphics\Shaders\BaseShader.h"
#include "Misc\Defines.h"

class ShaderProgram;

class VertexShader : public BaseShader
{
public:
	virtual void RegisterAttribs() {}
	virtual void GetAttribs() {}
	virtual VertexOut Execute(const VertexIn &appdata) = 0;
	void SetProgram(ShaderProgram *program) { mProgram = program; }

	std::vector<GLuint> GetVector4Indices() { return mAttrib4fIndices; }

protected:
	ShaderProgram *mProgram;

	std::vector<GLuint> mAttrib4fIndices;
};