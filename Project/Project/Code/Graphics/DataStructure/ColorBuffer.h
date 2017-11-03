#pragma once

#include "Color.h"
#include "BufferBase.h"

class ColorBuffer : public BufferBase<Color>
{
public:
	ColorBuffer() : BufferBase<Color>() {}
	ColorBuffer(int width, int height) : BufferBase<Color>(width, height) {}

	Color Get(int x, int y)
	{
		if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
		{
			int index = x + y * mWidth;
			return mData[index];
		}
		else
		{
			return Color::black;
		}
	}
};