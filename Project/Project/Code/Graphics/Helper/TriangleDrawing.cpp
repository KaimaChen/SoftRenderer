#include "TriangleDrawing.h"

DrawPixelFunc TriangleDrawing::mDrawPixel = nullptr;
TriangleDrawing *TriangleDrawing::mInstance = nullptr;

TriangleDrawing *TriangleDrawing::Instance()
{
	if (mInstance == nullptr)
		mInstance = new TriangleDrawing();
	return mInstance;
}

void TriangleDrawing::T3DDrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram)
{
	Vector4 p0 = v0.screenPos;
	Vector4 p1 = v1.screenPos;
	Vector4 p2 = v2.screenPos;
	int x0 = (int)p0.x;
	int y0 = (int)p0.y;
	int x1 = (int)p1.x;
	int y1 = (int)p1.y;
	int x2 = (int)p2.x;
	int y2 = (int)p2.y;

	//三点共线
	if (y0 == y1 && y1 == y2)
	{
		return;
	}

	Sort(x0, y0, x1, y1, x2, y2, v0, v1, v2);

	if (y0 == y1)
		DrawBottomTriangle(v2, v0, v1, shaderProgram);
	else if (y1 == y2)
		DrawTopTriangle(v0, v1, v2, shaderProgram);
	//       p2
	//      /   \
		//    /       \
	// p1------newP
//     \         \
	//         \       \
	//             \    \
	//                 p0
	else
	{
		int newX = x0;
		if (x2 != x0)
		{
			float k = (float)(y2 - y0) / (x2 - x0);
			float b = y0 - k * x0;
			newX = (y1 - b) / k;
		}
		//用于计算重心坐标的中间变量
		float t1 = y1 - y2;
		float t2 = x2 - x1;
		float t3 = y2 - y0;
		float t4 = x0 - x2;
		float t = 1.0f / ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
		float w0 = (t1 * (newX - x2) + t2 * (y1 - y2)) * t;
		float w1 = (t3 * (newX - x2) + t4 * (y1 - y2)) * t;
		float w2 = 1 - w0 - w1;
		VertexOut newV = Interpolate(newX, y1, v0, v1, v2, w0, w1, w2);

		if (newX < x1)
			Tools::Swap<VertexOut>(v1, newV);

		DrawBottomTriangle(v2, v1, newV, shaderProgram);
		DrawTopTriangle(v0, newV, v1, shaderProgram);
	}
}

//*****************************************************************************
//绘制平底三角形
//    p0
//   /   \
// /       \
//p1-----p2
void TriangleDrawing::DrawBottomTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram)
{
	Vector4 p0 = v0.screenPos;
	Vector4 p1 = v1.screenPos;
	Vector4 p2 = v2.screenPos;
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;

	if (y0 == y1 || y0 == y2)
		return;

	//用于计算重心坐标的中间变量
	float t1 = y1 - y2;
	float t2 = x2 - x1;
	float t3 = y2 - y0;
	float t4 = x0 - x2;
	float t = 1.0f / ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));

	float kx0 = (float)(x0 - x1) / (y0 - y1);
	float kx1 = (float)(x0 - x2) / (y0 - y2);

	for (int y = y1; y <= y0; ++y)
	{
		float startX = x1 + kx0 * (y - y1);
		float endX = x2 + kx1 * (y - y1);

		for (int x = startX; x <= endX; ++x)
		{
			float w0 = (t1 * (x - x2) + t2 * (y - y2)) * t;
			float w1 = (t3 * (x - x2) + t4 * (y - y2)) * t;
			float w2 = 1 - w0 - w1;

			/*if (w0 > 0 && w1 > 0 && w2 > 0)
			mColorBuffer->Set(x, y, Color::white);*/

			VertexOut v2f = Interpolate(x, y, v0, v1, v2, w0, w1, w2);
			shaderProgram->SetFragCoord(v2f.screenPos);
			Color finalColor = shaderProgram->ExecuteFragmentShader(v2f);
			finalColor.Clamp();
			if (finalColor.isValid)
				mDrawPixel(x, y, v2f.screenPos.z, finalColor);
		}
	}
}

//*****************************************************************************
//绘制平顶三角形
//p2-------p1
//  \           /
//    \       /
//       p0
void TriangleDrawing::DrawTopTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram)
{
	Vector4 p0 = v0.screenPos;
	Vector4 p1 = v1.screenPos;
	Vector4 p2 = v2.screenPos;
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;

	if (y0 == y1 || y0 == y2)
		return;

	//用于计算重心坐标的中间变量
	float t1 = y1 - y2;
	float t2 = x2 - x1;
	float t3 = y2 - y0;
	float t4 = x0 - x2;
	float t = 1.0f / ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));

	float kx0 = (float)(x1 - x0) / (y1 - y0);
	float kx1 = (float)(x2 - x0) / (y2 - y0);

	for (int y = y0; y <= y1; ++y)
	{
		float startX = x0 + kx1 * (y - y0);
		float endX = x0 + kx0 * (y - y0);
		if (startX > endX)
			Tools::Swap<float>(startX, endX);

		for (int x = startX; x <= endX; ++x)
		{
			float w0 = (t1 * (x - x2) + t2 * (y - y2)) * t;
			float w1 = (t3 * (x - x2) + t4 * (y - y2)) * t;
			float w2 = 1 - w0 - w1;

			/*if (w0 > 0 && w1 > 0 && w2 > 0)
			mColorBuffer->Set(x, y, Color::white);*/

			VertexOut v2f = Interpolate(x, y, v0, v1, v2, w0, w1, w2);
			shaderProgram->SetFragCoord(v2f.screenPos);
			Color finalColor = shaderProgram->ExecuteFragmentShader(v2f);
			finalColor.Clamp();
			if (finalColor.isValid)
				mDrawPixel(x, y, v2f.screenPos.z, finalColor);
		}
	}
}

//*****************************************************************************
//先按y排序，y相同的再按x排序
void TriangleDrawing::Sort(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2, VertexOut &v0, VertexOut &v1, VertexOut &v2)
{
	//y排序
	if (y0 > y1)
	{
		Tools::Swap<int>(x0, x1);
		Tools::Swap<int>(y0, y1);
		Tools::Swap<VertexOut>(v0, v1);
	}
	if (y1 > y2)
	{
		Tools::Swap<int>(x1, x2);
		Tools::Swap<int>(y1, y2);
		Tools::Swap<VertexOut>(v1, v2);
	}
	if (y0 > y1)
	{
		Tools::Swap<int>(x0, x1);
		Tools::Swap<int>(y0, y1);
		Tools::Swap<VertexOut>(v0, v1);
	}

	//x排序
	if (y0 == y1)
	{
		if (x0 > x1)
		{
			Tools::Swap<int>(x0, x1);
			Tools::Swap<VertexOut>(v0, v1);
		}
	}
	if (y1 == y2)
	{
		if (x1 > x2)
		{
			Tools::Swap<int>(x1, x2);
			Tools::Swap<VertexOut>(v1, v2);
		}
	}
}

//*****************************************************************************
VertexOut TriangleDrawing::Interpolate(float x, float y, const VertexOut &v0, const VertexOut &v1, const VertexOut &v2, float w0, float w1, float w2)
{
	VertexOut v2f;

	float z = w0 * v0.screenPos.z + w1 * v1.screenPos.z + w2 * v2.screenPos.z;
	float iz = w0 * v0.inverseZ + w1 * v1.inverseZ + w2 * v2.inverseZ;
	Vector4 worldPos = Vector4::Interpolate(v0.worldPos, v1.worldPos, v2.worldPos, w0, w1, w2);
	Color color = Color::Interpolate(v0.color, v1.color, v2.color, w0, w1, w2);
	Vector2 uv = Vector2::Interpolate(v0.uv, v1.uv, v2.uv, w0, w1, w2);
	Vector4 normal = Vector4::Interpolate(v0.normal, v1.normal, v2.normal, w0, w1, w2);
	float vz = 1.0f / iz;

	v2f.screenPos = Vector4((float)x, (float)y, z);
	v2f.worldPos = worldPos;
	v2f.color = color;
	v2f.uv = uv;
	v2f.normal = normal;
	v2f.EndPerspectiveCorrectInterpolation(vz);

	return v2f;
}