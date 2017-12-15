#pragma once

#include "File.h"
#include <string>
#include <iostream>
using namespace std;

using byte = unsigned char;
using ushort = unsigned short;

#pragma pack(1)

struct TGAHeader
{
	byte idLength; //字段6的长度
	byte colorMapType; //使用的颜色表类型
	byte imageType; //使用的图像类型
					//Color Map Spec：颜色表说明
	ushort paletteStart; //第一个元素索引值，即颜色表起始位置
	ushort paletteSize; //颜色表包含的元素个数
	byte paletteEntryDepth; //颜色表每一个元素的大小
							//Image Spec：图像说明
	ushort x;
	ushort y;
	ushort width;
	ushort height;
	byte colorDepth;
	byte descriptor; //图像描述
};

class TGALoader
{
public:
	~TGALoader();

	void Load(const std::string path);
	void PrintHeader();

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	void *GetData() const { return mData; }
	int GetDataLength() const { return mDataLength; }

private:
	void LoadHeader(const std::string path);
	void LoadUnCompressed(const std::string path);
	void LoadCompressed(const std::string path);

private:
	int mWidth = 0;
	int mHeight = 0;
	void *mData = nullptr;
	int mDataLength = 0;
	int mChannelNum = 0;
	TGAHeader mHeader;
};