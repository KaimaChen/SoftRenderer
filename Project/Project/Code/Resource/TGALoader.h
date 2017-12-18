//===========================================
// TGA�����ࣺĿǰ֧��RGB/RGBA��δѹ����RLEѹ�������ɫͼ��
//===========================================

#pragma once

#include <string>
#include <iostream>
#include <cstdio>
using namespace std;

using ubyte = unsigned char;
using ushort = unsigned short;

const int UNCOMPRESSED_TRUE_COLOR = 2; //δѹ�������ɫͼ��
const int RLE_TRUE_COLOR = 10; //RLEѹ�������ɫͼ��

//*****************************************************************************

struct TGAHeader
{
	ubyte idLength; //�ֶ�6�ĳ���
	ubyte colorMapType; //ʹ�õ���ɫ������
	ubyte imageType; //ʹ�õ�ͼ������
					 //Color Map Spec����ɫ��˵��
	ushort paletteStart; //��һ��Ԫ������ֵ������ɫ����ʼλ��
	ushort paletteSize; //��ɫ�������Ԫ�ظ���
	ubyte paletteEntryDepth; //��ɫ��ÿһ��Ԫ�صĴ�С
							 //Image Spec��ͼ��˵��
	ushort x;
	ushort y;
	ushort width;
	ushort height;
	ubyte colorDepth;
	ubyte descriptor; //ͼ������
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