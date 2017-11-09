#pragma once

#include "Libs\stb_image.h"
#include "Math\Vector2.h"
#include "Graphics\DataStructure\Color.h"
#include "Misc\Defines.h"

enum TextureFilter
{
	Nearest,
	Linear,
};

enum TextureWrap
{
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder, //超出部分渲染为指定边缘颜色
};

class Texture2D
{
public:
	Texture2D(const char *path);
	Texture2D(ubyte *data, int width, int height, int channelNum) : mData(data), mWidth(width), mHeight(height), mChannelNum(channelNum) {}
	~Texture2D() { delete mData; mData = nullptr; }
	Color Read(const Vector2 &uv) const;

	void SetFilter(TextureFilter filter) { mFilter = filter; }
	void SetWrap(TextureWrap wrapS, TextureWrap wrapT) { mWrapS = wrapS; mWrapT = wrapT; }
	void SetBorderColor(Color color) { mBorderColor = color; }

	TextureFilter GetFilter() { return mFilter; }
	TextureWrap GetWrapS() { return mWrapS; }
	TextureWrap GetWrapT() { return mWrapT; }
	Color GetBorderColor() { return mBorderColor; }

	Texture2D *GenMipMap() const;

private:
	Color NearestRead(const Vector2 &uv) const;
	Color LinearRead(const Vector2 &uv) const;
	int GetIndex(int x, int y) const;
	Color GetColor(int x, int y) const;
	Vector2 GetUV(Vector2 uv) const;

	static float Repeat(float v);
	static float MirrorRepeat(float v);
	static float ClampToEdge(float v);
	static float ClampToBorder(float v);

private:
	unsigned char *mData = nullptr;
	int mWidth = 0;
	int mHeight = 0;
	int mChannelNum = 4;
	TextureFilter mFilter = TextureFilter::Linear;
	TextureWrap mWrapS = TextureWrap::Repeat;
	TextureWrap mWrapT = TextureWrap::Repeat;
	Color mBorderColor = Color::error;
};