#include "GX\Buffer.h"

Buffer::Buffer()
{
	mWidth = mHeight = mSize = 0;
	mType = BufferType::Value;
	mDatas = nullptr;
}

Buffer::Buffer(uint width, uint height, BufferType type)
{
	mWidth = width;
	mHeight = height;
	mType = type;
	uint num = (type == BufferType::RGBA) ? 4 : 1;
	mSize = width * height * num;
	mDatas = new byte[mSize];
	Clear(0);
}

void Buffer::Clear(byte v)
{
	for (int i = 0; i < mSize; ++i)
	{
		mDatas[i] = v;
	}
}

void Buffer::Clear(byte r, byte g, byte b, byte a)
{
	if (mType != BufferType::RGBA)
	{
		Debug::LogError("Clear(r,g,b,a) only works on BufferType::RGBA !!");
		return;
	}

	for (int i = 0; i < mSize; i += 4)
	{
		mDatas[i] = r;
		mDatas[i + 1] = g;
		mDatas[i + 2] = b; 
		mDatas[i + 3] = a;
	}
}

void Buffer::Set(uint x, uint y, byte v)
{
	int num = (mType == BufferType::RGBA) ? 4 : 1;
	int index = (x + y * mWidth) * num;
	mDatas[index] = v;
}

void Buffer::Set(uint x, uint y, byte r, byte g, byte b, byte a)
{
	if (mType != BufferType::RGBA)
	{
		Debug::LogError("Set(x,y,r,g,b,a) only works on BufferType::RGBA !!");
		return;
	}

	int index = (x + y * mWidth) * 4;
	mDatas[index] = r;
	mDatas[index + 1] = g;
	mDatas[index + 2] = b;
	mDatas[index + 3] = a;
}