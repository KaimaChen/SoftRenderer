/*
描述：
右手系如下：
		^ y
		 |
		 ------> x
		/
	  /
	 z

TODO list：
0. 监听键盘
1. 使用多线程提高性能
2. 编写着色器编译器
3. Mipmap

Bug:
1. 当两个三角形共边时，渲染谁的？
*/

#pragma once

#include "Graphics\Drawing.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Mesh.h"
#include "Graphics\DataStructure\Camera.h"
#include "Math\Matrix4x4.h"
#include "Misc\RenderState.h"
#include "Managers\RenderManager.h"
#include "Demo\DemoData.h"
#include "Graphics\Texture2D.h"
#include "Demo\Shaders\PureColorShader.h"
#include "Demo\Shaders\UnlitTexShader.h"
#include "Demo\Shaders\DiffuseShader.h"

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
	camera.zn = 0.1f;
	camera.zf = 10.0f;
	RenderManager::Instance()->SetMainCamera(camera);

	BoxData box;
	box.Init();
	RenderManager::Instance()->SetVertices(box.vertices);
	RenderManager::Instance()->SetIndices(box.indices);

	/*TriangleData triangle;
	triangle.Init();
	RenderManager::Instance()->SetVertices(triangle.vertices);
	RenderManager::Instance()->SetIndices(triangle.indices);*/

	/*QuadData quad;
	quad.Init();
	RenderManager::Instance()->SetVertices(quad.vertices);
	RenderManager::Instance()->SetIndices(quad.indices); */

	//PureColorShader *shader = new PureColorShader();
	//UnlitTexShader *shader = new UnlitTexShader();
	DiffuseShader *shader = new DiffuseShader();
	RenderManager::Instance()->SetCurrentShader(shader);

	Texture2D *texture = new Texture2D("./Resources/container.png");
	texture->SetFilter(TextureFilter::Linear);
	texture->SetWrap(TextureWrap::MirroredRepeat);
	RenderManager::Instance()->SetCurrentTexture(texture);

	Light *mainLight = new Light();
	mainLight->position = Vector4(2, 1, 2);
	mainLight->isDirectional = false;
	mainLight->color = Color::yellow;
	RenderManager::Instance()->SetMainLight(mainLight);
}

float rx = PI / 6.0f;
float ry = PI / 6.0f;
void Update()
{
	Drawing::Instance()->Clear(Color::black, FLT_MAX);

	//rx = ry = 0;
	rx += 0.01f;
	ry += 0.01f;
	
	Matrix4x4 worldMat = Matrix4x4::RotateX(rx) * Matrix4x4::RotateY(ry);
	RenderManager::Instance()->SetWorldMat(worldMat);
	RenderManager::Instance()->Render();

	Drawing::Instance()->Render();
}