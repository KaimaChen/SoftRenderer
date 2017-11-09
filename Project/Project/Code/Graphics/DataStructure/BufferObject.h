#pragma once

#include <malloc.h>
#include <cstring>
#include "Misc\Defines.h"

class BufferObject
{
public:
	~BufferObject();
	bool Copy(GLsizeiptr size, const void *data);
private:
	void Clear();
private:
	GLsizeiptr mSize;
	void *mData = nullptr;
};