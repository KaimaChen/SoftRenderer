#pragma once

#include "Misc\Defines.h"
#include "Misc\Debug.h"

enum BufferType
{
	Value,
	RGBA,
};

class Buffer
{
public:
	Buffer();
	Buffer(uint width, uint height, BufferType type);
	void Clear(byte v);
	void Clear(byte r, byte g, byte b, byte a);
	void Set(uint x, uint y, byte v);
	void Set(uint x, uint y, byte r, byte g, byte b, byte a);
	uint GetSize() { return mSize; }
	uint GetWidth() { return mWidth; }
	uint GetHeight() { return mHeight; }

	const byte &operator[](uint i) const { return mDatas[i]; }
	byte &operator[](uint i) { return mDatas[i]; }
private:
	byte *mDatas;
	uint mWidth;
	uint mHeight;
	uint mSize;
	BufferType mType;
};