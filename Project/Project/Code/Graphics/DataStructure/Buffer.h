#pragma once

#include "BufferBase.h"

class Buffer : public BufferBase<float>
{
public:
	Buffer() : BufferBase<float>() {}
	Buffer(int width, int height) : BufferBase<float>(width, height) {}

	float Get(int x, int y)
	{
		if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
		{
			int index = x + y * mWidth;
			return mData[index];
		}
		else
		{
			return 1.0f;
		}
	}
};

//class Buffer
//{
//public:
//	Buffer() : mData(nullptr), mWidth(0), mHeight(0), mSize(0) {}
//	Buffer(int width, int height) : mWidth(width), mHeight(height) { mSize = width * height; mData = new float[mSize]; }
//	~Buffer() { delete[] mData; mData = nullptr; }
//
//	int GetSize() { return mSize; }
//	int GetWidth() { return mWidth; }
//	int GetHeight() { return mHeight; }
//
//	void Clear(float v);
//	void Set(int x, int y, float v);
//	float Get(int x, int y);
//
//	const float &operator[] (int i) const { return mData[i]; }
//	float &operator[] (int i) { return mData[i]; }
//
//private:
//	float *mData;
//	int mWidth;
//	int mHeight;
//	int mSize;
//};