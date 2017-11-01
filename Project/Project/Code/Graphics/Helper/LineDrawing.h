#pragma once

#include <cmath>

class LineDrawing
{
public:
	static void(*NativeDrawPixel)(int x, int y, float r, float g, float b);
public:
	static void SimpleDrawLine(int x0, int y0, int x1, int y1);
	static void DDADrawline(int x0, int y0, int x1, int y1);
	static void BresenhamDrawLine(int x0, int y0, int x1, int y1);
};