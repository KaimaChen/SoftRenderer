#include "Drawing.h"
#include "Managers\RenderManager.h"

Drawing *Drawing::mInstance = nullptr;

Drawing *Drawing::Instance()
{
	if (mInstance == nullptr)
		mInstance = new Drawing();
	return mInstance;
}

Drawing::Drawing()
{
	mColorBuffer = new ColorBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	mZBuffer = new Buffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Drawing::~Drawing()
{
	if (mZBuffer)
	{
		delete mZBuffer;
		mZBuffer = nullptr;
	}
	if (mColorBuffer)
	{
		delete mColorBuffer;
		mColorBuffer = nullptr;
	}
}

void Drawing::DrawPixel(int x, int y, float z, const Color &color)
{
	if (mZBuffer->Get(x, y) < z) //深度比较
		return;

	Color finalColor = color;
	if (RenderManager::Instance()->IsBlendEnabled()) //透明度混合
	{
		Color originColor = mColorBuffer->Get(x, y);
		finalColor = color * color.a + originColor * (1 - color.a);
		finalColor.a = color.a;
	}

	mZBuffer->Set(x, y, z);
	mColorBuffer->Set(x, y, finalColor);
}

//从左向右画线：papb->pcpd
//TODO: 插值是性能爆炸点！！！一个Vector4差不多要丢10帧
void Drawing::ProcessScanLine(int y, VertexOut va, VertexOut vb, VertexOut vc, VertexOut vd, Shader *shader)
{
	VertexOut v2f = VertexOut();

	Vector4 pa = va.screenPos;
	Vector4 pb = vb.screenPos;
	Vector4 pc = vc.screenPos;
	Vector4 pd = vd.screenPos;

	float gradient1 = (pa.y != pb.y) ? (y - pa.y) / (pb.y - pa.y) : 1;
	float gradient2 = (pc.y != pd.y) ? (y - pc.y) / (pd.y - pc.y) : 1;

	int sx = (int)Math::Interpolate(pa.x, pb.x, gradient1);
	int ex = (int)Math::Interpolate(pc.x, pd.x, gradient2);

	float z1 = Math::Interpolate(pa.z, pb.z, gradient1);
	float z2 = Math::Interpolate(pc.z, pd.z, gradient2);

	float iz1 = Math::Interpolate(va.inverseZ, vb.inverseZ, gradient1);
	float iz2 = Math::Interpolate(vc.inverseZ, vd.inverseZ, gradient2);

	Vector4 w1 = Vector4::Interpolate(va.worldPos, vb.worldPos, gradient1);
	Vector4 w2 = Vector4::Interpolate(vc.worldPos, vd.worldPos, gradient2);

	Color c1 = Color::Interpolate(va.color, vb.color, gradient1);
	Color c2 = Color::Interpolate(vc.color, vd.color, gradient2);

	Vector2 uv1 = Vector2::Interpolate(va.uv, vb.uv, gradient1);
	Vector2 uv2 = Vector2::Interpolate(vc.uv, vd.uv, gradient2);

	Vector4 n1 = Vector4::Interpolate(va.normal, vb.normal, gradient1);
	Vector4 n2 = Vector4::Interpolate(vc.normal, vd.normal, gradient2);

	for (int x = sx; x < ex; ++x)
	{
		float gradient = (x - sx) / (float)(ex - sx);
		float z = Math::Interpolate(z1, z2, gradient);
		v2f.screenPos = Vector4((float)x, (float)y, z);
		v2f.worldPos = Vector4::Interpolate(w1, w2, gradient);
		v2f.color = Color::Interpolate(c1, c2, gradient);		
		v2f.uv = Vector2::Interpolate(uv1, uv2, gradient);
		v2f.normal = Vector4::Interpolate(n1, n2, gradient);

		float iz = Math::Interpolate(iz1, iz2, gradient);
		float vz = 1.0f / iz;
		v2f.EndPerspectiveCorrectInterpolation(vz);

		Color finalColor = shader->FragmentShader(v2f);
		finalColor.Clamp();

		if(finalColor.isValid) //用于Alpha Test
			DrawPixel(x, y, z, finalColor);
	}
}

void Drawing::DrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, Shader *shader)
{
	Vector4 p0 = v0.screenPos;
	Vector4 p1 = v1.screenPos;
	Vector4 p2 = v2.screenPos;

	//排序，以便从上到下依次为p1, p2, p3
	if (p0.y > p1.y)
	{
		Tools::Swap<Vector4>(p0, p1);
		Tools::Swap<VertexOut>(v0, v1);
	}
	if (p1.y > p2.y)
	{
		Tools::Swap<Vector4>(p1, p2);
		Tools::Swap<VertexOut>(v1, v2);
	}
	if (p0.y > p1.y)
	{
		Tools::Swap<Vector4>(p0, p1);
		Tools::Swap<VertexOut>(v0, v1);
	}

	float dP1P2, dP1P3;

	if (p1.y - p0.y > 0) //实际上是保证不相等
		dP1P2 = (p1.x - p0.x) / (p1.y - p0.y);
	else
		dP1P2 = (p1.x == p0.x) ? 0 : FLT_MAX;

	if (p2.y - p0.y > 0)
		dP1P3 = (p2.x - p0.x) / (p2.y - p0.y);
	else
		dP1P3 = (p2.x == p0.x ) ? 0 : FLT_MAX;

	if (dP1P2 > dP1P3) //p2在右
	{
		for (int y = (int)p0.y; y <= (int)p2.y; ++y)
		{
			if (y < p1.y)
				ProcessScanLine(y, v0, v2, v0, v1, shader);
			else
				ProcessScanLine(y, v0, v2, v1, v2, shader);
		}
	}
	else
	{
		for (int y = (int)p0.y; y <= (int)p2.y; ++y)
		{
			if (y < p1.y)
				ProcessScanLine(y, v0, v1, v0, v2, shader);
			else
				ProcessScanLine(y, v1, v2, v0, v2, shader);
		}
	}
}

void Drawing::DrawTriangleWire(VertexOut v0, VertexOut v1, VertexOut v2)
{
	LineDrawing::SimpleDrawLine(v0.screenPos.x, v0.screenPos.y, v1.screenPos.x, v1.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::SimpleDrawLine(v0.screenPos.x, v0.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::SimpleDrawLine(v1.screenPos.x, v1.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
}

void Drawing::Clear(Color color, float z)
{
	mColorBuffer->Clear(Color::black);
	mZBuffer->Clear(z);	
}

void Drawing::Render()
{
	for (int x = 0; x < mColorBuffer->GetWidth(); ++x)
	{
		for (int y = 0; y < mColorBuffer->GetHeight(); ++y)
		{
			Color color = mColorBuffer->Get(x, y);
			NativeDrawPixel(x, y, color.r, color.g, color.b);
		}
	}
}

void Drawing::Debug(int x, int y)
{
	Color color = mColorBuffer->Get(x, y);
	float z = mZBuffer->Get(x, y);
	cout << "Color: " << color << " , z: " << z << endl;
}