#pragma once

#include <cmath>
#include <functional>
#include "Graphics\DataStructure\Color.h"

//实现各种画线算法
class LineDrawing
{
private:
	using Callback = std::function<void(int x, int y, float z, const Color &color)>;
public:
	static void SimpleDrawLine(int x0, int y0, int x1, int y1, Callback drawPixel);
	static void DDADrawline(int x0, int y0, int x1, int y1, Callback drawPixel);
	static void BresenhamDrawLine(int x0, int y0, int x1, int y1, Callback drawPixel);
};