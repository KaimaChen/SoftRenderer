#pragma once

#include <string>
#include "Math\Vector3.h"

class Face
{
public:
	Face() {}
	Face(int a, int b, int c) : v0(a), v1(b), v2(c) { }
	int v0;
	int v1; 
	int v2;
};

class Mesh
{
public:
	Mesh(std::string name, int verticesCount, int facesCount);
	~Mesh();
	Vector3 *Vertices() { return mVertices; }
	Face *Faces() { return mFaces; }
	int GetVerticesCount() { return mVerticesCount; }
	void SetVerticesCount(int v) { mVerticesCount = v; }
	int GetFacesCount() { return mFacesCount; }
	void SetFacesCount(int v) { mFacesCount = v; }
	Vector3 GetPosition() { return mPosition; }
	void SetPosition(Vector3 pos) { mPosition = pos; }
	Vector3 GetRotation() { return mRotation; }
	void SetRotation(Vector3 rot) { mRotation = rot; }
private:
	std::string mName;
	Vector3 *mVertices;
	int mVerticesCount;
	Face *mFaces;
	int mFacesCount;
	Vector3 mPosition;
	Vector3 mRotation;
};