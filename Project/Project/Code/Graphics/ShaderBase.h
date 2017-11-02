#pragma once

#include "DataStructure\Vertex.h"

class ShaderBase
{
public:
	ShaderBase() {}
	~ShaderBase() {}

	virtual VertexOut VertexShader(const VertexIn &appdata) = 0;
	virtual Color FragmentShader(VertexOut &v2f) = 0;
};