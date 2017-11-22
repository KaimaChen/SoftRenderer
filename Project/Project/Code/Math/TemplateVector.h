#pragma once

#include "Misc\Defines.h"

template<typename T, int Size>
class TemplateVector
{
public:
	static TemplateVector zero;
public:
	TemplateVector()
	{
		for (int i = 0; i < Size; ++i)
		{
			val[i] = 0;
		}
	}

	TemplateVector(T *arr)
	{
		for (int i = 0; i < Size; ++i)
		{
			val[i] = arr[i];
		}
	}

	const T &operator[](unsigned i) const { return val[i]; }
	T &operator[](unsigned i) { return val[i]; }
	T val[Size];
};

//*****************************************************************************
using bvec2 = TemplateVector<bool, 2>;
using bvec3 = TemplateVector<bool, 3>;
using bvec4 = TemplateVector<bool, 4>;

//*****************************************************************************
using fvec2 = TemplateVector<float, 2>;
using fvec3 = TemplateVector<float, 3>;
using fvec4 = TemplateVector<float, 4>;

//*****************************************************************************
using ivec2 = TemplateVector<int, 2>;
using ivec3 = TemplateVector<int, 3>;
using ivec4 = TemplateVector<int, 4>;

//*****************************************************************************
using uivec2 = TemplateVector<uint, 2>;
using uivec3 = TemplateVector<uint, 3>;
using uivec4 = TemplateVector<uint, 4>;

//*****************************************************************************
template<int Size>
bool all(const TemplateVector<bool, Size> &v)
{
	bool result = false;
	for (int i = 0; i < Size; ++i)
	{
		result |= v[i];
	}
	return result;
}

//*****************************************************************************
template<int Size>
bool any(const TemplateVector<bool, Size> &v)
{
	bool result = false;
	for (int i = 0; i < Size; ++i)
	{
		result |= v[i];
	}
	return result;
}