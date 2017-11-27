#pragma once

#include "Managers\Context.h"

#include "Demo\Scenes\DiffuseScene.h"
#include "Demo\Scenes\AlphaTestScene.h"
#include "Demo\Scenes\TextureWrapScene.h"
#include "Demo\Scenes\AlphaBlendScene.h"
#include "Demo\Scenes\StencilTestScene.h"
#include "Demo\Scenes\MipmapScene.h"
#include "Demo\Scenes\TestScene.h"

enum State
{
	Running,
	Pause,
};
State state = State::Running;

enum DemoName
{
	DiffuseDemo,
	AlphaTestDemo,
	TextureWrapDemo,
	AlphaBlendDemo,
	StencilTestDemo,
	MipmapDemo,
	Test,
};
DemoName demoName = DemoName::StencilTestDemo;
DemoScene *scene;

void Init(void(*DrawPixel)(int x, int y, float r, float g, float b))
{
	Drawing::Instance()->NativeDrawPixel = DrawPixel;

	switch (demoName)
	{
	case DemoName::DiffuseDemo:
		scene = new DiffuseScene();
		break;
	case DemoName::AlphaTestDemo:
		scene = new AlphaTestScene();
		break;
	case DemoName::TextureWrapDemo:
		scene = new TextureWrapScene();
		break;
	case DemoName::AlphaBlendDemo:
		scene = new AlphaBlendScene();
		break;
	case DemoName::StencilTestDemo:
		scene = new StencilTestScene();
		break;
	case DemoName::MipmapDemo:
		scene = new MipmapScene();
		break;
	default:
		scene = new TestScene();
	}
	
	scene->Init();
}

void Update()
{
	if (state != State::Pause)
		scene->Update();
}

void Clear()
{
	if(state != State::Pause)
		Context::Instance()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Render()
{
	if (state != State::Pause)
		Drawing::Instance()->Render();
}

void ListenKeys(int *keys)
{
	#define VK_LEFT 0x25
	#define VK_UP 0x26
	#define VK_RIGHT 0x27
	#define VK_DOWN 0x28

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

	if (keys['Y'])
	{
		Context::Instance()->SetRenderMode(RenderMode::WireFrame);
	}
	else if (keys['U'])
	{
		Context::Instance()->SetRenderMode(RenderMode::Shading);
	}

	if (keys['O'])
		state = State::Running;
	else if (keys['P'])
		state = State::Pause;
}

int lastMouseX = -1;
int lastMouseY = -1;
//mouseX: 当前鼠标所在屏幕位置X
//mouseY: 当前鼠标所在屏幕位置Y
//buttons: 当前鼠标的状态（0没按，1左键，2右键）
void ListenMouse(int mouseX, int mouseY, int buttons)
{
	if(scene != nullptr)
		scene->ListenMouse(mouseX, mouseY, buttons);

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
	if (scene != nullptr)
		scene->ListenMiddleClick(mouseX, mouseY);
	Drawing::Instance()->Debug(mouseX, mouseY);
}