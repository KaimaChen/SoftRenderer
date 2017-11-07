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
	vertices.resize(24);
	//up
	vertices[0].position = Vector4(-1, 1, 1);
	vertices[0].uv = Vector2(0, 0);
	vertices[0].normal = Vector4(0, 1, 0, 0);
	
	vertices[1].position = Vector4(1, 1, 1);
	vertices[1].uv = Vector2(1, 0);
	vertices[1].normal = Vector4(0, 1, 0, 0);

	vertices[2].position = Vector4(1, 1, -1);
	vertices[2].uv = Vector2(1, 1);
	vertices[2].normal = Vector4(0, 1, 0, 0);

	vertices[3].position = Vector4(-1, 1, -1);
	vertices[3].uv = Vector2(0, 1);
	vertices[3].normal = Vector4(0, 1, 0, 0);
	
	//down
	vertices[4].position = Vector4(-1, -1, -1);
	vertices[4].uv = Vector2(0, 0);
	vertices[4].normal = Vector4(0, -1, 0, 0);

	vertices[5].position = Vector4(1, -1, -1);
	vertices[5].uv = Vector2(1, 0);
	vertices[5].normal = Vector4(0, -1, 0, 0);

	vertices[6].position = Vector4(1, -1, 1);
	vertices[6].uv = Vector2(1, 1);
	vertices[6].normal = Vector4(0, -1, 0, 0);

	vertices[7].position = Vector4(-1, -1, 1);
	vertices[7].uv = Vector2(0, 1);
	vertices[7].normal = Vector4(0, -1, 0, 0);

	//front
	vertices[8].position = Vector4(-1, -1, 1);
	vertices[8].uv = Vector2(0, 0);
	vertices[8].normal = Vector4(0, 0, 1, 0);

	vertices[9].position = Vector4(1, -1, 1);
	vertices[9].uv = Vector2(1, 0);
	vertices[9].normal = Vector4(0, 0, 1, 0);

	vertices[10].position = Vector4(1, 1, 1);
	vertices[10].uv = Vector2(1, 1);
	vertices[10].normal = Vector4(0, 0, 1, 0);

	vertices[11].position = Vector4(-1, 1, 1);
	vertices[11].uv = Vector2(0, 1);
	vertices[11].normal = Vector4(0, 0, 1, 0);

	//back
	vertices[12].position = Vector4(1, -1, -1);
	vertices[12].uv = Vector2(0, 0);
	vertices[12].normal = Vector4(0, 0, -1, 0);

	vertices[13].position = Vector4(-1, -1, -1);
	vertices[13].uv = Vector2(1, 0);
	vertices[13].normal = Vector4(0, 0, -1, 0);

	vertices[14].position = Vector4(-1, 1, -1);
	vertices[14].uv = Vector2(1, 1);
	vertices[14].normal = Vector4(0, 0, -1, 0);

	vertices[15].position = Vector4(1, 1, -1);
	vertices[15].uv = Vector2(0, 1);
	vertices[15].normal = Vector4(0, 0, -1, 0);

	//left
	vertices[16].position = Vector4(-1, -1, -1);
	vertices[16].uv = Vector2(0, 0);
	vertices[16].normal = Vector4(-1, 0, 0, 0);

	vertices[17].position = Vector4(-1, -1, 1);
	vertices[17].uv = Vector2(1, 0);
	vertices[17].normal = Vector4(-1, 0, 0, 0);

	vertices[18].position = Vector4(-1, 1, 1);
	vertices[18].uv = Vector2(1, 1);
	vertices[18].normal = Vector4(-1, 0, 0, 0);

	vertices[19].position = Vector4(-1, 1, -1);
	vertices[19].uv = Vector2(0, 1);
	vertices[19].normal = Vector4(-1, 0, 0, 0);

	//right
	vertices[20].position = Vector4(1, -1, 1);
	vertices[20].uv = Vector2(0, 0);
	vertices[20].normal = Vector4(1, 0, 0, 0);

	vertices[21].position = Vector4(1, -1, -1);
	vertices[21].uv = Vector2(1, 0);
	vertices[21].normal = Vector4(1, 0, 0, 0);

	vertices[22].position = Vector4(1, 1, -1);
	vertices[22].uv = Vector2(1, 1);
	vertices[22].normal = Vector4(1, 0, 0, 0);

	vertices[23].position = Vector4(1, 1, 1);
	vertices[23].uv = Vector2(0, 1);
	vertices[23].normal = Vector4(1, 0, 0, 0);

	indices.resize(36);
	//up
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	//down
	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;
	indices[9] = 4;
	indices[10] = 6;
	indices[11] = 7;
	//front
	indices[12] = 8;
	indices[13] = 9;
	indices[14] = 10;
	indices[15] = 8;
	indices[16] = 10;
	indices[17] = 11;
	//back
	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;
	indices[21] = 12;
	indices[22] = 14;
	indices[23] = 15;
	//left
	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;
	indices[27] = 16;
	indices[28] = 18;
	indices[29] = 19;
	//right
	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;
	indices[33] = 20;
	indices[34] = 22;
	indices[35] = 23;
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

	vertices[0].position = Vector4(-1, -1, 0);
	vertices[0].color = Color::red;
	vertices[0].uv = Vector2(0, 0);

	vertices[1].position = Vector4(1, -1, 0);
	vertices[1].color = Color::green;
	vertices[1].uv = Vector2(1, 0);

	vertices[2].position = Vector4(0, 1, 0);
	vertices[2].color = Color::blue;
	vertices[2].uv = Vector2(0.5f, 1);

	indices.resize(3);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
}

struct QuadData
{
	void Init();
	std::vector<VertexIn> vertices;
	std::vector<int> indices;
};

void QuadData::Init()
{
	vertices.resize(4);

	vertices[0].position = Vector4(-1, -1, 0);
	vertices[0].color = Color::red;
	vertices[0].uv = Vector2(0, 0);

	vertices[1].position = Vector4(1, -1, 0);
	vertices[1].color = Color::green;
	vertices[1].uv = Vector2(1, 0);

	vertices[2].position = Vector4(1, 1, 0);
	vertices[2].color = Color::blue;
	vertices[2].uv = Vector2(1, 1);

	vertices[3].position = Vector4(-1, 1, 0);
	vertices[3].color = Color::blue;
	vertices[3].uv = Vector2(0, 1);

	indices.resize(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
}