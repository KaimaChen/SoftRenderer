#include "Texture2D.h"

Texture2D::Texture2D(const char *path)
{
	mData = stbi_load(path, &mWidth, &mHeight, &mChannelNum, 0);
	mFilter = TextureFilter::Linear;
	mWrap = TextureWrap::Repeat;
	mBorderColor = Color::error;
}

Color Texture2D::Read(const Vector2 &uv) const
{
	Vector2 st = GetUV(uv);
	if (st.x < 0 || st.y < 0)
		return mBorderColor;

	if (mFilter == TextureFilter::Nearest)
		return NearestRead(st);
	else
		return LinearRead(st);
}

Color Texture2D::NearestRead(const Vector2 &uv) const
{
	float u = Math::Frac(uv.x);
	float v = Math::Frac(uv.y);
	int x = (int)round(u * mWidth);
	int y = (int)round(v * mHeight);
	return GetColor(x, y);
}

Color Texture2D::LinearRead(const Vector2 &uv) const
{
	float u = Math::Frac(uv.x);
	float v = Math::Frac(uv.y);
	float fx = u * mWidth;
	float fy = v * mHeight;
	int x0 = (int)fx;
	int y0 = (int)fy;
	float dx = fx - x0;
	float dy = fy - y0;
	float omdx = 1 - dx;
	float omdy = 1 - dy;

	Color color = GetColor(x0, y0) * omdx * omdy +
		GetColor(x0, y0 + 1) * omdx * dy +
		GetColor(x0 + 1, y0) * dx * omdy +
		GetColor(x0 + 1, y0 + 1) * dx * dy;

	return color;
}

int Texture2D::GetIndex(int x, int y) const
{
	return (x + y * mWidth) * mChannelNum;
}

Color Texture2D::GetColor(int x, int y) const
{
	int index = GetIndex(x, y);
	return Color(
		mData[index] / 255.0f,
		mData[index + 1] / 255.0f,
		mData[index + 2] / 255.0f,
		1.0f
	);
}

Vector2 Texture2D::GetUV(Vector2 uv) const
{
	if (mWrap == TextureWrap::MirroredRepeat)
	{
		float s = Math::Frac(uv.x);
		float t = Math::Frac(uv.y);
		if (int(uv.x) % 2 == 1)
			s = 1 - s;
		if (int(uv.y) % 2 == 1)
			t = 1 - t;

		return Vector2(s, t);
	}
	else if (mWrap == TextureWrap::ClampToEdge)
	{
		return Vector2(
			Math::Clamp01(uv.x),
			Math::Clamp01(uv.y)
		);
	}
	else if (mWrap == TextureWrap::ClampToBorder)
	{
		if (uv.x < 0 || uv.y < 0 || uv.x > 1 || uv.y >1)
			return Vector2(-1, -1);
		else
			return uv;
	}
	else
	{
		return Vector2(
			Math::Frac(uv.x),
			Math::Frac(uv.y)
		);
	}
}