#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include "Math\Vector4.h"
#include "Math\Vector2.h"

class OBJLoader
{
public:
	bool Load(const char *path);
private:
	std::vector<Vector4> mVertices;
	std::vector<Vector2> mUvs;
	std::vector<Vector4> mNormals;
};