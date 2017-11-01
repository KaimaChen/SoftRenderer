#pragma once

#include <string>
#include "Math\Vector3.h"
#include "Math\Vector2.h"

struct Face
{
	Face() {}
	Face(int a, int b, int c, Vector3 n, Vector3 cp) : v0(a), v1(b), v2(c), normal(n), centerPosition(cp) { }
	int v0;
	int v1; 
	int v2;
	Vector3 normal;
	Vector3 centerPosition;
};

struct Vertex
{
	Vertex() : position(Vector3::zero), uv(Vector2::zero), normal(Vector3::zero) {}
	Vertex(Vector3 pos, Vector2 st, Vector3 n) : position(pos), uv(st), normal(n) {}
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
};

class Mesh
{
public:
	Mesh(std::string name, int verticesCount, int facesCount);
	~Mesh();
	Vertex *Vertices() { return mVertices; }
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
	Vertex *mVertices;
	int mVerticesCount;
	Face *mFaces;
	int mFacesCount;
	Vector3 mPosition;
	Vector3 mRotation;
};