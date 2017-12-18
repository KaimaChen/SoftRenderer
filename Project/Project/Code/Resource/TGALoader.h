//===========================================
// TGA加载类：目前支持RGB/RGBA的未压缩或RLE压缩的真彩色图像
//===========================================

#pragma once

#include <string>
#include <iostream>
#include <cstdio>
using namespace std;

using ubyte = unsigned char;
using ushort = unsigned short;

const int UNCOMPRESSED_TRUE_COLOR = 2; //未压缩的真彩色图像
const int RLE_TRUE_COLOR = 10; //RLE压缩的真彩色图像

//*****************************************************************************

struct TGAHeader
{
	ubyte idLength; //字段6的长度
	ubyte colorMapType; //使用的颜色表类型
	ubyte imageType; //使用的图像类型
					 //Color Map Spec：颜色表说明
	ushort paletteStart; //第一个元素索引值，即颜色表起始位置
	ushort paletteSize; //颜色表包含的元素个数
	ubyte paletteEntryDepth; //颜色表每一个元素的大小
							 //Image Spec：图像说明
	ushort x;
	ushort y;
	ushort width;
	ushort height;
	ubyte colorDepth;
	ubyte descriptor; //图像描述
};

//*****************************************************************************

class TGALoader
{
public:
	~TGALoader();

	bool Load(const std::string path);
	void PrintHeader();

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	int GetChannelNum() const { return mChannelNum; }
	void *GetData() const { return mData; }
	int GetDataLength() const { return mDataLength; }

private:
	void LoadHeader(FILE *stream);
	void LoadUncompressed(FILE *stream);
	void LoadCompressed(FILE *stream);
	void BGR2RGB();

private:
	int mWidth = 0;
	int mHeight = 0;
	ubyte *mData = nullptr;
	int mDataLength = 0;
	int mChannelNum = 0;
	TGAHeader mHeader;
};