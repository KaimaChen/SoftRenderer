#include "Mesh.h"

Mesh::Mesh(std::string name, int verticesCount, int facesCount)
{
	mName = name;
	mVerticesCount = verticesCount;
	mVertices = new Vertex[verticesCount];
	mFacesCount = facesCount;
	mFaces = new Face[facesCount];
}

Mesh::~Mesh()
{
	delete[] mVertices;
	mVertices = nullptr;
}