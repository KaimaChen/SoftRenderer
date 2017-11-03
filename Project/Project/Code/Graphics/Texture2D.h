#pragma once

#include "Libs\stb_image.h"
#include "Math\Vector2.h"
#include "Graphics\DataStructure\Color.h"

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
	~Texture2D() { delete mData; mData = nullptr; }
	Color Read(const Vector2 &uv) const;

	void SetFilter(TextureFilter filter) { mFilter = filter; }
	TextureFilter GetFilter() { return mFilter; }
	void SetWrap(TextureWrap wrap) { mWrap = wrap; }
	TextureWrap GetWrap() { return mWrap; }
	void SetBorderColor(Color color) { mBorderColor = color; }
	Color GetBorderColor() { return mBorderColor; }

private:
	Color NearestRead(const Vector2 &uv) const;
	Color LinearRead(const Vector2 &uv) const;
	int GetIndex(int x, int y) const;
	Color GetColor(int x, int y) const;
	Vector2 GetUV(Vector2 uv) const;

private:
	unsigned char *mData;
	int mWidth;
	int mHeight;
	int mChannelNum;
	TextureFilter mFilter;
	TextureWrap mWrap;
	Color mBorderColor;
};