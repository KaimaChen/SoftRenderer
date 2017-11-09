#include "Texture2D.h"

Texture2D::Texture2D(const char *path)
{
	mData = stbi_load(path, &mWidth, &mHeight, &mChannelNum, 0);
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
	float s, t;
	switch (mWrapS)
	{
	case TextureWrap::MirroredRepeat:
		s = this->MirrorRepeat(uv.x);
		break;
	case TextureWrap::ClampToEdge:
		s = this->ClampToEdge(uv.x);
		break;
	case TextureWrap::ClampToBorder:
		s = this->ClampToBorder(uv.x);
		break;
	default:
		s = this->Repeat(uv.x);
		break;
	}

	switch (mWrapT)
	{
	case TextureWrap::MirroredRepeat:
		t = this->MirrorRepeat(uv.y);
		break;
	case TextureWrap::ClampToEdge:
		t = this->ClampToEdge(uv.y);
		break;
	case TextureWrap::ClampToBorder:
		t = this->ClampToBorder(uv.y);
		break;
	default:
		t = this->Repeat(uv.y);
		break;
	}

	return Vector2(s, t);
}

Texture2D *Texture2D::GenMipMap() const
{
	int width = mWidth / 2;
	if (width <= 0)
		width = 1;

	int height = mHeight / 2;
	if (height <= 0)
		height = 1;

	int channelNum = mChannelNum;
	
	int size = width * height * channelNum;
	ubyte *data = new ubyte[size]{ 1 };

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int srcIndex[4];
			float r = 0, g = 0, b = 0;

			//Box Filter: 取周围四个样本，进行平均
			srcIndex[0] = ((x * 2) + (y * 2) * mWidth) * channelNum;
			srcIndex[1] = ((x * 2 + 1) + (y * 2) * mWidth) * channelNum;
			srcIndex[2] = ((x * 2) + (y * 2 + 1) * mWidth) * channelNum;
			srcIndex[3] = ((x * 2 + 1) + (y * 2 + 1) * mWidth) * channelNum;

			for (int sample = 0; sample < 4; ++sample)
			{
				r += mData[srcIndex[sample]];
				g += mData[srcIndex[sample] + 1];
				b += mData[srcIndex[sample] + 2];
			}

			r /= 4.0f;
			g /= 4.0f;
			b /= 4.0f;

			data[(x + y * width) * channelNum] = r;
			data[(x + y * width) * channelNum + 1] = g;
			data[(x + y * width) * channelNum + 2] = b;
		}
	}

	return new Texture2D(data, width, height, channelNum);
}

float Texture2D::Repeat(float v)
{
	return Math::Frac(v);
}

float Texture2D::MirrorRepeat(float v)
{
	float fv = Math::Frac(v);
	if ((int)v % 2 == 1)
		fv = 1 - fv;
	return fv;
}

float Texture2D::ClampToEdge(float v)
{
	return Math::Clamp01(v);
}

float Texture2D::ClampToBorder(float v)
{
	if (v < 0 || v > 1)
		return -1;
	else
		return v;
}