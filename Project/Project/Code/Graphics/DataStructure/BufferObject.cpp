#include "BufferObject.h"

BufferObject::~BufferObject()
{
	if (mData)
	{
		delete mData;
		mData = nullptr;
	}
}

void BufferObject::Clear()
{
	mSize = 0;
	if (mData)
	{
		free(mData);
		mData = nullptr;
	}
}

bool BufferObject::Copy(GLsizeiptr size, const void *data)
{
	Clear();

	mSize = size;
	mData = malloc(size);
	if (mData == NULL)
		return false;

	if (data)
	{
		memcpy(mData, data, size);
	}

	return true;
}