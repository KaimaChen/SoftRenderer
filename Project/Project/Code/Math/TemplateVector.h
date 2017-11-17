#pragma once

template<typename T, int Size>
class TemplateVector
{
public:
	const T &operator[](unsigned i) const { return val[i]; }
	T &operator[](unsigned i) { return val[i]; }
	T val[Size];
};

using bvec2 = TemplateVector<bool, 2>;
using bvec3 = TemplateVector<bool, 3>;
using bvec4 = TemplateVector<bool, 4>;

//TODO: 能不能利用某种模板编程将下面三个整到一个函数中去
bool all(const bvec2 &v);
bool all(const bvec3 &v);
bool all(const bvec4 &v);
bool any(const bvec4 &v);