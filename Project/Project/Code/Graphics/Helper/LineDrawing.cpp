#include "LineDrawing.h"

//*****************************************************************************
//思路：不断找到中点来画
void LineDrawing::SimpleDrawLine(int x0, int y0, int x1, int y1, Callback drawPixel)
{
	if (drawPixel == nullptr)
		return;

	float dist = sqrtf(powf(x0 - x1, 2) + powf(y0 - y1, 2));
	if (dist < 2)
		return;

	int midX = int(x0 + (x1 - x0) / 2.0f);
	int midY = int(y0 + (y1 - y0) / 2.0f);
	drawPixel(midX, midY, 0, Color::white);

	SimpleDrawLine(x0, y0, midX, midY, drawPixel);
	SimpleDrawLine(midX, midY, x1, y1, drawPixel);
}

//*****************************************************************************
//Digital Differential Analyzer (DDA) algorithm
void LineDrawing::DDADrawline(int x0, int y0, int x1, int y1, Callback drawPixel)
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
		drawPixel(x, y, 0, Color::white);
	}
}

//*****************************************************************************
void LineDrawing::BresenhamDrawLine(int x0, int y0, int x1, int y1, Callback drawPixel)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true)
	{
		drawPixel(x0, y0, 0, Color::white);

		if ((x0 == x1) && (y0 == y1)) break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}
}