#include "Graphics\Buffer.h"

Buffer::Buffer()
{
	mWidth = mHeight = mSize = 0;
	mDatas = nullptr;
}

Buffer::Buffer(uint width, uint height)
{
	mWidth = width;
	mHeight = height;
	mSize = width * height;
	mDatas = new float[mSize];
	Clear(0);
}

void Buffer::Clear(float v)
{
	for (int i = 0; i < mSize; ++i)
	{
		mDatas[i] = v;
	}
}

void Buffer::Set(uint x, uint y, float v)
{
	int index = x + y * mWidth;
	mDatas[index] = v;
}