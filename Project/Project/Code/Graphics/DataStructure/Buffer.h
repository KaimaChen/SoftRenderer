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