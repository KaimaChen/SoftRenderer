#include "BufferObject.h"

//*****************************************************************************
BufferObject::~BufferObject()
{
	Clear();
}

//*****************************************************************************
void BufferObject::Clear()
{
	mSize = 0;
	SAFE_FREE(mData);
}

//*****************************************************************************
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