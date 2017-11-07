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
	mStencilBuffer = new Buffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Drawing::~Drawing()
{
	if (mZBuffer)
	{
		delete mZBuffer;
		mZBuffer = nullptr;
	}
	if (mStencilBuffer)
	{
		delete mStencilBuffer;
		mStencilBuffer = nullptr;
	}
	if (mColorBuffer)
	{
		delete mColorBuffer;
		mColorBuffer = nullptr;
	}
}

void Drawing::DrawPixel(int x, int y, float z, const Color &color)
{
	if (!IsDepthTestPass(x, y, z))
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

//TODO: 插值是性能爆炸点！！！一个Vector4差不多要丢10帧
//从左向右画线：papb->pcpd
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
	if (sx > SCREEN_WIDTH)
		return;

	int ex = (int)Math::Interpolate(pc.x, pd.x, gradient2);
	if (ex < 0)
		return;

	float inverseDx = 1.0f / (float)(ex - sx);

	//水平裁剪	
	int startX = Math::Max(sx, 0);
	int endX = Math::Min(ex, SCREEN_WIDTH);

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

	for (int x = startX; x < endX; ++x)
	{
		float gradient = (x - sx) * inverseDx;
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

		if (finalColor.isValid) //用于Alpha Test
			DrawPixel(x, y, z, finalColor);
	}
}

void Drawing::DrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, Shader *shader)
{
	Vector4 p0 = v0.screenPos;
	Vector4 p1 = v1.screenPos;
	Vector4 p2 = v2.screenPos;

	//检测是否退化为直线
	if ((Math::Approximate(p0.x, p1.x) && Math::Approximate(p1.x, p2.x)) ||
		(Math::Approximate(p0.y, p1.y) && Math::Approximate(p1.y, p2.y)))
		return;

	//排序，以便从上到下依次为p0, p1, p2
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

	//裁剪测试
	

	float dP1P2, dP1P3;

	if (p1.y - p0.y > 0) //实际上是保证不相等
		dP1P2 = (p1.x - p0.x) / (p1.y - p0.y);
	else
		dP1P2 = (p1.x == p0.x) ? 0 : FLT_MAX;

	if (p2.y - p0.y > 0)
		dP1P3 = (p2.x - p0.x) / (p2.y - p0.y);
	else
		dP1P3 = (p2.x == p0.x ) ? 0 : FLT_MAX;

	int yStart = Math::Max(p0.y, 0);
	int yEnd = Math::Min(p2.y, SCREEN_HEIGHT);

	if (dP1P2 > dP1P3) //p2在右
	{
		for (int y = yStart; y <= yEnd; ++y)
		{
			if (y < p1.y)
				ProcessScanLine(y, v0, v2, v0, v1, shader);
			else
				ProcessScanLine(y, v0, v2, v1, v2, shader);
		}
	}
	else
	{
		for (int y = yStart; y <= yEnd; ++y)
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
	LineDrawing::BresenhamDrawLine(v0.screenPos.x, v0.screenPos.y, v1.screenPos.x, v1.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::BresenhamDrawLine(v0.screenPos.x, v0.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::BresenhamDrawLine(v1.screenPos.x, v1.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
}

void Drawing::DrawTriangleTest(int x0, int y0, int x1, int y1, int x2, int y2)
{
	TriangleDrawing::T3DDrawTriangle(x0, y0, x1, y1, x2, y2, DRAW_PIXEL_FUNC);
}

void Drawing::Clear(Color color, float z, float s)
{
	mColorBuffer->Clear(Color::black);
	mZBuffer->Clear(z);	
	mStencilBuffer->Clear(s);
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

bool Drawing::IsDepthTestPass(int x, int y, float z)
{
	switch (RenderManager::Instance()->GetDepthFunc())
	{
	case GL_ALWAYS:
		return true;
	case GL_LESS:
	{
		if (z < mZBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	case GL_LEQUAL:
	{
		if (z <= mZBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	case GL_EQUAL:
	{
		if (Math::Approximate(mZBuffer->Get(x, y), z))
			return true;
		else
			return false;
	}
	case GL_NOTEQUAL:
	{
		if (!Math::Approximate(mZBuffer->Get(x, y), z))
			return true;
		else
			return false;
	}
	case GL_GREATER:
	{
		if (z > mZBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	case GL_GEQUAL:
	{
		if (z >= mZBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	default:
		return false;
	}

	return false;
}

bool Drawing::IsStencilTestPass(int x, int y, float s)
{
	//if(mStencilBuffer->Get(x, y) < s)
	return false;
}