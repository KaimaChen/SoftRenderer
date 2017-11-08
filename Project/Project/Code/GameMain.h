/*
描述：
API设计为OpenGL类似的
渲染状态都存放在RenderManager
坐标系为右手系，如下：
		^ y
		 |
		 ------> x
	   /
	 /
   z

功能点：
* 线框模式与着色模式
* Z缓冲：可以设置比较方式（即glDepthFunc）
* 模板测试：支持glStencilFunc, glStencilOp的功能（但不支持Separate版的，因为我用不上）
* C++类形式的Vertex与Fragment Shader（参考Demo/Shaders/*）
* 逐像素光照（参考DiffuseShader）
* 贴图
	* 过滤模式：Nearest, Linear
	* 环绕(Wrap)：Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
* 透明度测试（使用Shader.h中定义的discard）
* 透明度混合
* 面剔除：正面、背面、双面
	* 可以设置CW或CCW为正面顶点顺序
* Mipmap

控制：
WSAD或方向键：摄像机前后左右移动
HJKL：物体旋转
鼠标中键：打印当前鼠标位置的颜色、深度值、模板值

TODO list：
* 使用多线程和SIMD提高性能
* 可以自定义顶点着色器输出结构而不是写死哪些部分可以插值
* 改用智能指针
* 多边形剪裁
* fbx导入
* 场景管理
* 场景切割
* DSL
* shader uniform
* 片元的dxy
* 提前深度测试 (Early Depth Testing)
* VAO, VBO, EBO
* glEnable那一系列功能
* gl_FragCoord

Bug:
* 当两个三角形共边时，渲染谁的？（比如两个面共边，一个面看不到，但是这条共边渲染了这个看不见的面） —— 左上填充规则？
* 边缘会有白点闪烁
* 距离稍微远点时有颗粒闪烁的感觉(mipmap?)
* 贴图的边缘颜色不对
* 透视投影矫正还是有问题

Some Done Bug:
* Blend时部分地方的颜色错误 —— 因为没有对Shader计算出来的颜色进行Clamp，导致有些分量的值超过1

疑惑：
* 当深度测试关闭时，对于模板测试来说深度测试的结果是什么？默认通过吗
*/

#pragma once

#include "Graphics\Drawing.h"
#include "Graphics\Helper\LineDrawing.h"
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
#include "Demo\Shaders\AlphaBlendShader.h"
#include "Demo\Shaders\GouraudShader.h"
#include "Demo\Shaders\ScreenQuadShader.h"

BoxData box;
TriangleData triangle;
QuadData quad;
void Init(void(*DrawPixel)(int x, int y, float r, float g, float b))
{
	Drawing::Instance()->NativeDrawPixel = DrawPixel; //Important

	Camera *camera = new Camera();
	camera->eye = Vector4(0, 0, 10);
	camera->at = Vector4::zero;
	camera->up = Vector4(0, 1, 0);
	camera->fov = 0.78f;
	camera->aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	camera->zn = 1.0f;
	camera->zf = 50.0f;
	RenderManager::Instance()->SetMainCamera(camera);

	box.Init();
	triangle.Init();
	quad.Init();

	Texture2D *texture = new Texture2D("./Resources/Checkerboard.png");
	texture->SetFilter(TextureFilter::Linear);
	texture->SetWrap(TextureWrap::MirroredRepeat);
	RenderManager::Instance()->SetTexture0(texture);

	Texture2D *noiseTex = new Texture2D("./Resources/noise.png");
	noiseTex->SetFilter(TextureFilter::Nearest);
	noiseTex->SetWrap(TextureWrap::Repeat);
	RenderManager::Instance()->SetTexture1(noiseTex);

	Light *mainLight = new Light();
	mainLight->position = Vector4(-1, 2, 2);
	mainLight->isDirectional = false;
	mainLight->color = Color::yellow;
	RenderManager::Instance()->SetMainLight(mainLight);

	RenderManager::Instance()->SetRenderMode(RenderMode::Shading);
	RenderManager::Instance()->glEnable(GL_CULL_FACE);
	//RenderManager::Instance()->glEnable(GL_DEPTH_TEST);
}

float rx = PI / 6.0f;
float ry = PI / 6.0f;
void Update()
{
	Matrix4x4 worldMat = Matrix4x4::identity;

	RenderManager::Instance()->SetVertices(box.vertices);
	RenderManager::Instance()->SetIndices(box.indices);

	RenderManager::Instance()->SetCurrentShader(new DiffuseShader());
	worldMat = Matrix4x4::RotateX(rx) * Matrix4x4::RotateY(ry);
	RenderManager::Instance()->SetWorldMat(worldMat);
	RenderManager::Instance()->Render();

	Vector4 lightPos = RenderManager::Instance()->MainLight()->position;
	worldMat = Matrix4x4::Scale(0.5f, 0.5f, 0.5f) * Matrix4x4::Translate(lightPos.x, lightPos.y, lightPos.z);
	RenderManager::Instance()->SetWorldMat(worldMat);
	RenderManager::Instance()->SetCurrentShader(new GouraudShader());
	RenderManager::Instance()->Render();

	Debug::isOneFrameDone = true;
}

void Clear()
{
	RenderManager::Instance()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

	if (keys['O'])
	{
		RenderManager::Instance()->SetRenderMode(RenderMode::WireFrame);
	}
	else if (keys['P'])
	{
		RenderManager::Instance()->SetRenderMode(RenderMode::Shading);
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