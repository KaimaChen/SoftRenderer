#pragma once

#include "Math\Vector2.h"
#include "Math\Vector4.h"
#include "Math\Math.h"
#include "Graphics\DataStructure\Color.h"

struct VertexIn
{
	VertexIn() : position(Vector4::zero), normal(Vector4::zero), uv(Vector2::zero), color(Color::white) {}
	VertexIn(Vector4 pos, Vector4 n, Vector2 st, Color col) : position(pos), normal(n), uv(st), color(col) {}

	Vector4 position;
	Vector4 normal;
	Vector2 uv;
	Color color;
};

//*****************************************************************************
struct VertexOut
{
	VertexOut() {};

	//https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes
	void BeginPerspectiveCorrectInterpolation()
	{
		//if (Math::Approximate(clipPos.z, 0)) //±‹√‚≥˝0Œ Ã‚
		//	clipPos.z = FLT_MIN;

		//inverseZ = 1.0f / clipPos.z;
		//worldPos *= inverseZ; 
		//normal *= inverseZ;
		//uv *= inverseZ;
		//color *= inverseZ;
	}

	void EndPerspectiveCorrectInterpolation(float z)
	{
		/*worldPos *= z;
		normal *= z;
		uv *= z;
		color *= z;*/
	}

	Vector4 clipPos;
	Vector4 screenPos;
	Vector4 worldPos;	
	Vector4 normal;
	Vector2 uv;
	Color color;
	float inverseZ;
};