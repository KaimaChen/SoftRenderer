#pragma once

#include "Graphics\DataStructure\Vertex.h"
#include <vector>

struct BoxData
{
	void Init();
	std::vector<VertexIn> vertices;
	std::vector<int> indices;
};

void BoxData::Init()
{
	vertices.resize(8);
	vertices[0].position = Vector4(-1, 1, 1);
	vertices[1].position = Vector4(1, 1, 1);
	vertices[2].position = Vector4(-1, -1, 1);
	vertices[3].position = Vector4(-1, -1, -1);
	vertices[4].position = Vector4(-1, 1, -1);
	vertices[5].position = Vector4(1, 1, -1);
	vertices[6].position = Vector4(1, -1, 1);
	vertices[7].position = Vector4(1, -1, -1);

	indices.resize(36);
	//up
	indices[0] = 4;
	indices[1] = 0;
	indices[2] = 1;
	indices[3] = 4;
	indices[4] = 1;
	indices[5] = 5;
	//down
	indices[6] = 6;
	indices[7] = 2;
	indices[8] = 3;
	indices[9] = 6;
	indices[10] = 3;
	indices[11] = 7;
	//front
	indices[12] = 1;
	indices[13] = 0;
	indices[14] = 2;
	indices[15] = 1;
	indices[16] = 2;
	indices[17] = 6;
	//back
	indices[18] = 4;
	indices[19] = 5;
	indices[20] = 7;
	indices[21] = 4;
	indices[22] = 7;
	indices[23] = 3;
	//left
	indices[24] = 0;
	indices[25] = 4;
	indices[26] = 3;
	indices[27] = 0;
	indices[28] = 3;
	indices[29] = 2;
	//right
	indices[30] = 5;
	indices[31] = 1;
	indices[32] = 6;
	indices[33] = 5;
	indices[34] = 6;
	indices[35] = 7;
}

struct TriangleData
{
	void Init();
	std::vector<VertexIn> vertices;
	std::vector<int> indices;
};

void TriangleData::Init()
{
	vertices.resize(3);
	vertices[0].position = Vector4(0, -1, 0);
	vertices[1].position = Vector4(1, -1, 0);
	vertices[2].position = Vector4(0, 1, 0);

	indices.resize(3);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
}