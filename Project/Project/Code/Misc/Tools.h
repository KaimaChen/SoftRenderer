#pragma once

class Tools
{
public:
	template<typename T>
	static void Swap(T &v1, T &v2);
};

//*****************************************************************************
template<typename T>
void Tools::Swap(T &v1, T &v2)
{
	T tmp = v2;
	v2 = v1;
	v1 = tmp;
}