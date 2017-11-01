/*
待办难题：
1. 使用多线程提高性能
2. 编写着色器编译器
3. 整个渲染管线架构

Bug:
1. 当两个三角形共边时，渲染谁的？
*/

#pragma once

#include "Graphics\Drawing.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Mesh.h"
#include "Graphics\DataStructure\Light.h"
#include "Math\Matrix4x4.h"
#include "Misc\RenderState.h"

RenderState gState = RenderState::Shading;
Mesh gMesh = Mesh("Cube", 8, 12);
Light gLight = Light();
Matrix4x4 V, P;

void Init(void(*DrawPixel)(int x, int y, float r, float g, float b))
{
	Drawing::Instance()->NativeDrawPixel = DrawPixel; //Important
	LineDrawing::NativeDrawPixel = DrawPixel;

	gLight.position = Vector3(2, 1, 2);
	gLight.isDirectional = false;
	gLight.diretion = Vector3(1, 1, 1);
	gLight.color = Color::yellow;

	gMesh.Vertices()[0].position = Vector3(-1, 1, 1);
	gMesh.Vertices()[1].position = Vector3(1, 1, 1);
	gMesh.Vertices()[2].position = Vector3(-1, -1, 1);
	gMesh.Vertices()[3].position = Vector3(-1, -1, -1);
	gMesh.Vertices()[4].position = Vector3(-1, 1, -1);
	gMesh.Vertices()[5].position = Vector3(1, 1, -1);
	gMesh.Vertices()[6].position = Vector3(1, -1, 1);
	gMesh.Vertices()[7].position = Vector3(1, -1, -1);

	//up
	Vector3 normal = Vector3(0, 1, 0);
	Vector3 centerPosition = (gMesh.Vertices()[0].position + gMesh.Vertices()[1].position + gMesh.Vertices()[4].position) / 3;
	gMesh.Faces()[0] = Face(0, 1, 4, normal, normal);
	centerPosition = (gMesh.Vertices()[1].position + gMesh.Vertices()[4].position + gMesh.Vertices()[5].position) / 3;
	gMesh.Faces()[1] = Face(1, 4, 5, normal, normal);
	//down
	normal = Vector3(0, -1, 0);
	centerPosition = (gMesh.Vertices()[2].position + gMesh.Vertices()[3].position + gMesh.Vertices()[6].position) / 3;
	gMesh.Faces()[2] = Face(2, 3, 6, normal, normal);
	centerPosition = (gMesh.Vertices()[3].position + gMesh.Vertices()[6].position + gMesh.Vertices()[7].position) / 3;
	gMesh.Faces()[3] = Face(3, 6, 7, normal, normal);
	//front
	normal = Vector3(0, 0, -1);
	centerPosition = (gMesh.Vertices()[3].position + gMesh.Vertices()[4].position + gMesh.Vertices()[5].position) / 3;
	gMesh.Faces()[4] = Face(3, 4, 5, normal, normal);
	centerPosition = (gMesh.Vertices()[3].position + gMesh.Vertices()[5].position + gMesh.Vertices()[7].position) / 3;
	gMesh.Faces()[5] = Face(3, 5, 7, normal, normal);
	//back
	normal = Vector3(0, 0, 1);
	centerPosition = (gMesh.Vertices()[0].position + gMesh.Vertices()[1].position + gMesh.Vertices()[2].position) / 3;
	gMesh.Faces()[6] = Face(0, 1, 2, normal, normal);
	centerPosition = (gMesh.Vertices()[1].position + gMesh.Vertices()[2].position + gMesh.Vertices()[6].position) / 3;
	gMesh.Faces()[7] = Face(1, 2, 6, normal, normal);
	//left
	normal = Vector3(-1, 0, 0);
	centerPosition = (gMesh.Vertices()[2].position + gMesh.Vertices()[3].position + gMesh.Vertices()[4].position) / 3;
	gMesh.Faces()[8] = Face(2, 3, 4, normal, normal);
	centerPosition = (gMesh.Vertices()[0].position + gMesh.Vertices()[2].position + gMesh.Vertices()[4].position) / 3;
	gMesh.Faces()[9] = Face(0, 2, 4, normal, normal);
	//right
	normal = Vector3(1, 0, 0);
	centerPosition = (gMesh.Vertices()[1].position + gMesh.Vertices()[5].position + gMesh.Vertices()[7].position) / 3;
	gMesh.Faces()[10] = Face(1, 5, 7, normal, normal);
	centerPosition = (gMesh.Vertices()[1].position + gMesh.Vertices()[6].position + gMesh.Vertices()[7].position) / 3;
	gMesh.Faces()[11] = Face(1, 6, 7, normal, normal);

	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	V = Matrix4x4::LookAtLH(Vector3(5,0,0), Vector3(0,0,0), Vector3(0, 1, 0));
	P = Matrix4x4::PerspectiveFovLH(0.78f, aspect, 0.01f, 1.0f);
}

float rx = 0;
float ry = 0;
float rxLight = 0;
float ryLight = 0;
bool printed = false;
void Update()
{
	Drawing::Instance()->Clear(0);
	
	
	Matrix4x4 M = Matrix4x4::RotateX(rx) * Matrix4x4::RotateY(ry);
	Matrix4x4 MVP = M * V * P;

	rx += 0.01f;
	ry += 0.01f;
	int count = 12;
	for (int i = 0; i < count; i++)
	{
		Face face = gMesh.Faces()[i];
		Vector3 v0 = gMesh.Vertices()[face.v0].position;
		Vector3 v1 = gMesh.Vertices()[face.v1].position;
		Vector3 v2 = gMesh.Vertices()[face.v2].position;
		Vector3 screenPos0 = Drawing::Instance()->Project(v0, MVP);
		Vector3 screenPos1 = Drawing::Instance()->Project(v1, MVP);
		Vector3 screenPos2 = Drawing::Instance()->Project(v2, MVP);

		if (gState == RenderState::Shading)
		{
			Vector3 worldPos;
			M.PointMulMat(face.centerPosition, worldPos);
			Vector3 lightWorldPos = gLight.position;
			Vector3 worldNormal;
			M.DirMulMat(face.normal, worldNormal);
			worldNormal.Normalize();
			Vector3 toLightDir = (lightWorldPos - worldPos).Normalize();
			Color color = gLight.color * Math::Clamp01(Vector3::Dot(toLightDir, worldNormal)) + Color::yellow * 0.1f;

			Drawing::Instance()->DrawTriangle(screenPos0, screenPos1, screenPos2, color);
		}
		else if (gState == RenderState::WireFrame)
		{
			LineDrawing::BresenhamDrawLine(screenPos0.x, screenPos0.y, screenPos1.x, screenPos1.y);
			LineDrawing::BresenhamDrawLine(screenPos0.x, screenPos0.y, screenPos2.x, screenPos2.y);
			LineDrawing::BresenhamDrawLine(screenPos2.x, screenPos2.y, screenPos1.x, screenPos1.y);
		}
	}

	//Drawing::Instance()->Render();
}