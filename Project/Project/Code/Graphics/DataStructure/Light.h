#pragma once

#include "Math\Vector4.h"
#include "Graphics\DataStructure\Color.h"

struct Light
{
	Light()
	{
		position = Vector4::zero;
		isDirectional = true;
		diretion = Vector4::one;
		color = Color::white;
	}

	Vector4 position;
	bool isDirectional;
	Vector4 diretion;
	Color color;
};