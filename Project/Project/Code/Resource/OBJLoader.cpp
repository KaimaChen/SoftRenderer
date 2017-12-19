#include "OBJLoader.h"

bool OBJLoader::Load(const char *path)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector4> tempVertices;
	std::vector<Vector2> tempUvs;
	std::vector<Vector4> tempNormals;

	FILE *stream = nullptr;
	if (fopen_s(&stream, path, "r") != 0)
		return false;

	while (true)
	{
		char lineHeader[128];

		int res = fscanf(stream, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0)
		{
			Vector4 vec;
			fscanf(stream, "%f %f %fn", &vec.x, &vec.y, &vec.z);
			tempVertices.push_back(vec);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			Vector2 vec;
			fscanf(stream, "%f %fn", &vec.x, &vec.y);
			tempUvs.push_back(vec);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			Vector4 vec;
			fscanf(stream, "%f %f %fn", &vec.x, &vec.y, &vec.z);
			tempNormals.push_back(vec);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(stream, "%d/%d/%d %d/%d/%d %d/%d/%dn", 
				&vertexIndex[0], &vertexIndex[1], &vertexIndex[2],
				&uvIndex[0], &uvIndex[1], &uvIndex[2],
				&normalIndex[0], &normalIndex[1], &normalIndex[2]);

			if (matches != 9)
			{
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

			for (int i = 0; i < 3; i++)
			{
				unsigned int vIndex = vertexIndices[i];
				Vector4 vertex = tempVertices[vIndex - 1]; //obj索引是从1开始的
				mVertices.push_back(vertex);

				unsigned int uIndex = uvIndices[i];
				Vector2 uv = tempUvs[uIndex - 1];
				mUvs.push_back(uv);

				unsigned int nIndex = normalIndices[i];
				Vector4 normal = tempNormals[nIndex - 1];
				mNormals.push_back(normal);
			}
		}
	}

	return true;
}