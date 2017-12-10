#pragma once

#include "DemoScene.h"
#include "Demo\DemoData.h"
#include "Demo\Shaders\ScreenQuadShader.h"
#include "Managers\Context.h"

class TextureWrapScene : public DemoScene
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

		Context::Instance()->SetRenderMode(RenderMode::Shading);
		Context::Instance()->glEnable(GL_CULL_FACE);
		Context::Instance()->glEnable(GL_DEPTH_TEST);

		const char *path = "./Resources/container.png";
		int width, height, channelNum;
		ubyte *data = stbi_load(path, &width, &height, &channelNum, 0);
		GLenum format = GL_RGB;
		if (channelNum == 4)
			format = GL_RGBA;

		GLuint texId;
		Context::Instance()->glGenTextures(1, &texId);
		Context::Instance()->glBindTexture(GL_TEXTURE_2D, texId);
		Context::Instance()->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}

	void Update() override
	{
		ShaderProgram *diffuseProgram = new ShaderProgram();
		diffuseProgram->Attach(new ScreenQuadVertexShader());
		diffuseProgram->Attach(new ScreenQuadFragmentShader());

		Matrix4x4 worldMat = Matrix4x4::identity;

		Context::Instance()->SetVertices(mQuad.vertices);
		Context::Instance()->SetIndices(mQuad.indices);

		Context::Instance()->SetShaderProgram(diffuseProgram);
		Context::Instance()->SetWorldMat(worldMat);
		Context::Instance()->Render();
	}

private:
	QuadData mQuad;
};