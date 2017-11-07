#pragma once

#include "Misc\Settings.h"
#include "Misc\Tools.h"
#include "Misc\Debug.h"
#include "Graphics\DataStructure\Buffer.h"
#include "Graphics\DataStructure\ColorBuffer.h"
#include "Graphics\DataStructure\Color.h"
#include "Graphics\DataStructure\Vertex.h"
#include "Graphics\Mesh.h"
#include "Graphics\Shader.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Helper\TriangleDrawing.h"
#include "Math\Vector2.h"
#include "Math\Vector4.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include <cmath>

#define DRAW_PIXEL_FUNC std::bind(&Drawing::DrawPixel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

class Drawing
{
public:
	void (*NativeDrawPixel)(int x, int y, float r, float g, float b) = nullptr;
	static Drawing *Instance();

	void DrawPixel(int x, int y, float z, const Color &color);	
	void DrawTriangleWire(VertexOut v0, VertexOut v1, VertexOut v2);
	void DrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, Shader *shader);
	void DrawTriangleTest(int x0, int y0, int x1, int y1, int x2, int y2);
	void Clear(Color color, float z, float s);
	void Render();
	void Debug(int x, int y);

	bool IsDepthTestPass(int x, int y, float z);
	bool IsStencilTestPass(int x, int y, float s);
private:
	Drawing();
	~Drawing();
	void ProcessScanLine(int y, VertexOut va, VertexOut vb, VertexOut vc, VertexOut vd, Shader *shader);
private:
	static Drawing *mInstance;
	ColorBuffer *mColorBuffer;
	Buffer *mZBuffer;
	Buffer *mStencilBuffer;
};