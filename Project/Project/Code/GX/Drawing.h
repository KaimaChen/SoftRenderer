#pragma once

#include "Misc\Settings.h"
#include "GX\Buffer.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Matrix4x4.h"
#include <cmath>

class Drawing
{
public:
	void (*DrawPixel)(int x, int y) = nullptr;
	static Drawing *Instance();
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawLine(Vector2 p1, Vector2 p2);
	void DrawVertex(const Vector3 &vertex, const Matrix4x4 &transMat);
	void Clear(byte v);
	void Render();
	Vector2 Project(const Vector3 &clipPos);
	Vector2 Project(const Vector3 &modelPos, const Matrix4x4 &transMat);
private:
	Drawing();
	void SimpleDrawLine(int x1, int y1, int x2, int y2);
	void DDADrawline(int x1, int y1, int x2, int y2);
	void BresenhamDrawLine(int x1, int y1, int x2, int y2);
private:
	static Drawing *mInstance;
	Buffer mBuffer;
};