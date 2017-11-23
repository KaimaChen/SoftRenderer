#pragma once

#include "DemoScene.h"
#include "Demo\DemoData.h"
#include "Demo\Shaders\DiffuseShader.h"
#include "Demo\Shaders\UnlitTexShader.h"
#include "Demo\Shaders\PureColorShader.h"
#include "Managers\Context.h"

class TestScene : public DemoScene
{
public:
	void Init() override
	{
		mRx = mRy = 0;
		x0 = 100;
		y0 = 150;
		x1 = 10;
		y1 = 50;
		x2 = 150;
		y2 = 50;

		mBox.Init();
		mBox2.Init();
		mQuad.Init();

		Camera *camera = new Camera();
		camera->eye = Vector4(0, 0, 10);
		camera->at = Vector4::zero;
		camera->up = Vector4(0, 1, 0);
		camera->fov = 0.78f;
		camera->aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		camera->zn = 1.0f;
		camera->zf = 50.0f;
		Context::Instance()->SetMainCamera(camera);

		Light *mainLight = new Light();
		mainLight->position = Vector4(-1, 2, 2);
		mainLight->isDirectional = false;
		mainLight->color = Color::yellow;
		Context::Instance()->SetMainLight(mainLight);

		Texture2D *texture = new Texture2D("./Resources/container.png");
		texture->SetFilter(GL_LINEAR);
		texture->SetWrap(GL_REPEAT, GL_REPEAT);
		Context::Instance()->SetTexture0(texture);

		Context::Instance()->SetRenderMode(RenderMode::Shading);
		Context::Instance()->glEnable(GL_CULL_FACE);
		Context::Instance()->glEnable(GL_DEPTH_TEST);
		Context::Instance()->glEnable(GL_BLEND);
		Context::Instance()->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Update() override
	{
		mRx += 0.1f;
		mRy += 0.1f;

		ShaderProgram *diffuseProgram = new ShaderProgram();
		diffuseProgram->Attach(new DiffuseVertexShader());
		diffuseProgram->Attach(new DiffuseFragmentShader());
		diffuseProgram->Link();

		ShaderProgram *unlitTexProgram = new ShaderProgram();
		unlitTexProgram->Attach(new UnlitTexVertexShader());
		unlitTexProgram->Attach(new UnlitTexFragmentShader());
		unlitTexProgram->Link();

		ShaderProgram *pureColorProgram = new ShaderProgram();
		PureColorVertexShader *pureColorVS = new PureColorVertexShader();
		PureColorFragmentShader *pureColorFS = new PureColorFragmentShader();
		pureColorProgram->Attach(pureColorVS);
		pureColorProgram->Attach(pureColorFS);
		pureColorProgram->Link();

		Matrix4x4 worldMat = Matrix4x4::identity;

		Context::Instance()->SetVertices(mBox.vertices);
		Context::Instance()->SetIndices(mBox.indices);
		//Context::Instance()->SetShaderProgram(diffuseProgram);
		Context::Instance()->SetShaderProgram(pureColorProgram);
		Context::Instance()->glUniform4f(pureColorFS->colorLocation, 1, 1, 0, 1);
		Context::Instance()->glVertexAttrib4f(pureColorVS->colorIndex, 1, 0, 0, 1);
		worldMat = Matrix4x4::RotateX(mRx) * Matrix4x4::RotateY(mRy) * Matrix4x4::Translate(0, 0, 0);
		Context::Instance()->SetWorldMat(worldMat);
		Context::Instance()->Render();
	}

	void ListenMouse(int mouseX, int mouseY, int buttons) override
	{
		if (buttons == 1)
		{
			x0 = mouseX;
			y0 = mouseY;
		}
		else if (buttons == 2)
		{
			x1 = mouseX;
			y1 = mouseY;
		}
	}

	void ListenMiddleClick(int mouseX, int mouseY) override
	{
		x2 = mouseX;
		y2 = mouseY;
	}

private:
	BoxData mBox, mBox2;
	QuadData mQuad;
	float mRx, mRy;
	int x0, y0, x1, y1, x2, y2;
};