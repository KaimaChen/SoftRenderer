#pragma once

#include "Misc\Settings.h"
#include "Misc\Tools.h"
#include "Graphics\DataStructure\Buffer.h"
#include "Graphics\DataStructure\ColorBuffer.h"
#include "Graphics\DataStructure\Color.h"
#include "Graphics\DataStructure\Vertex.h"
#include "Graphics\Mesh.h"
#include "Graphics\Shader.h"
#include "Math\Vector2.h"
#include "Math\Vector4.h"
#include "Math\Matrix4x4.h"
#include "Math\Math.h"
#include <cmath>

class Drawing
{
public:
	void (*NativeDrawPixel)(int x, int y, float r, float g, float b) = nullptr;
	static Drawing *Instance();

	void DrawPixel(int x, int y, float z, const Color &color);
	void DrawTriangle(VertexOut v0, VertexOut v1, VertexOut v2, Shader *shader);
	void Clear(Color color, float z);
	void Render();
private:
	Drawing();
	~Drawing();
	void ProcessScanLine(int y, VertexOut va, VertexOut vb, VertexOut vc, VertexOut vd, Shader *shader);
private:
	static Drawing *mInstance;
	Buffer *mZBuffer;
	ColorBuffer *mColorBuffer;
};