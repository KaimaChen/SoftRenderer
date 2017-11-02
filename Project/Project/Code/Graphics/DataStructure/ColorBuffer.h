#pragma once

#include "Color.h"
#include "BufferBase.h"

class ColorBuffer : public BufferBase<Color>
{
public:
	ColorBuffer() : BufferBase<Color>() {}
	ColorBuffer(int width, int height) : BufferBase<Color>() {}

	virtual void Set(int x, int y, Color v)
	{
		int index = (x + y * mWidth) * 4;
		mData[index] = v;
	}
};