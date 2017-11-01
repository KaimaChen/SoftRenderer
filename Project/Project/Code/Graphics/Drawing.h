#pragma once

#include "Misc\Settings.h"
#include "Misc\Tools.h"
#include "Graphics\Buffer.h"
#include "Graphics\DataStructure\Color.h"
#include "Graphics\Mesh.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include <cmath>

class Drawing
{
public:
	void (*NativeDrawPixel)(int x, int y, float r, float g, float b) = nullptr;
	static Drawing *Instance();

	void DrawPixel(int x, int y, float z, const Color &color);
	void DrawVertex(const Vector3 &vertex, const Matrix4x4 &transMat, const Color &color);
	void DrawTriangle(Vector3 p1, Vector3 p2, Vector3 p3, Color color);
	void Clear(float v);
	void Render();
	Vector3 Project(const Vector3 &clipPos);
	Vector3 Project(const Vector3 &modelPos, const Matrix4x4 &transMat);
private:
	Drawing();
	void ProcessScanLine(int y, Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, Color color);
private:
	static Drawing *mInstance;
	Buffer mZBuffer;
};