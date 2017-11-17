#include "Drawing.h"
#include "Managers\Context.h"

Drawing *Drawing::mInstance = nullptr;

//*****************************************************************************
Drawing *Drawing::Instance()
{
	if (mInstance == nullptr)
		mInstance = new Drawing();
	return mInstance;
}

//*****************************************************************************
Drawing::Drawing()
{
	mColorBuffer = new ColorBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	mDepthBuffer = new DepthBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	mStencilBuffer = new StencilBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}

//*****************************************************************************
Drawing::~Drawing()
{
	SAFE_DELETE(mDepthBuffer);
	SAFE_DELETE(mStencilBuffer);
	SAFE_DELETE(mColorBuffer);
}

//*****************************************************************************
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
	Color dstColor = mColorBuffer->Get(x, y);
	if (Context::Instance()->glIsEnabled(GL_BLEND)) 
		finalColor = Blend(color, dstColor);

	//填充缓冲区
	AssignFrameBuffer(x, y, finalColor);
}

//*****************************************************************************
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

	//int sx = (int)(Math::Interpolate(pa.x, pb.x, gradient1) + 0.5f);
	int sx = (int)Math::Interpolate(pa.x, pb.x, gradient1);
	if (sx > SCREEN_WIDTH)
		return;

	//int ex = (int)(Math::Interpolate(pc.x, pd.x, gradient2) + 0.5f);
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

		if (!EarlyZTest(x, y, z, shaderProgram))
			continue;

		if (!ScissorTest(x, y)) //TODO: 其实这个裁剪测试可以放到更早的地方以便提高性能
			continue;

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

//*****************************************************************************
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

	int yStart = (int)Math::Max(p0.y, 0);
	int yEnd = (int)Math::Min(p2.y, (float)SCREEN_HEIGHT);

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

//*****************************************************************************
void Drawing::DrawTriangleTest(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram)
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
		int startX = (int)Math::Min(Math::Min(x0, x1), (float)x2);
		int endX = (int)Math::Max(Math::Max(x0, x1), (float)x2);
		DrawHorizontalLine(startX, endX, y0);
		return;
	}

	Sort(x0, y0, x1, y1, x2, y2, v0, v1, v2);

	if (y0 == y1)
		//DrawBottomTriangle(x2, y2, x0, y0, x1, y1);
		DrawBottomTriangle(v2, v0, v1, shaderProgram);
	else if (y1 == y2)
		DrawTopTriangle(v0, v1, v2, shaderProgram);
		//DrawTopTriangle(x0, y0, x1, y1, x2, y2);
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
		
		//DrawBottomTriangle(x2, y2, x1, y1, newX, y1);
		//DrawTopTriangle(x0, y0, newX, y1, x1, y1);
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
void Drawing::DrawBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	if (y0 == y1 || y0 == y2)
		return;

	float kx0 = (float)(x0 - x1) / (y0 - y1);
	float kx1 = (float)(x0 - x2) / (y0 - y2);
	
	for (int y = y1; y <= y0; ++y)
	{
		float startX = x1 + kx0 * (y - y1);
		float endX = x2 + kx1 * (y - y1);
		DrawHorizontalLine(startX, endX, y);
	}
}

//*****************************************************************************
//绘制平底三角形
//    p0
//   /   \
// /       \
//p1-----p2
void Drawing::DrawBottomTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram)
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
				DrawPixel(x, y, v2f.screenPos.z, finalColor);
				//mColorBuffer->Set(x, y, Color::white);
		}
	}
}

//*****************************************************************************
//绘制平顶三角形
//p2-------p1
//  \           /
//    \       /
//       p0
void Drawing::DrawTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	if (y0 == y1 || y0 == y2)
		return;

	float kx0 = (float)(x1 - x0) / (y1 - y0);
	float kx1 = (float)(x2 - x0) / (y2 - y0);

	for (int y = y0; y <= y1; ++y)
	{
		float startX = x0 + kx1 * (y - y0);
		float endX = x0 + kx0 * (y - y0);
		if (startX > endX)
			Tools::Swap<float>(startX, endX);
		DrawHorizontalLine(startX, endX, y);
	}
}

void Drawing::DrawTopTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram)
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
		//DrawHorizontalLine(startX, endX, y);

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
				DrawPixel(x, y, v2f.screenPos.z, finalColor);
			//	mColorBuffer->Set(x, y, Color::white);
		}
	}
}

//*****************************************************************************
void Drawing::DrawHorizontalLine(int sx, int ex, int y)
{
	for (int x = sx; x <= ex; ++x)
	{
		mColorBuffer->Set(x, y, Color::white);
	}
}

//*****************************************************************************
//先按y排序，y相同的再按x排序
void Drawing::Sort(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2, VertexOut &v0, VertexOut &v1, VertexOut &v2)
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
VertexOut Drawing::Interpolate(float x, float y, const VertexOut &v0, const VertexOut &v1, const VertexOut &v2, float w0, float w1, float w2)
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

//*****************************************************************************
void Drawing::DrawTriangleWire(VertexOut v0, VertexOut v1, VertexOut v2)
{
	LineDrawing::BresenhamDrawLine(v0.screenPos.x, v0.screenPos.y, v1.screenPos.x, v1.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::BresenhamDrawLine(v0.screenPos.x, v0.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
	LineDrawing::BresenhamDrawLine(v1.screenPos.x, v1.screenPos.y, v2.screenPos.x, v2.screenPos.y, DRAW_PIXEL_FUNC);
}

//*****************************************************************************
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

//*****************************************************************************
void Drawing::Debug(int x, int y)
{
	Color color = mColorBuffer->Get(x, y);
	float z = mDepthBuffer->Get(x, y);
	int s = mStencilBuffer->Get(x, y);
	cout << "(" << x << "," << y << ") = " << "Color: " << color << " , z: " << z << " , s: " << s << endl;
}

//*****************************************************************************
bool Drawing::IsDepthTestPass(int x, int y, float z)
{
	int depthFunc;
	Context::Instance()->glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
	switch (depthFunc)
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

//*****************************************************************************
bool Drawing::IsStencilTestPass(int x, int y)
{
	int ref, valueMask, stencilFunc;
	if (Context::Instance()->IsCurrentFaceFront())
	{
		Context::Instance()->glGetIntegerv(GL_STENCIL_REF, &ref);
		Context::Instance()->glGetIntegerv(GL_STENCIL_VALUE_MASK, &valueMask);
		Context::Instance()->glGetIntegerv(GL_STENCIL_FUNC, &stencilFunc);
	}
	else
	{
		Context::Instance()->glGetIntegerv(GL_STENCIL_BACK_REF, &ref);
		Context::Instance()->glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &valueMask);
		Context::Instance()->glGetIntegerv(GL_STENCIL_BACK_FUNC, &stencilFunc);
	}
	
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

//*****************************************************************************
void Drawing::UpdateStencil(int x, int y, bool stencilTest, bool depthTest)
{
	bool isFront = Context::Instance()->IsCurrentFaceFront();

	if (!stencilTest)
	{
		int sfail;
		if (isFront)
			Context::Instance()->glGetIntegerv(GL_STENCIL_FAIL, &sfail);
		else
			Context::Instance()->glGetIntegerv(GL_STENCIL_BACK_FAIL, &sfail);

		WriteStencil(x, y, sfail);
	}
	else if (stencilTest && !depthTest)
	{
		int dpfail;
		if(isFront)
			Context::Instance()->glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &dpfail);
		else
			Context::Instance()->glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &dpfail);
		
		WriteStencil(x, y, dpfail);
	}
	else if (stencilTest && depthTest)
	{
		int dppass;
		if(isFront)
			Context::Instance()->glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &dppass);
		else
			Context::Instance()->glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &dppass);
		
		WriteStencil(x, y, dppass);
	}
}

//*****************************************************************************
void Drawing::WriteStencil(int x, int y, GLenum op)
{
	int value = mStencilBuffer->Get(x, y);
	int ref, stencilBits;

	if (Context::Instance()->IsCurrentFaceFront())
		Context::Instance()->glGetIntegerv(GL_STENCIL_REF, &ref);
	else
		Context::Instance()->glGetIntegerv(GL_STENCIL_BACK_REF, &ref);

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
		mStencilBuffer->Set(x, y, ref);
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

//*****************************************************************************
Color Drawing::Blend(const Color &srcColor, const Color &dstColor)
{
	int srcRGBBlendFunc, srcAlphaBlendFunc, dstRGBBlendFunc, dstAlphaBlendFunc, rgbBlendEquation, alphaBlendEquation;

	Context::Instance()->glGetIntegerv(GL_BLEND_SRC_RGB, &srcRGBBlendFunc);
	Context::Instance()->glGetIntegerv(GL_BLEND_SRC_ALPHA, &srcAlphaBlendFunc);
	Context::Instance()->glGetIntegerv(GL_BLEND_DST_RGB, &dstRGBBlendFunc);
	Context::Instance()->glGetIntegerv(GL_BLEND_DST_ALPHA, &dstAlphaBlendFunc);
	Context::Instance()->glGetIntegerv(GL_BLEND_EQUATION_RGB, &rgbBlendEquation);
	Context::Instance()->glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &alphaBlendEquation);

	float srcRGB[3], srcAlpha, dstRGB[3], dstAlpha;

	if (!AssignBlendFunc(srcRGBBlendFunc, srcColor, dstColor, 3, srcRGB))
		return Color::black;
	if (!AssignBlendFunc(srcAlphaBlendFunc, srcColor, dstColor, 1, &srcAlpha))
		return Color::black;
	if (!AssignBlendFunc(dstRGBBlendFunc, srcColor, dstColor, 3, dstRGB))
		return Color::black;
	if (!AssignBlendFunc(dstAlphaBlendFunc, srcColor, dstColor, 1, &dstAlpha))
		return Color::black;

	Color result;
	switch (rgbBlendEquation)
	{
	case GL_FUNC_ADD:
		result.r = srcColor.r * srcRGB[0] + dstColor.r * dstRGB[0];
		result.g = srcColor.g * srcRGB[1] + dstColor.g * dstRGB[1];
		result.b = srcColor.b * srcRGB[2] + dstColor.b * dstRGB[2];
		break;
	case GL_FUNC_SUBTRACT:
		result.r = srcColor.r * srcRGB[0] - dstColor.r * dstRGB[0];
		result.g = srcColor.g * srcRGB[1] - dstColor.g * dstRGB[1];
		result.b = srcColor.b * srcRGB[2] - dstColor.b * dstRGB[2];
		break;
	case GL_FUNC_REVERSE_SUBTRACT:
		result.r = dstColor.r * dstRGB[0] - srcColor.r * srcRGB[0];
		result.g = dstColor.g * dstRGB[1] - srcColor.g * srcRGB[1];
		result.b = dstColor.b * dstRGB[2] - srcColor.b * srcRGB[2];
		break;
	case GL_MIN:
		result.r = Math::Min(srcColor.r, dstColor.r);
		result.g = Math::Min(srcColor.g, dstColor.g);
		result.b = Math::Min(srcColor.b, dstColor.b);
		break;
	case GL_MAX:
		result.r = Math::Max(srcColor.r, dstColor.r);
		result.g = Math::Max(srcColor.g, dstColor.g);
		result.b = Math::Max(srcColor.b, dstColor.b);
		break;
	default:
		cout << "RGB Blend Equation Error" << endl;
		return Color::black;
		break;
	}
	switch (alphaBlendEquation)
	{
	case GL_FUNC_ADD:
		result.a = srcColor.a * srcAlpha + dstColor.a * dstAlpha;
		break;
	case GL_FUNC_SUBTRACT:
		result.a = srcColor.a * srcAlpha - dstColor.a * dstAlpha;
		break;
	case GL_FUNC_REVERSE_SUBTRACT:
		result.a = dstColor.a * dstAlpha - srcColor.a * srcAlpha;
		break;
	case GL_MIN:
		result.a = Math::Min(srcColor.a, dstColor.a);
		break;
	case GL_MAX:
		result.a = Math::Max(srcColor.a, dstColor.a);
		break;
	default:
		cout << "Alpha Blend Euqation Error" << endl;
		return Color::black;
		break;
	}

	return result;
}

//*****************************************************************************
bool Drawing::AssignBlendFunc(GLenum blendFunc, const Color &srcColor, const Color &dstColor, int count, float *result)
{
	float color[4];
	Context::Instance()->glGetFloatv(GL_BLEND_COLOR, color);
	Color blendColor = Color(color[0], color[1], color[2], color[3]);

	auto assignFloat = [&](float data) {
		for (int i = 0; i < count; ++i)
		{
			result[i] = data;
		}
	};

	auto assignColor = [&](const Color &color) {
		for (int i = 0; i < count; ++i)
		{
			result[i] = color[i];
		}
	};

	switch (blendFunc)
	{
	case GL_ZERO:
		assignFloat(0);
		break;
	case GL_ONE:
		assignFloat(1);
		break;
	case GL_SRC_COLOR:
		assignColor(srcColor);
		break;
	case GL_ONE_MINUS_SRC_COLOR:
		assignColor(1 - srcColor);
		break;
	case GL_DST_COLOR:
		assignColor(dstColor);
		break;
	case GL_ONE_MINUS_DST_COLOR:
		assignColor(1 - dstColor);
		break;
	case GL_SRC_ALPHA:
		assignFloat(srcColor.a);
		break;
	case GL_ONE_MINUS_SRC_ALPHA:
		assignFloat(1 - srcColor.a);
		break;
	case GL_DST_ALPHA:
		assignFloat(1 - dstColor.a);
		break;
	case GL_ONE_MINUS_DST_ALPHA:
		assignFloat(1 - dstColor.a);
		break;
	case GL_CONSTANT_COLOR:
		assignColor(blendColor);
		break;
	case GL_ONE_MINUS_CONSTANT_COLOR:
		assignColor(1 - blendColor);
		break;
	case GL_CONSTANT_ALPHA:
		assignFloat(blendColor[3]);
		break;
	case GL_ONE_MINUS_CONSTANT_ALPHA:
		assignFloat(1 - blendColor[3]);
		break;
	default:
		cout << "Blend Func Error" << endl;
		return false;
	}
	
	return true;
}

//*****************************************************************************
void Drawing::AssignFrameBuffer(int x, int y, const Color &finalColor)
{
	GLboolean data[4];
	Context::Instance()->glGetBooleanv(GL_COLOR_WRITEMASK, data);

	Color color = mColorBuffer->Get(x, y);
	if (data[0])
		color.r = finalColor.r;
	if (data[1])
		color.g = finalColor.g;
	if (data[2])
		color.b = finalColor.b;
	if (data[3])
		color.a = finalColor.a;

	mColorBuffer->Set(x, y, color);
}

//*****************************************************************************
void Drawing::ClearColorBuffer(const Color &color)
{
	mColorBuffer->Clear(color);
}

//*****************************************************************************
void Drawing::ClearDepthBuffer(float d)
{
	mDepthBuffer->Clear(d);
}

//*****************************************************************************
void Drawing::ClearStencilBuffer(int s)
{
	mStencilBuffer->Clear(s);
}

//*****************************************************************************
bool Drawing::EarlyZTest(int x, int y, float z, ShaderProgram *shader)
{
	if (shader->GetMighChangeZ())
		return true;

	if (!Context::Instance()->glIsEnabled(GL_DEPTH_TEST))
		return true;

	if (IsDepthTestPass(x, y, z))
		return true;

	return false;
}

//*****************************************************************************
bool Drawing::ScissorTest(int x, int y)
{
	if (!Context::Instance()->glIsEnabled(GL_SCISSOR_TEST))
		return true;

	int scissorBox[4];
	Context::Instance()->glGetIntegerv(GL_SCISSOR_BOX, scissorBox);

	if (x < scissorBox[0] || x >(scissorBox[0] + scissorBox[2]) || y < scissorBox[1] || y >(scissorBox[1] + scissorBox[3]))
		return false;

	return true;
}