#include "Drawing.h"

Drawing *Drawing::mInstance = nullptr;

Drawing *Drawing::Instance()
{
	if (mInstance == nullptr)
		mInstance = new Drawing();
	return mInstance;
}

Drawing::Drawing()
{
	mBuffer = Buffer(SCREEN_WIDTH, SCREEN_HEIGHT, BufferType::Value);
}

void Drawing::DrawLine(int x1, int y1, int x2, int y2)
{
	//SimpleDrawLine(x1, y1, x2, y2);
	//DDADrawline(x1, y1, x2, y2);
	BresenhamDrawLine(x1, y1, x2, y2);
}

void Drawing::DrawLine(Vector2 p1, Vector2 p2)
{
	DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
}

void Drawing::SimpleDrawLine(int x1, int y1, int x2, int y2)
{
	float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	if (dist < 2)
		return;

	int midX = x1 + (x2 - x1) / 2.0f;
	int midY = y1 + (y2 - y1) / 2.0f;
	mBuffer.Set(midX, midY, 1);

	SimpleDrawLine(x1, y1, midX, midY);
	SimpleDrawLine(midX, midY, x2, y2);
}

//Digital Differential Analyzer (DDA) algorithm
void Drawing::DDADrawline(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps = 0;
	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);

	float xIncrement = dx / (float)steps;
	float yIncrement = dy / (float)steps;

	float x = (float)x1;
	float y = (float)y1;
	for (int v = 0; v < steps; v++)
	{
		x += xIncrement;
		y += yIncrement;
		mBuffer.Set((uint)x, (uint)y, 1);
	}
}

void Drawing::BresenhamDrawLine(int x1, int y1, int x2, int y2)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;

	while (true)
	{
		mBuffer.Set(x1, y1, 1);

		if ((x1 == x2) && (y1 == y2)) break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x1 += sx; }
		if (e2 < dx) { err += dx; y1 += sy; }
	}
}

void Drawing::DrawVertex(const Vector3 &vertex, const Matrix4x4 &transMat)
{
	Vector3 clipPos;
	transMat.PointMulMat(vertex, clipPos);
	Vector2 screenPos = Project(clipPos);
	DrawPixel((int)screenPos.x, (int)screenPos.y);
}

void Drawing::Clear(byte v)
{
	mBuffer.Clear(v);
}

void Drawing::Render()
{
	for (uint x = 0; x < mBuffer.GetWidth(); ++x)
	{
		for (uint y = 0; y < mBuffer.GetHeight(); ++y)
		{
			int index = (x + y * mBuffer.GetWidth());
			if (mBuffer[index] == 1)
				DrawPixel(x, y);
		}
	}
}

//[-1, 1]
Vector2 Drawing::Project(const Vector3 &clipPos)
{
	float x = clipPos.x / 2.0f * mBuffer.GetWidth() + mBuffer.GetWidth() / 2.0f;
	float y = clipPos.y / 2.0f * mBuffer.GetHeight() + mBuffer.GetHeight() / 2.0f;
	return Vector2(x, y);
}

Vector2 Drawing::Project(const Vector3 &modelPos, const Matrix4x4 &transMat)
{
	Vector3 clipPos;
	transMat.PointMulMat(modelPos, clipPos);
	return Project(clipPos);
}