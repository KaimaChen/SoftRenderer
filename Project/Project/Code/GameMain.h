/*
描述：
右手系

待办难题：
1. 使用多线程提高性能
2. 编写着色器编译器
3. 整个渲染管线架构

Bug:
1. 当两个三角形共边时，渲染谁的？
2. 新建一个文件后运行就会报错，必须clean一下才行
*/

#pragma once

#include "Graphics\Drawing.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Mesh.h"
#include "Graphics\DataStructure\Light.h"
#include "Graphics\DataStructure\Camera.h"
#include "Math\Matrix4x4.h"
#include "Misc\RenderState.h"
#include "Managers\RenderManager.h"
#include "Demo\DemoData.h"
#include "Demo\Shaders\PureColorShader.h"

Light gLight = Light();

void Init(void(*DrawPixel)(int x, int y, float r, float g, float b))
{
	Drawing::Instance()->NativeDrawPixel = DrawPixel; //Important
	LineDrawing::NativeDrawPixel = DrawPixel;

	Camera camera;
	camera.eye = Vector4(0, 0, 5);
	camera.at = Vector4::zero;
	camera.up = Vector4(0, 1, 0);
	camera.fov = 0.78f;
	camera.aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	camera.zn = 0.01f;
	camera.zf = 1.0f;
	RenderManager::Instance()->SetMainCamera(camera);

	BoxData box;
	box.Init();
	RenderManager::Instance()->SetVertices(box.vertices);
	RenderManager::Instance()->SetIndices(box.indices);

	PureColorShader *shader = new PureColorShader();
	RenderManager::Instance()->SetCurrentShader(shader);

	/*TriangleData triangle;
	triangle.Init();
	RenderManager::Instance()->SetVertices(triangle.vertices);
	RenderManager::Instance()->SetIndices(triangle.indices);*/


	gLight.position = Vector4(2, 1, 2);
	gLight.isDirectional = false;
	gLight.diretion = Vector4(1, 1, 1);
	gLight.color = Color::yellow;
}

float rx = 0;
float ry = 0;
void Update()
{
	Drawing::Instance()->Clear(0);

	rx += 0.01f;
	ry += 0.01f;
	
	Matrix4x4 worldMat = Matrix4x4::RotateX(rx) * Matrix4x4::RotateY(ry);
	RenderManager::Instance()->SetWorldMat(worldMat);
	RenderManager::Instance()->Render();
}