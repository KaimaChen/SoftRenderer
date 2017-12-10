#pragma once

#include "Graphics\Shaders\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include "Managers\Context.h"

class UnlitTexVertexShader : public VertexShader
{
public:
	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mProgram->GetMatrix(MVP_MAT_INDEX);
		v2f.uv = appdata.uv;
		return v2f;
	}
};

class UnlitTexFragmentShader : public FragmentShader
{
public:
	virtual void RegisterUniforms() override
	{
		mIntLocations.push_back(texLocation);
	}

	virtual void GetUniforms() override
	{
		mProgram->GetUniform1i(texLocation, mTex);
	}

	virtual void PreExecute() override
	{
		mTexture = Context::Instance()->GetTexture2D(mTex);
	}

	Color Execute(const VertexOut &v2f) override
	{
		Color albedo = mTexture->Read(v2f.uv);
		return albedo;
	}

	int texLocation = 1;
private:
	int mTex = 0;
	Texture2D *mTexture = nullptr;
};