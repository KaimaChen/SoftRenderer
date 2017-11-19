#pragma once

#include <cmath>

#include "Misc\Settings.h"
#include "Misc\Tools.h"
#include "Misc\Defines.h"
#include "Graphics\DataStructure\DepthBuffer.h"
#include "Graphics\DataStructure\StencilBuffer.h"
#include "Graphics\DataStructure\ColorBuffer.h"
#include "Graphics\DataStructure\Color.h"
#include "Graphics\Shaders\Vertex.h"
#include "Graphics\Shaders\ShaderProgram.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Helper\TriangleDrawing.h"
#include "Math\Vector2.h"
#include "Math\Vector4.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"

#define DRAW_PIXEL_FUNC std::bind(&Drawing::DrawPixel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

class Drawing
{
public:
	void (*NativeDrawPixel)(int x, int y, float r, float g, float b) = nullptr;
	static Drawing *Instance();

	void DrawPixel(int x, int y, float z, const Color &color);	
	void DrawTriangleWire(VertexOut v0, VertexOut v1, VertexOut v2);
	void DrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram);
	void DrawTriangleTest(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram);
	void DrawTriangleTest2(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram);
	void ClearColorBuffer(const Color &color);
	void ClearDepthBuffer(float z);
	void ClearStencilBuffer(int s);
	void Render();
	void Debug(int x, int y);
private:
	Drawing();
	~Drawing();
	void ProcessScanLine(int y, VertexOut va, VertexOut vb, VertexOut vc, VertexOut vd, ShaderProgram *shaderProgram);
	void DrawBottomTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram);
	void DrawTopTriangle(VertexOut v0, VertexOut v1, VertexOut v2, ShaderProgram *shaderProgram);
	void Sort(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2, VertexOut &v0, VertexOut &v1, VertexOut &v2);
	VertexOut Interpolate(float x, float y, const VertexOut &v0, const VertexOut &v1, const VertexOut &v2, float w0, float w1, float w2);

	bool IsDepthTestPass(int x, int y, float z);
	bool IsStencilTestPass(int x, int y);
	void UpdateStencil(int x, int y, bool stencilTest, bool depthTest);
	void WriteStencil(int x, int y, GLenum op);
	Color Blend(const Color &srcColor, const Color &dstColor);
	bool AssignBlendFunc(GLenum blendFunc, const Color &srcColor, const Color &dstColor, int count, float *result);
	void AssignFrameBuffer(int x, int y, const Color &finalColor);
	bool EarlyZTest(int x, int y, float z, ShaderProgram *shader);
	bool ScissorTest(int x, int y);
private:
	static Drawing *mInstance;
	ColorBuffer *mColorBuffer;
	StencilBuffer *mStencilBuffer;
	DepthBuffer *mDepthBuffer;
};