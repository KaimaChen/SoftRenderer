#pragma once

#include <cmath>
#include <functional>
#include "Graphics\Shaders\ShaderProgram.h"
#include "Misc\Tools.h"

using DrawPixelFunc = std::function<void(int x, int y, float z, const Color &color)>;

class TriangleDrawing
{
private:
	static DrawPixelFunc mDrawPixel;
	static TriangleDrawing *mInstance;
	TriangleDrawing() {}
public:
	static TriangleDrawing *Instance();
	void SetDrawPixel(DrawPixelFunc drawPixel) { mDrawPixel = drawPixel; }
	void T3DDrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shader);
private:
	void DrawBottomTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram);
	void DrawTopTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram);
	void Sort(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2, VertexOut &v0, VertexOut &v1, VertexOut &v2);
	VertexOut Interpolate(float x, float y, const VertexOut &v0, const VertexOut &v1, const VertexOut &v2, float w0, float w1, float w2);
};