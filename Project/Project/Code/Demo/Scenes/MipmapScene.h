#pragma once

#include "DemoScene.h"
#include "Demo\DemoData.h"
#include "Demo\Shaders\PartScreenQuadShader.h"
#include "Managers\Context.h"

class MipmapScene : public DemoScene
{
public:
	void Init() override
	{
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
		Context::Instance()->SetTexture0(texture);

		Context::Instance()->SetRenderMode(RenderMode::Shading);
		Context::Instance()->glEnable(GL_CULL_FACE);
		Context::Instance()->glEnable(GL_DEPTH_TEST);
	}

	void Update() override
	{
		ShaderProgram *program = new ShaderProgram();
		PartScreenQuadVertexShader *vertexShader = new PartScreenQuadVertexShader();
		PartScreenQuadFragmentShader *fragmentShader = new PartScreenQuadFragmentShader();
		program->Attach(vertexShader);
		program->Attach(fragmentShader);
		program->Link();
		
		Context::Instance()->SetVertices(mQuad.vertices);
		Context::Instance()->SetIndices(mQuad.indices);
		Context::Instance()->SetShaderProgram(program);

		Matrix4x4 afterMVP = Matrix4x4::Scale(0.5f, 0.5f, 1.0f) * Matrix4x4::Translate(-0.5f, 0.5f, 0.0f);
		program->SetMatrix(afterMVP, vertexShader->afterMVPMatIndex);
		Context::Instance()->glUniform1i(fragmentShader->texLocation, 0);
		Context::Instance()->Render();

		afterMVP = Matrix4x4::Scale(0.5f, 0.5f, 1.0f) * Matrix4x4::Translate(-0.5f, -0.5f, 0.0f);
		program->SetMatrix(afterMVP, vertexShader->afterMVPMatIndex);
		Context::Instance()->glUniform1i(fragmentShader->texLocation, 1);
		Context::Instance()->Render();

		afterMVP = Matrix4x4::Scale(0.5f, 0.5f, 1.0f) * Matrix4x4::Translate(0.5f, 0.5f, 0.0f);
		program->SetMatrix(afterMVP, vertexShader->afterMVPMatIndex);
		Context::Instance()->glUniform1i(fragmentShader->texLocation, 2);
		Context::Instance()->Render();

		afterMVP = Matrix4x4::Scale(0.5f, 0.5f, 1.0f) * Matrix4x4::Translate(0.5f, -0.5f, 0.0f);
		program->SetMatrix(afterMVP, vertexShader->afterMVPMatIndex);
		Context::Instance()->glUniform1i(fragmentShader->texLocation, 3);
		Context::Instance()->Render();
	}

private:
	QuadData2 mQuad;
	GLuint mTexIds[4];
};