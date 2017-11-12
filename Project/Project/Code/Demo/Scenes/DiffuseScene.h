#pragma once

#include "DemoScene.h"
#include "Demo\DemoData.h"
#include "Demo\Shaders\DiffuseShader.h"
#include "Managers\Context.h"

class DiffuseScene : public DemoScene
{
public:
	void Init() override 
	{
		mRx = mRy = 0;

		mBox.Init();

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
	}

	void Update() override
	{
		mRx += 0.1f;
		mRy += 0.1f;

		ShaderProgram *diffuseProgram = new ShaderProgram();
		diffuseProgram->Attach(new DiffuseVertexShader());
		diffuseProgram->Attach(new DiffuseFragmentShader());

		Matrix4x4 worldMat = Matrix4x4::identity;

		Context::Instance()->SetVertices(mBox.vertices);
		Context::Instance()->SetIndices(mBox.indices);

		Context::Instance()->SetShaderProgram(diffuseProgram);
		worldMat = Matrix4x4::RotateX(mRx) * Matrix4x4::RotateY(mRy);
		Context::Instance()->SetWorldMat(worldMat);
		Context::Instance()->Render();
	}

private:
	BoxData mBox;
	float mRx, mRy;
};