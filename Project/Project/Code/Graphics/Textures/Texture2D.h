#pragma once

#include "Libs\stb_image.h"
#include "Math\Vector2.h"
#include "Graphics\DataStructure\Color.h"
#include "Graphics\Textures\Texture.h"
#include "Misc\Defines.h"

class Texture2D : public Texture
{
public:
	Texture2D(const char *path);
	Texture2D(ubyte *data, int width, int height, int channelNum) : Texture(data, width, height, channelNum) {}
	~Texture2D() { SAFE_DELETE(mData); }

	Color Read(const Vector2 &uv) const;
	Texture2D *GenMipMap() const;

	void SetFilter(GLenum filter) { mFilter = filter; }

	GLenum GetFilter() { return mFilter; }

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
	GLenum mFilter = GL_LINEAR;
};