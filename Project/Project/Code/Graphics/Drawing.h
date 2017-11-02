#pragma once

#include "Misc\Settings.h"
#include "Misc\Tools.h"
#include "Graphics\DataStructure\Buffer.h"
#include "Graphics\DataStructure\Color.h"
#include "Graphics\Mesh.h"
#include "Math\Vector2.h"
#include "Math\Vector4.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include <cmath>

class Drawing
{
public:
	void (*NativeDrawPixel)(int x, int y, float r, float g, float b) = nullptr;
	static Drawing *Instance();

	void DrawPixel(int x, int y, float z, const Color &color);
	void DrawTriangle(Vector4 p1, Vector4 p2, Vector4 p3, Color color);
	void Clear(float v);
	void Render();
	Vector4 Project(const Vector4 &clipPos);
	Vector4 Project(const Vector4 &modelPos, const Matrix4x4 &transMat);
private:
	Drawing();
	~Drawing();
	void ProcessScanLine(int y, Vector4 pa, Vector4 pb, Vector4 pc, Vector4 pd, Color color);
private:
	static Drawing *mInstance;
	Buffer *mZBuffer;
};