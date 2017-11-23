#pragma once

#include "Graphics\Shaders\ShaderProgram.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"

class PureColorVertexShader : public VertexShader
{
public:
	virtual void RegisterAttribs() override
	{
		mAttrib4fIndices.push_back(colorIndex);
	}

	virtual void GetAttribs() override
	{
		mProgram->GetAttrib4f(colorIndex, mColor);
	}

	VertexOut Execute(const VertexIn &appdata) override
	{
		VertexOut v2f = VertexOut();
		v2f.clipPos = appdata.position * mProgram->GetMatrix(MVP_MAT_INDEX);
		v2f.color = Color(mColor[0], mColor[1], mColor[2], mColor[3]);
		return v2f;
	}

	int colorIndex = 1;
private:
	fvec4 mColor;
};

class PureColorFragmentShader : public FragmentShader
{
public:
	virtual void RegisterUniforms() override
	{
		mColorLocations.push_back(colorLocation);
	}

	virtual void GetUniforms() override
	{
		mProgram->GetColorUniform(colorLocation, mColor);
	}

	virtual Color Execute(const VertexOut &v2f) override
	{
		//return mColor;
		return v2f.color;
	}

	int colorLocation = 1;
private:
	Color mColor = Color::white;
};