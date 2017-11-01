#pragma once

#include "Misc\Defines.h"
#include "Misc\Debug.h"

class Buffer
{
public:
	Buffer();
	Buffer(uint width, uint height);

	void Clear(float v);
	void Set(uint x, uint y, float v);

	uint GetSize() { return mSize; }
	uint GetWidth() { return mWidth; }
	uint GetHeight() { return mHeight; }

	const float &operator[](uint i) const { return mDatas[i]; }
	float &operator[](uint i) { return mDatas[i]; }
private:
	float *mDatas;
	uint mWidth;
	uint mHeight;
	uint mSize;
};