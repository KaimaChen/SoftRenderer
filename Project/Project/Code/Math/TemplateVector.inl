#include "TemplateVector.h"

bool all(const bvec2 &v)
{
	bool result = true;
	for (int i = 0; i < 2; ++i)
	{
		result &= v[i];
	}
	return result;
}

bool all(const bvec3 &v)
{
	bool result = true;
	for (int i = 0; i < 3; ++i)
	{
		result &= v[i];
	}
	return result;
}

bool all(const bvec4 &v)
{
	bool result = true;
	for (int i = 0; i < 4; ++i)
	{
		result &= v[i];
	}
	return result;
}

bool any(const bvec4 &v)
{
	bool result = false;
	for (int i = 0; i < 4; ++i)
	{
		result |= v[i];
	}
	return result;
}