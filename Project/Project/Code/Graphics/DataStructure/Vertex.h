#pragma once

#include "Math\Vector2.h"
#include "Math\Vector4.h"
#include "Color.h"

struct VertexIn
{
	VertexIn() : position(Vector4::zero), normal(Vector4::zero), uv(Vector2::zero), color(Color::white) {}
	VertexIn(Vector4 pos, Vector4 n, Vector2 st, Color col) : position(pos), normal(n), uv(st), color(col) {}

	Vector4 position;
	Vector4 normal;
	Vector2 uv;
	Color color;
};

struct VertexOut
{
	VertexOut() {};
	VertexOut(Vector4 wp, Vector4 cp, Vector4 n, Vector2 st, Color col, float iz) : worldPos(wp), clipPos(cp), normal(n), uv(st), color(col), inverseZ(iz) {}

	Vector4 worldPos;
	Vector4 clipPos;
	Vector4 normal;
	Vector2 uv;
	Color color;
	float inverseZ;
};