#pragma once

#include <iostream>
using namespace std;

template <typename T>
class BufferBase
{
public:
	BufferBase() : mData(nullptr), mWidth(0), mHeight(0), mSize(0) {}
	BufferBase(int width, int height) : mWidth(width), mHeight(height) { mSize = width * height; mData = new T[mSize]; }
	~BufferBase() { SAFE_DELETE_ARRAY(mData); }

	int GetSize() { return mSize; }
	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

	void Clear(T v);

	const T &operator[] (int i) const { return mData[i]; }
	T &operator[] (int i) { return mData[i]; }

	virtual void Set(int x, int y, T v);
protected:
	T *mData;
	int mWidth;
	int mHeight;
	int mSize;
};

//*****************************************************************************
template <typename T>
void BufferBase<T>::Clear(T v)
{
	for (int i = 0; i < mSize; ++i)
	{
		mData[i] = v;
	}
}

//*****************************************************************************
template <typename T>
void BufferBase<T>::Set(int x, int y, T v)
{
	if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
	{
		int index = x + y * mWidth;
		mData[index] = v;
	}
}

