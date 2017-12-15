#include "TGALoader.h"

TGALoader::~TGALoader()
{
	if (mData)
	{
		delete mData;
		mData = nullptr;
	}
}

void TGALoader::Load(const std::string path)
{
	File file(path);
	file.Read(0, sizeof(TGAHeader), &mHeader);

	mWidth = mHeader.width;
	mHeight = mHeader.height;

	if (mHeader.colorDepth == 8 || mHeader.colorDepth == 24 || mHeader.colorDepth == 32)
	{
		mChannelNum = mHeader.colorDepth / 8;
		mDataLength = sizeof(char) * mWidth * mHeight * mChannelNum;

		mData = new char[mDataLength];

		if (mData)
		{
			file.Read(sizeof(TGAHeader), mDataLength, mData);
		}
	}
}

void TGALoader::LoadHeader(const std::string path)
{

}

void TGALoader::LoadUnCompressed(const std::string path)
{
	
}

void TGALoader::LoadCompressed(const std::string path)
{

}

void TGALoader::PrintHeader()
{
	cout << "idLength: " << (int)mHeader.idLength << endl;
	cout << "colorMapType: " << (int)mHeader.colorMapType << endl;
	cout << "imageType: " << (int)mHeader.imageType << endl;
	cout << "paletteStart: " << (int)mHeader.paletteStart << endl;
	cout << "paletteSize: " << (int)mHeader.paletteSize << endl;
	cout << "paletteEntryDepth: " << (int)mHeader.paletteEntryDepth << endl;
	cout << "x: " << (int)mHeader.x << endl;
	cout << "y: " << (int)mHeader.y << endl;
	cout << "width: " << (int)mHeader.width << endl;
	cout << "height: " << (int)mHeader.height << endl;
	cout << "colorDepth: " << (int)mHeader.colorDepth << endl;
	cout << "desc: " << (int)mHeader.descriptor << endl;
}