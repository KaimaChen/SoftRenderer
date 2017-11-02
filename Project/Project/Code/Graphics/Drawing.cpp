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
	mZBuffer = new Buffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Drawing::~Drawing()
{
	delete mZBuffer;
	mZBuffer = nullptr;
}

void Drawing::DrawPixel(int x, int y, float z, const Color &color)
{
	if (mZBuffer->Get(x, y) < z)
		return;

	mZBuffer->Set(x, y, z);
	NativeDrawPixel(x, y, color.r, color.g, color.b);
}

//从左向右画线：papb->pcpd
void Drawing::ProcessScanLine(int y, Vector4 pa, Vector4 pb, Vector4 pc, Vector4 pd, Color color)
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

void Drawing::DrawTriangle(Vector4 p1, Vector4 p2, Vector4 p3, Color color)
{
	//排序，以便从上到下依次为p1, p2, p3
	if (p1.y > p2.y)
		Tools::Swap<Vector4>(p1, p2);
	if (p2.y > p3.y)
		Tools::Swap<Vector4>(p2, p3);
	if (p1.y > p2.y)
		Tools::Swap<Vector4>(p1, p2);

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
	mZBuffer->Clear(FLT_MAX);
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
Vector4 Drawing::Project(const Vector4 &clipPos)
{
	float x = clipPos.x / 2.0f * SCREEN_WIDTH + SCREEN_WIDTH / 2.0f;
	float y = clipPos.y / 2.0f * SCREEN_HEIGHT + SCREEN_HEIGHT / 2.0f;
	return Vector4(x, y, clipPos.z);
}

Vector4 Drawing::Project(const Vector4 &modelPos, const Matrix4x4 &transMat)
{
	Vector4 clipPos = modelPos * transMat;
	return Project(clipPos);
}