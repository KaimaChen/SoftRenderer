#pragma once

#include "Graphics\DataStructure\Color.h"
#include "Misc\Defines.h"
#include <string>

class Texture
{
public:
	Texture() {}
	Texture(ubyte *data, int width, int height, int channelNum) 
		: mWidth(width), mHeight(height), mChannelNum(channelNum) 
	{		
		int size = width * height * channelNum;
		mData = new ubyte[size];
		memcpy_s(mData, size, data, size);
	}

	~Texture() { SAFE_DELETE(mData); }

	void SetMinFilter(GLenum filter) { mMinFilter = filter; }
	void SetMagFilter(GLenum filter) { mMagFilter = filter; }
	void SetWrapS(GLenum wrap) { mWrapS = wrap; }
	void SetWrapT(GLenum wrap) { mWrapT = wrap; }
	void SetWrapR(GLenum wrap) { mWrapR = wrap; }
	void SetBorderColor(Color color) { mBorderColor = color; }

	GLenum GetMinFilter() { return mMinFilter; }
	GLenum GetMagFilter() { return mMagFilter; }
	GLenum GetWrapS() { return mWrapS; }
	GLenum GetWrapT() { return mWrapT; }
	GLenum GetWrapR() { return mWrapR; }
	Color GetBorderColor() { return mBorderColor; }

protected:
	ubyte *mData = nullptr;
	int mWidth = 0;
	int mHeight = 0;
	int mChannelNum = 4;

	GLenum mMinFilter = GL_NEAREST;
	GLenum mMagFilter = GL_NEAREST;
	GLenum mWrapS = GL_REPEAT;
	GLenum mWrapT = GL_REPEAT;
	GLenum mWrapR = GL_REPEAT;
	Color mBorderColor = Color::error;
};