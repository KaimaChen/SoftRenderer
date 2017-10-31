#pragma once

#include "GX\Drawing.h"
#include "GX\Mesh.h"
#include "GX\Camera.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"

Mesh gMesh = Mesh("Cube", 8, 12);
Matrix4x4 V, P;

void Init(void(*DrawPixel)(int x, int y))
{
	Drawing::Instance()->DrawPixel = DrawPixel; //Important

	gMesh.Vertices()[0] = Vector3(-1, 1, 1);
	gMesh.Vertices()[1] = Vector3(1, 1, 1);
	gMesh.Vertices()[2] = Vector3(-1, -1, 1);
	gMesh.Vertices()[3] = Vector3(-1, -1, -1);
	gMesh.Vertices()[4] = Vector3(-1, 1, -1);
	gMesh.Vertices()[5] = Vector3(1, 1, -1);
	gMesh.Vertices()[6] = Vector3(1, -1, 1);
	gMesh.Vertices()[7] = Vector3(1, -1, -1);

	//up
	gMesh.Faces()[0] = Face(0, 1, 4);
	gMesh.Faces()[1] = Face(1, 4, 5);
	//down
	gMesh.Faces()[2] = Face(2, 3, 6);
	gMesh.Faces()[3] = Face(3, 6, 7);
	//front
	gMesh.Faces()[4] = Face(3, 4, 5);
	gMesh.Faces()[5] = Face(3, 5, 7);
	//back
	gMesh.Faces()[6] = Face(0, 1, 2);
	gMesh.Faces()[7] = Face(1, 2, 6);
	//left
	gMesh.Faces()[8] = Face(2, 3, 4);
	gMesh.Faces()[9] = Face(0, 2, 4);
	//right
	gMesh.Faces()[10] = Face(1, 5, 7);
	gMesh.Faces()[11] = Face(5, 6, 7);
	
	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	V = Matrix4x4::LookAtLH(Vector3(0,0,5), Vector3(0,0,0), Vector3(0, 1, 0));
	P = Matrix4x4::PerspectiveFovLH(0.78f, aspect, 0.01f, 1.0f);
}

float rx = 0;
float ry = 0;
void Update()
{
	Drawing::Instance()->Clear(0);
	
	rx += 0.01f;
	ry += 0.01f;
	Matrix4x4 M = Matrix4x4::RotateX(rx) * Matrix4x4::RotateY(ry);
	Matrix4x4 MVP = M * V * P;

	for (int i = 0; i < gMesh.GetFacesCount(); i++)
	{
		Face face = gMesh.Faces()[i];
		Vector3 v0 = gMesh.Vertices()[face.v0];
		Vector3 v1 = gMesh.Vertices()[face.v1];
		Vector3 v2 = gMesh.Vertices()[face.v2];
		Vector2 screenPos0 = Drawing::Instance()->Project(v0, MVP);
		Vector2 screenPos1 = Drawing::Instance()->Project(v1, MVP);
		Vector2 screenPos2 = Drawing::Instance()->Project(v2, MVP);

		Drawing::Instance()->DrawLine(screenPos0, screenPos1);
		Drawing::Instance()->DrawLine(screenPos1, screenPos2);
		Drawing::Instance()->DrawLine(screenPos0, screenPos2);
	}

	Drawing::Instance()->Render();
}