#include "Mesh.h"

Mesh::Mesh(std::string name, int verticesCount, int facesCount)
{
	mName = name;
	mVerticesCount = verticesCount;
	mVertices = new Vector3[verticesCount];
	mFacesCount = facesCount;
	mFaces = new Face[facesCount];
}

Mesh::~Mesh()
{
	delete[] mVertices;
	mVertices = nullptr;
}