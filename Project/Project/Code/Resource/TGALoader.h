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
	byte idLength; //�ֶ�6�ĳ���
	byte colorMapType; //ʹ�õ���ɫ������
	byte imageType; //ʹ�õ�ͼ������
					//Color Map Spec����ɫ��˵��
	ushort paletteStart; //��һ��Ԫ������ֵ������ɫ����ʼλ��
	ushort paletteSize; //��ɫ�������Ԫ�ظ���
	byte paletteEntryDepth; //��ɫ��ÿһ��Ԫ�صĴ�С
							//Image Spec��ͼ��˵��
	ushort x;
	ushort y;
	ushort width;
	ushort height;
	byte colorDepth;
	byte descriptor; //ͼ������
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