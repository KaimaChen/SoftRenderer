#include "Drawing.h"
#include "Managers\Context.h"

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
	mDepthBuffer = new DepthBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	mStencilBuffer = new StencilBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Drawing::~Drawing()
{
	if (mDepthBuffer)
	{
		delete mDepthBuffer;
		mDepthBuffer = nullptr;
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
	bool isDepthTestEnabled = Context::Instance()->glIsEnabled(GL_DEPTH_TEST);
	bool depthTest = !isDepthTestEnabled || IsDepthTestPass(x, y, z);

	//模板测试
	if (Context::Instance()->glIsEnabled(GL_STENCIL_TEST))
	{
		bool stencilTest = IsStencilTestPass(x, y);
		UpdateStencil(x, y, stencilTest, depthTest);

		if (!stencilTest)
			return;
	}

	//深度测试
	if (!depthTest)
		return;

	bool writeMask;
	Context::Instance()->glGetBooleanv(GL_DEPTH_WRITEMASK, &writeMask);
	if (writeMask)
		mDepthBuffer->Set(x, y, z);
	
	//透明度混合
	Color finalColor = color;
	if (Context::Instance()->glIsEnabled(GL_BLEND)) 
	{
		Color originColor = mColorBuffer->Get(x, y);
		finalColor = color * color.a + originColor * (1 - color.a);
		finalColor.a = color.a;
	}

	mColorBuffer->Set(x, y, finalColor);
}

//TODO: 插值是性能爆炸点！！！一个Vector4差不多要丢10帧
//从左向右画线：papb->pcpd
void Drawing::ProcessScanLine(int y, VertexOut va, VertexOut vb, VertexOut vc, VertexOut vd, ShaderProgram *shaderProgram)
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
	int startX = (sx < 0) ? 0 : sx;
	int endX = (ex > SCREEN_WIDTH) ? SCREEN_WIDTH : ex;

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
		shaderProgram->SetFragCoord(v2f.screenPos);

		v2f.worldPos = Vector4::Interpolate(w1, w2, gradient);
		v2f.color = Color::Interpolate(c1, c2, gradient);
		v2f.uv = Vector2::Interpolate(uv1, uv2, gradient);
		v2f.normal = Vector4::Interpolate(n1, n2, gradient);

		float iz = Math::Interpolate(iz1, iz2, gradient);
		float vz = 1.0f / iz;
		v2f.EndPerspectiveCorrectInterpolation(vz);

		Color finalColor = shaderProgram->ExecuteFragmentShader(v2f);
		finalColor.Clamp();

		if (finalColor.isValid) //用于Alpha Test
			DrawPixel(x, y, z, finalColor);
	}
}

void Drawing::DrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram)
{
	Vector4 p0 = v0.screenPos;
	Vector4 p1 = v1.screenPos;
	Vector4 p2 = v2.screenPos;

	//检测是否退化为直线
	if ((Math::Approximate(p0.x, p1.x) && Math::Approximate(p1.x, p2.x)) ||
		(Math::Approximate(p0.y, p1.y) && Math::Approximate(p1.y, p2.y)))
		return;

	//排序，以便从下到上依次为p0, p1, p2
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
	if (p0.y < 0 || p2.y > SCREEN_HEIGHT ||
		(p0.x < 0 && p1.x < 0 && p2.x < 0) ||
		(p0.x > SCREEN_WIDTH && p1.x > SCREEN_WIDTH && p2.x > SCREEN_WIDTH))
		return;

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
				ProcessScanLine(y, v0, v2, v0, v1, shaderProgram);
			else
				ProcessScanLine(y, v0, v2, v1, v2, shaderProgram);
		}
	}
	else
	{
		for (int y = yStart; y <= yEnd; ++y)
		{
			if (y < p1.y)
				ProcessScanLine(y, v0, v1, v0, v2, shaderProgram);
			else
				ProcessScanLine(y, v1, v2, v0, v2, shaderProgram);
		}
	}
}

void Drawing::DrawTriangleWire(VertexOut v0, VertexOut v1, VertexOut v2)
{
	LineDrawing::BresenhamDrawLine(v0.screenPos.x, v0.screenPos.y, v1.screenPos.x, v1.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::BresenhamDrawLine(v0.screenPos.x, v0.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::BresenhamDrawLine(v1.screenPos.x, v1.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
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
	float z = mDepthBuffer->Get(x, y);
	int s = mStencilBuffer->Get(x, y);
	cout << "Color: " << color << " , z: " << z << " , s: " << s << endl;
}

bool Drawing::IsDepthTestPass(int x, int y, float z)
{
	switch (Context::Instance()->GetDepthFunc())
	{
	case GL_ALWAYS:
		return true;
	case GL_LESS:
	{
		if (z < mDepthBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	case GL_LEQUAL:
	{
		if (z <= mDepthBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	case GL_EQUAL:
	{
		if (Math::Approximate(mDepthBuffer->Get(x, y), z))
			return true;
		else
			return false;
	}
	case GL_NOTEQUAL:
	{
		if (!Math::Approximate(mDepthBuffer->Get(x, y), z))
			return true;
		else
			return false;
	}
	case GL_GREATER:
	{
		if (z > mDepthBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	case GL_GEQUAL:
	{
		if (z >= mDepthBuffer->Get(x, y))
			return true;
		else
			return false;
	}
	default:
		return false;
	}

	return false;
}

bool Drawing::IsStencilTestPass(int x, int y)
{
	int ref, valueMask;
	Context::Instance()->glGetIntegerv(GL_STENCIL_REF, &ref);
	Context::Instance()->glGetIntegerv(GL_STENCIL_VALUE_MASK, &valueMask);
	GLenum stencilFunc = Context::Instance()->GetStencilFunc();
	int stencilVal = mStencilBuffer->Get(x, y);

	int a = stencilVal & valueMask;
	int b = ref & valueMask;

	switch (stencilFunc)
	{
	case GL_ALWAYS:
		return true;
	case GL_LESS:
		return b < a;
	case GL_LEQUAL:
		return b <= a;
	case GL_EQUAL:
		return b == a;
	case GL_NOTEQUAL:
		return b != a;
	case GL_GREATER:
		return b > a;
	case GL_GEQUAL:
		return b >= a;
	case GL_NEVER:
		return false;
	default:
		return false;
	}

	return false;
}

void Drawing::UpdateStencil(int x, int y, bool stencilTest, bool depthTest)
{
	if (!stencilTest)
	{
		int sfail;
		Context::Instance()->glGetIntegerv(GL_STENCIL_FAIL, &sfail);
		WriteStencil(x, y, sfail);
	}
	else if (stencilTest && !depthTest)
	{
		int dpfail;
		Context::Instance()->glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &dpfail);
		WriteStencil(x, y, dpfail);
	}
	else if (stencilTest && depthTest)
	{
		int dppass;
		Context::Instance()->glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &dppass);
		WriteStencil(x, y, dppass);
	}
}

void Drawing::WriteStencil(int x, int y, GLenum op)
{
	int value = mStencilBuffer->Get(x, y);
	int ref, stencilBits;
	Context::Instance()->glGetIntegerv(GL_STENCIL_REF, &ref);
	Context::Instance()->glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
	int maxValue = pow(2, stencilBits) - 1;
	switch (op)
	{
	case GL_KEEP:
		return;
	case GL_ZERO:
		mStencilBuffer->Set(x, y, 0);
		break;
	case GL_REPLACE:
	{
		mStencilBuffer->Set(x, y, ref);
	}
		break;
	case GL_INCR:
	{
		value++;
		value = (value > maxValue) ? maxValue : value;
		mStencilBuffer->Set(x, y, value);
	}
		break;
	case GL_INCR_WRAP:
	{
		value++;
		value = (value > maxValue) ? 0 : value;
		mStencilBuffer->Set(x, y, value);
	}
		break;
	case GL_DECR:
	{
		value--;
		value = (value < 0) ? 0 : value;
		mStencilBuffer->Set(x, y, value);
	}
		break;
	case GL_DECR_WRAP:
	{
		value--;
		value = (value < 0) ? maxValue : value;
		mStencilBuffer->Set(x, y, value);
	}
		break;
	case GL_INVERT:
		value = maxValue - value;
		mStencilBuffer->Set(x, y, value);
		break;
	default:
		cout << "Stencil Op = " << op << " doesnt exist" << endl;
		break;
	}
}

void Drawing::ClearColorBuffer(const Color &color)
{
	mColorBuffer->Clear(color);
}

void Drawing::ClearDepthBuffer(float d)
{
	mDepthBuffer->Clear(d);
}

void Drawing::ClearStencilBuffer(int s)
{
	mStencilBuffer->Clear(s);
}