#pragma once

#include <string>
#include "Math\Vector4.h"
#include "Math\Vector2.h"

struct Face
{
	Face() {}
	Face(int a, int b, int c, Vector4 n, Vector4 cp) : v0(a), v1(b), v2(c), normal(n), centerPosition(cp) { }
	int v0;
	int v1; 
	int v2;
	Vector4 normal;
	Vector4 centerPosition;
};

struct Vertex
{
	Vertex() : position(Vector4::zero), uv(Vector2::zero), normal(Vector4::zero) {}
	Vertex(Vector4 pos, Vector2 st, Vector4 n) : position(pos), uv(st), normal(n) {}
	Vector4 position;
	Vector2 uv;
	Vector4 normal;
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
	Vector4 GetPosition() { return mPosition; }
	void SetPosition(Vector4 pos) { mPosition = pos; }
	Vector4 GetRotation() { return mRotation; }
	void SetRotation(Vector4 rot) { mRotation = rot; }
private:
	std::string mName;
	Vertex *mVertices;
	int mVerticesCount;
	Face *mFaces;
	int mFacesCount;
	Vector4 mPosition;
	Vector4 mRotation;
};