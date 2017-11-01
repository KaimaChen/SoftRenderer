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
	mZBuffer = Buffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Drawing::DrawPixel(int x, int y, float z, const Color &color)
{
	int index = (x + y * mZBuffer.GetWidth());
	if (mZBuffer[index] < z)
		return;

	mZBuffer[index] = z;
	NativeDrawPixel(x, y, color.r, color.g, color.b);
}

void Drawing::DrawVertex(const Vector3 &vertex, const Matrix4x4 &transMat, const Color &color)
{
	Vector3 clipPos;
	transMat.PointMulMat(vertex, clipPos);
	Vector3 screenPos = Project(clipPos);
	DrawPixel(screenPos.x, screenPos.y, 0, Color::white);
}

//从左向右画线：papb->pcpd
void Drawing::ProcessScanLine(int y, Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, Color color)
{
	float gradient1 = (pa.y != pb.y) ? (y - pa.y) / (pb.y - pa.y) : 1;
	float gradient2 = (pc.y != pd.y) ? (y - pc.y) / (pd.y - pc.y) : 1;

	int sx = (int)Math::Interpolate(pa.x, pb.x, gradient1);
	int ex = (int)Math::Interpolate(pc.x, pd.x, gradient2);

	float z1 = Math::Interpolate(pa.z, pb.z, gradient1);
	float z2 = Math::Interpolate(pc.z, pd.z, gradient2);

	for (int x = sx; x < ex; ++x)
	{
		float gradient = (x - sx) / (float)(ex - sx);
		float z = Math::Interpolate(z1, z2, gradient);

		DrawPixel(x, y, z, color);
	}
}

void Drawing::DrawTriangle(Vector3 p1, Vector3 p2, Vector3 p3, Color color)
{
	//排序，以便从上到下依次为p1, p2, p3
	if (p1.y > p2.y)
		Tools::Swap<Vector3>(p1, p2);
	if (p2.y > p3.y)
		Tools::Swap<Vector3>(p2, p3);
	if (p1.y > p2.y)
		Tools::Swap<Vector3>(p1, p2);

	float dP1P2, dP1P3;

	if (p2.y - p1.y > 0) //实际上是保证不相等
		dP1P2 = (p2.x - p1.x) / (p2.y - p1.y);
	else
		dP1P2 = (p2.x == p1.x) ? 0 : FLT_MAX;

	if (p3.y - p1.y > 0)
		dP1P3 = (p3.x - p1.x) / (p3.y - p1.y);
	else
		dP1P3 = (p3.x == p1.x ) ? 0 : FLT_MAX;

	if (dP1P2 > dP1P3) //p2在右
	{
		for (int y = p1.y; y <= p3.y; ++y)
		{
			if (y < p2.y)
				ProcessScanLine(y, p1, p3, p1, p2, color);
			else
				ProcessScanLine(y, p1, p3, p2, p3, color);
		}
	}
	else
	{
		for (int y = p1.y; y <= p3.y; ++y)
		{
			if (y < p2.y)
				ProcessScanLine(y, p1, p2, p1, p3, color);
			else
				ProcessScanLine(y, p2, p3, p1, p3, color);
		}
	}
}

void Drawing::Clear(float v)
{
	mZBuffer.Clear(FLT_MAX);
}

void Drawing::Render()
{
	/*for (uint x = 0; x < mBuffer.GetWidth(); ++x)
	{
		for (uint y = 0; y < mBuffer.GetHeight(); ++y)
		{
			int index = (x + y * mBuffer.GetWidth());
			if (mBuffer[index] == 1)
				DrawPixel(x, y, Color::red);
		}
	}*/
	//TODO
	/*for (int i = 0; i < mBuffer.GetSize(); i += 4)
	{
		float r = mBuffer[i] / 255.0f;
		float g = mBuffer[i + 1] / 255.0f;
		float b = mBuffer[i + 2] / 255.0f;
		float a = mBuffer[i + 3] / 255.0f;
		int y = i / mBuffer.GetWidth();
		int x = i - y * mBuffer.GetWidth();
		DrawPixel(x, y, 0, Color(r, g, b, a));
	}*/
}

//[-1, 1]
Vector3 Drawing::Project(const Vector3 &clipPos)
{
	float x = clipPos.x / 2.0f * SCREEN_WIDTH + SCREEN_WIDTH / 2.0f;
	float y = clipPos.y / 2.0f * SCREEN_HEIGHT + SCREEN_HEIGHT / 2.0f;
	return Vector3(x, y, clipPos.z);
}

Vector3 Drawing::Project(const Vector3 &modelPos, const Matrix4x4 &transMat)
{
	Vector3 clipPos;
	transMat.PointMulMat(modelPos, clipPos);
	return Project(clipPos);
}