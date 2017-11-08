#pragma once

#include "BufferBase.h"

class StencilBuffer : public BufferBase<int>
{
public:
	StencilBuffer() : BufferBase<int>() {}
	StencilBuffer(int width, int height) : BufferBase<int>(width, height) {}

	int Get(int x, int y)
	{
		if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
		{
			int index = x + y * mWidth;
			return mData[index];
		}
		else
		{
			return -1;
		}
	}
};