#include "LineDrawing.h"

void (*LineDrawing::NativeDrawPixel)(int x, int y, float r, float g, float b) = nullptr;

void LineDrawing::SimpleDrawLine(int x0, int y0, int x1, int y1)
{
	if (NativeDrawPixel == nullptr)
		return;

	float dist = sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
	if (dist < 2)
		return;

	int midX = x0 + (x1 - x0) / 2.0f;
	int midY = y0 + (y1 - y0) / 2.0f;
	NativeDrawPixel(midX, midY, 1, 1, 1);

	SimpleDrawLine(x0, y0, midX, midY);
	SimpleDrawLine(midX, midY, x1, y1);
}

//Digital Differential Analyzer (DDA) algorithm
void LineDrawing::DDADrawline(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int steps = 0;
	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);

	float xIncrement = dx / (float)steps;
	float yIncrement = dy / (float)steps;

	float x = (float)x0;
	float y = (float)y0;
	for (int v = 0; v < steps; v++)
	{
		x += xIncrement;
		y += yIncrement;
		NativeDrawPixel(x, y, 1, 1, 1);
	}
}

void LineDrawing::BresenhamDrawLine(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true)
	{
		NativeDrawPixel(x0, y0, 1, 1, 1);

		if ((x0 == x1) && (y0 == y1)) break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}
}