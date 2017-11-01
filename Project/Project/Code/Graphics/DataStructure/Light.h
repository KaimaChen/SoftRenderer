#pragma once

#include "Math\Vector3.h"
#include "Graphics\DataStructure\Color.h"

struct Light
{
	Light();
	Vector3 position;
	bool isDirectional;
	Vector3 diretion;
	Color color;
};

Light::Light()
{
	position = Vector3::zero;
	isDirectional = true;
	diretion = Vector3::one;
	color = Color::white;
}