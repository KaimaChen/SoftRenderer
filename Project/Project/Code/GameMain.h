/*
描述：
右手系如下：
		^ y
		 |
		 ------> x
	   /
	 /
   z

功能点：
* 线框模式与着色模式
* C++类形式的Vertex与Fragment Shader（参考Demo/Shaders/*）
* 逐像素光照（参考DiffuseShader）
* 贴图
	* 过滤模式：Nearest, Linear
	* 环绕(Wrap)：Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
* 透明度测试（使用Shader.h中定义的discard）
* 透明度混合
* 面剔除：正面、背面、双面
	* 可以设置CW或CCW为正面顶点顺序

控制：
WSAD或方向键：摄像机前后左右移动
HJKL：物体旋转
鼠标中键：打印当前鼠标位置的颜色、深度值

TODO list：
* 使用多线程和SIMD提高性能
* Mipmap
* Stencil buffer
* 可以自定义顶点着色器输出结构而不是写死哪些部分可以插值
* 改用智能指针
* 多边形剪裁
* fbx导入
* 场景管理
* 场景切割
* DSL
* shader uniform

Bug:
* 当两个三角形共边时，渲染谁的？（比如两个面共边，一个面看不到，但是这条共边渲染了这个看不见的面）
* 边缘会有白点闪烁
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
#include "Demo\Shaders\AlphaTestShader.h"
#include "Demo\Shaders\AlphaBlend.h"

void Init(void(*DrawPixel)(int x, int y, float r, float g, float b))
{
	Drawing::Instance()->NativeDrawPixel = DrawPixel; //Important

	Camera *camera = new Camera();
	camera->eye = Vector4(0, 0, 5);
	camera->at = Vector4::zero;
	camera->up = Vector4(0, 1, 0);
	camera->fov = 0.78f;
	camera->aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	camera->zn = 0.1f;
	camera->zf = 10.0f;
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

	Texture2D *texture = new Texture2D("./Resources/container.png");
	texture->SetFilter(TextureFilter::Linear);
	texture->SetWrap(TextureWrap::MirroredRepeat);
	RenderManager::Instance()->SetTexture0(texture);

	Texture2D *noiseTex = new Texture2D("./Resources/noise.png");
	noiseTex->SetFilter(TextureFilter::Nearest);
	noiseTex->SetWrap(TextureWrap::Repeat);
	RenderManager::Instance()->SetTexture1(noiseTex);

	Light *mainLight = new Light();
	mainLight->position = Vector4(3, 2, -3);
	mainLight->isDirectional = false;
	mainLight->color = Color::yellow;
	RenderManager::Instance()->SetMainLight(mainLight);

	RenderManager::Instance()->SetRenderMode(RenderMode::Shading);
	RenderManager::Instance()->SetBlendState(true);
	RenderManager::Instance()->SetCullFace(CullFace::CullBack);
	//RenderManager::Instance()->SetFrontFace(ClockDirection::CW);
}

float rx = PI / 6.0f;
float ry = PI / 6.0f;
void Update()
{
	Matrix4x4 worldMat;

	Vector4 lightPos = RenderManager::Instance()->MainLight()->position;
	worldMat = Matrix4x4::Translate(lightPos.x, lightPos.y, lightPos.z);
	RenderManager::Instance()->SetWorldMat(worldMat);
	RenderManager::Instance()->SetCurrentShader(new PureColorShader());
	RenderManager::Instance()->Render();

	worldMat = Matrix4x4::RotateX(rx) * Matrix4x4::RotateY(ry);
	RenderManager::Instance()->SetWorldMat(worldMat);
	RenderManager::Instance()->SetCurrentShader(new DiffuseShader());
	RenderManager::Instance()->Render();

	

	Debug::isOneFrameDone = true;
}

void Clear()
{
	Drawing::Instance()->Clear(Color::black, FLT_MAX);
}

void Render()
{
	Drawing::Instance()->Render();
}

void ListenKeys(int *keys)
{
	Camera *mainCamera = RenderManager::Instance()->MainCamera();
	if (keys[VK_LEFT] || keys['A'])
	{
		mainCamera->eye.x -= 0.1f;
		mainCamera->at.x -= 0.1f;
	}
	if (keys[VK_RIGHT] || keys['D'])
	{
		mainCamera->eye.x += 0.1f;
		mainCamera->at.x += 0.1f;
	}
	if (keys[VK_UP] || keys['W'])
	{
		mainCamera->eye.z -= 0.1f;
		mainCamera->at.z -= 0.1f;
	}
	if (keys[VK_DOWN] || keys['S'])
	{
		mainCamera->eye.z += 0.1f;
		mainCamera->at.z += 0.1f;
	}
	if (keys['Q'])
	{
		mainCamera->eye.y += 0.1f;
		mainCamera->at.y += 0.1f;
	}
	if (keys['E'])
	{
		mainCamera->eye.y -= 0.1f;
		mainCamera->at.y -= 0.1f;
	}
	if (keys['Z'])
	{
		mainCamera->at.x -= 0.1f;
	}
	if (keys['C'])
	{
		mainCamera->at.x += 0.1f;
	}

	if (keys['H'])
	{
		rx += 0.05f;
	}
	if (keys['J'])
	{
		rx -= 0.05f;
	}
	if (keys['K'])
	{
		ry += 0.05f;
	}
	if (keys['L'])
	{
		ry -= 0.05f;
	}
}

int lastMouseX = -1;
int lastMouseY = -1;
//mouseX: 当前鼠标所在屏幕位置X
//mouseY: 当前鼠标所在屏幕位置Y
//buttons: 当前鼠标的状态（0没按，1左键，2右键）
void ListenMouse(int mouseX, int mouseY, int buttons)
{
	Camera *mainCamera = RenderManager::Instance()->MainCamera();
	if (buttons == 1)
	{
		if (lastMouseX == -1 || lastMouseY == -1)
		{
			lastMouseX = mouseX;
			lastMouseY = mouseY;
		}

		int dx = mouseX - lastMouseX;
		int dy = mouseY - lastMouseY;
		mainCamera->eye.x -= (float)dx / SCREEN_WIDTH;
		mainCamera->at.x -= (float)dx / SCREEN_WIDTH;
		mainCamera->eye.y -= (float)dy / SCREEN_HEIGHT;
		mainCamera->at.y -= (float)dy / SCREEN_HEIGHT;
	}

	if (buttons != 1)
	{
		lastMouseX = lastMouseY = -1;
	}
}

void ListenMiddleClick(int mouseX, int mouseY)
{
	Drawing::Instance()->Debug(mouseX, mouseY);
}