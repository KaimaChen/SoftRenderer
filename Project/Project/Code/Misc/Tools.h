#pragma once

#include <vector>

//*****************************************************************************
template<typename T>
void Swap(T &v1, T &v2)
{
	T tmp = v2;
	v2 = v1;
	v1 = tmp;
}

//*****************************************************************************
template<typename T>
bool RemoveItemFromVector(std::vector<T> &vec, const T &item)
{
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		if (*it == item)
		{
			vec.erase(it);
			return true;
		}
	}

	return false;
}