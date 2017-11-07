#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include "Math\Math.h"
#include "Graphics\DataStructure\Color.h"
#include "Misc\Tools.h"

//实现各种三角形光栅化方法
class TriangleDrawing
{
private:
	using Callback = std::function<void(int x, int y, float z, const Color &color)>;
public:
	static void T3DDrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Callback drawPixel);
private:
	static void DrawLine(int xs, int xe, int y, Callback drawPixel);
	static void DrawTriangleTop(int x0, int y0, int x1, int y1, int x2, int y2, Callback drawPixel);
	static void DrawTriangleBottom(int x0, int y0, int x1, int y1, int x2, int y2, Callback drawPixel);
	static void Sort(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2);
};