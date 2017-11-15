#pragma once

#include "Libs\stb_image.h"
#include "Math\Vector2.h"
#include "Graphics\DataStructure\Color.h"
#include "Misc\Defines.h"

class Texture2D
{
public:
	Texture2D(const char *path);
	Texture2D(ubyte *data, int width, int height, int channelNum) : mData(data), mWidth(width), mHeight(height), mChannelNum(channelNum) {}
	~Texture2D() { SAFE_DELETE(mData); }

	Color Read(const Vector2 &uv) const;
	Texture2D *GenMipMap() const;

	void SetFilter(GLenum filter) { mFilter = filter; }
	void SetWrap(GLenum wrapS, GLenum wrapT) { mWrapS = wrapS; mWrapT = wrapT; }
	void SetBorderColor(Color color) { mBorderColor = color; }

	GLenum GetFilter() { return mFilter; }
	GLenum GetWrapS() { return mWrapS; }
	GLenum GetWrapT() { return mWrapT; }
	Color GetBorderColor() { return mBorderColor; }

private:
	Color NearestRead(const Vector2 &uv) const;
	Color LinearRead(const Vector2 &uv) const;
	int GetIndex(int x, int y) const;
	Color GetColor(int x, int y) const;
	Vector2 GetUV(Vector2 uv) const;
	Texture2D *GenNearestMipMap() const;
	Texture2D *GenBoxFilterMipMap() const;

	static float Repeat(float v);
	static float MirrorRepeat(float v);
	static float ClampToEdge(float v);
	static float ClampToBorder(float v);

private:
	unsigned char *mData = nullptr;
	int mWidth = 0;
	int mHeight = 0;
	int mChannelNum = 4;
	GLenum mFilter = GL_LINEAR;
	GLenum mWrapS = GL_REPEAT;
	GLenum mWrapT = GL_REPEAT;
	Color mBorderColor = Color::error;
};