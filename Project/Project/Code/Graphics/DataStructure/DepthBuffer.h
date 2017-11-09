#pragma once

#include "BufferBase.h"

class DepthBuffer : public BufferBase<float>
{
public:
	DepthBuffer() : BufferBase<float>() {}
	DepthBuffer(int width, int height) : BufferBase<float>(width, height) {}

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