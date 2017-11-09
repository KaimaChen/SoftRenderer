/*
������
API���ΪOpenGL���Ƶ�
��Ⱦ״̬�������Context
����ϵΪ����ϵ�����£�
		^ y
		 |
		 ------> x
	   /
	 /
   z

���ܵ㣺
* �߿�ģʽ����ɫģʽ
* Z���壺�������ñȽϷ�ʽ����glDepthFunc��
* ģ����ԣ�֧��glStencilFunc, glStencilOp�Ĺ��ܣ�����֧��Separate��ģ���Ϊ���ò��ϣ�
* C++����ʽ��Vertex��Fragment Shader���ο�Demo/Shaders/*��
* �����ع��գ��ο�DiffuseShader��
* ��ͼ
	* ����ģʽ��Nearest, Linear
	* ����(Wrap)��Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
* ͸���Ȳ��ԣ�ʹ��Shader.h�ж����discard��
* ͸���Ȼ��
* ���޳������桢���桢˫��
	* ��������CW��CCWΪ���涥��˳��
* Mipmap

���ƣ�
WSAD������������ǰ�������ƶ�
HJKL��������ת
����м�����ӡ��ǰ���λ�õ���ɫ�����ֵ��ģ��ֵ

TODO list��
* ��������ָ��
* VAO, VBO, EBO -> ��Shader�����ݽ��� -> Shader����
* ʹ�ö��̺߳�SIMD�������
* �����Զ��嶥����ɫ������ṹ������д����Щ���ֿ��Բ�ֵ
* ����μ���
* fbx����
* ��������
* �����и�
* DSL
* shader uniform
* ƬԪ��dxy
* ��ǰ��Ȳ��� (Early Depth Testing)
* glEnable��һϵ�й���
* gl_FragCoord
* MSAA
* ��պ���
* Matrix4x4�Ŀռ�ֲ��Ժܲ�Ż�

Bug:
* �����������ι���ʱ����Ⱦ˭�ģ������������湲�ߣ�һ���濴��������������������Ⱦ��������������棩 ���� ����������
* ��Ե���а׵���˸
* ������΢Զ��ʱ�п�����˸�ĸо�(mipmap?)
* ��ͼ�ı�Ե��ɫ����
* ͸��ͶӰ��������������

Some Done Bug:
* Blendʱ���ֵط�����ɫ���� ���� ��Ϊû�ж�Shader�����������ɫ����Clamp��������Щ������ֵ����1

�ɻ�
* ����Ȳ��Թر�ʱ������ģ�������˵��Ȳ��ԵĽ����ʲô��Ĭ��ͨ����
*/

#pragma once

#include "Graphics\Drawing.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\DataStructure\Camera.h"
#include "Math\Matrix4x4.h"
#include "Misc\RenderState.h"
#include "Managers\Context.h"
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
	Context::Instance()->SetMainCamera(camera);

	box.Init();
	triangle.Init();
	quad.Init();

	Texture2D *texture = new Texture2D("./Resources/container.png");
	texture->SetFilter(TextureFilter::Linear);
	texture->SetWrap(TextureWrap::Repeat, TextureWrap::Repeat);
	Context::Instance()->SetTexture0(texture);

	Texture2D *noiseTex = new Texture2D("./Resources/noise.png");
	noiseTex->SetFilter(TextureFilter::Nearest);
	noiseTex->SetWrap(TextureWrap::Repeat, TextureWrap::Repeat);
	Context::Instance()->SetTexture1(noiseTex);

	Light *mainLight = new Light();
	mainLight->position = Vector4(-1, 2, 2);
	mainLight->isDirectional = false;
	mainLight->color = Color::yellow;
	Context::Instance()->SetMainLight(mainLight);

	Context::Instance()->SetRenderMode(RenderMode::Shading);
	Context::Instance()->glEnable(GL_CULL_FACE);
	Context::Instance()->glEnable(GL_DEPTH_TEST);
	//Context::Instance()->glEnable(GL_BLEND);
}

float rx = PI / 6.0f;
float ry = PI / 6.0f;
void Update()
{
	/*ShaderProgram *unlitTexProgram = new ShaderProgram();
	unlitTexProgram->Attach(new UnlitTexVertexShader());
	unlitTexProgram->Attach(new UnlitTexFragmentShader());

	ShaderProgram *pureColorProgram = new ShaderProgram();
	pureColorProgram->Attach(new PureColorVertexShader());
	pureColorProgram->Attach(new PureColorFragmentShader());
	pureColorProgram->SetColor(Color::red);

	ShaderProgram *screenQuadProgram = new ShaderProgram();
	screenQuadProgram->Attach(new ScreenQuadVertexShader());
	screenQuadProgram->Attach(new ScreenQuadFragmentShader());

	ShaderProgram *alphaBlendProgram = new ShaderProgram();
	alphaBlendProgram->Attach(new AlphaBlendVertexShader());
	alphaBlendProgram->Attach(new AlphaBlendFragmentShader());

	

	ShaderProgram *alphaTestProgram = new ShaderProgram();
	alphaTestProgram->Attach(new AlphaTestVertexShader());
	alphaTestProgram->Attach(new AlphaTestFragmentShader());*/

	ShaderProgram *gouraudProgram = new ShaderProgram();
	gouraudProgram->Attach(new GouraudVertexShader());
	gouraudProgram->Attach(new GouraudFragmentShader());

	ShaderProgram *diffuseProgram = new ShaderProgram();
	diffuseProgram->Attach(new DiffuseVertexShader());
	diffuseProgram->Attach(new DiffuseFragmentShader());

	Matrix4x4 worldMat = Matrix4x4::identity;

	Context::Instance()->SetVertices(box.vertices);
	Context::Instance()->SetIndices(box.indices);

	Context::Instance()->SetShaderProgram(diffuseProgram);
	worldMat = Matrix4x4::RotateX(rx) * Matrix4x4::RotateY(ry);
	Context::Instance()->SetWorldMat(worldMat);
	Context::Instance()->Render();

	/*Vector4 lightPos = Context::Instance()->MainLight()->position;
	worldMat = Matrix4x4::Scale(0.5f, 0.5f, 0.5f) * Matrix4x4::Translate(lightPos.x, lightPos.y, lightPos.z);
	Context::Instance()->SetWorldMat(worldMat);
	Context::Instance()->SetShaderProgram(gouraudProgram);
	Context::Instance()->Render();*/

	/*Context::Instance()->SetVertices(quad.vertices);
	Context::Instance()->SetIndices(quad.indices);
	Context::Instance()->SetCurrentShader(screenQuadProgram);
	Context::Instance()->SetWorldMat(worldMat);
	Context::Instance()->Render();*/

	Debug::isOneFrameDone = true;
}

void Clear()
{
	Context::Instance()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Render()
{
	Drawing::Instance()->Render();
}

#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
void ListenKeys(int *keys)
{
	Camera *mainCamera = Context::Instance()->MainCamera();
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
		Context::Instance()->SetRenderMode(RenderMode::WireFrame);
	}
	else if (keys['P'])
	{
		Context::Instance()->SetRenderMode(RenderMode::Shading);
	}
}

int lastMouseX = -1;
int lastMouseY = -1;
//mouseX: ��ǰ���������Ļλ��X
//mouseY: ��ǰ���������Ļλ��Y
//buttons: ��ǰ����״̬��0û����1�����2�Ҽ���
void ListenMouse(int mouseX, int mouseY, int buttons)
{
	Camera *mainCamera = Context::Instance()->MainCamera();
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