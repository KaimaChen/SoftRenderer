#include "TGALoader.h"

TGALoader::~TGALoader()
{
	if (mData)
	{
		delete mData;
		mData = nullptr;
	}
}

//*****************************************************************************
bool TGALoader::Load(const std::string path)
{
	bool result = true;
	FILE *stream = nullptr;
	fopen_s(&stream, path.c_str(), "rb");
	LoadHeader(stream);

	if (mChannelNum <= 0)
	{
		fclose(stream);
		return false;
	}

	mDataLength = sizeof(ubyte) * mWidth * mHeight * mChannelNum;
	mData = new ubyte[mDataLength];

	if (!mData)
	{
		fclose(stream);
		return false;
	}

	for (int i = 0; i < mDataLength; i++)
		mData[i] = 0;

	if (mHeader.imageType == UNCOMPRESSED_TRUE_COLOR)
		LoadUncompressed(stream);
	else if (mHeader.imageType == RLE_TRUE_COLOR)
		LoadCompressed(stream);
	else
		result = false;

	//TGA存储的是BGR，因此要转换成RGB
	if (result)
		BGR2RGB();

	if (stream)
	{
		fclose(stream);
	}

	return result;
}

//*****************************************************************************
void TGALoader::LoadHeader(FILE *stream)
{
	//因为struct有内存对齐，所以没有一次过读sizeof(TGAHeader)到mHeader中
	fread(&mHeader.idLength, sizeof(ubyte), 1, stream);
	fread(&mHeader.colorMapType, sizeof(ubyte), 1, stream);
	fread(&mHeader.imageType, sizeof(ubyte), 1, stream);
	fread(&mHeader.paletteStart, sizeof(ushort), 1, stream);
	fread(&mHeader.paletteSize, sizeof(ushort), 1, stream);
	fread(&mHeader.paletteEntryDepth, sizeof(ubyte), 1, stream);
	fread(&mHeader.x, sizeof(ushort), 1, stream);
	fread(&mHeader.y, sizeof(ushort), 1, stream);
	fread(&mHeader.width, sizeof(ushort), 1, stream);
	fread(&mHeader.height, sizeof(ushort), 1, stream);
	fread(&mHeader.colorDepth, sizeof(ubyte), 1, stream);
	fread(&mHeader.descriptor, sizeof(ubyte), 1, stream);

	mWidth = mHeader.width;
	mHeight = mHeader.height;

	if (mWidth > 0 && mHeight > 0 && (mHeader.colorDepth == 24 || mHeader.colorDepth == 32))
		mChannelNum = mHeader.colorDepth / 8;
	else
		mChannelNum = 0;
}

//*****************************************************************************
void TGALoader::LoadUncompressed(FILE *stream)
{
	fread(mData, sizeof(ubyte), mDataLength, stream);
}

//*****************************************************************************
void TGALoader::LoadCompressed(FILE *stream)
{
	int pixelCount = mWidth * mHeight;
	int currentPixel = 0;
	int currentByte = 0;
	ubyte *colorBuffer = new ubyte[mChannelNum];

	do
	{
		ubyte chunkHeader = 0;
		fread(&chunkHeader, sizeof(ubyte), 1, stream);
		
		if (chunkHeader < 128) //RAW
		{
			chunkHeader++;
			for (int counter = 0; counter < chunkHeader; counter++)
			{
				size_t bytesRead = fread(colorBuffer, sizeof(ubyte), mChannelNum, stream); //Read one pixel
				if (bytesRead != mChannelNum)
				{
					cout << "Error RAW: " << endl;
					return;
				}

				for (int i = 0; i < mChannelNum; i++)
				{
					mData[currentByte + i] = colorBuffer[i];
				}
				currentByte += mChannelNum;
				currentPixel++;
			}
		}
		else //RLE
		{
			chunkHeader -= 127;
			size_t bytesRead = fread(colorBuffer, sizeof(ubyte), mChannelNum, stream);
			if (bytesRead != mChannelNum)
			{
				cout << "Error RLE: " << currentPixel << ", " << currentByte << ", " << bytesRead << ", " << mChannelNum << endl;
				return;
			}

			for (int counter = 0; counter < chunkHeader; counter++)
			{
				for (int i = 0; i < mChannelNum; i++)
				{
					mData[currentByte + i] = colorBuffer[i];
				}
				currentByte += mChannelNum;
				currentPixel++;
			}
		}
	} while (currentPixel < pixelCount);

	delete colorBuffer;
}

//*****************************************************************************
void TGALoader::BGR2RGB()
{
	for (int i = 0; i < mDataLength; i += mChannelNum)
	{
		mData[i] ^= mData[i + 2] ^= mData[i] ^= mData[i + 2];
	}
}

//*****************************************************************************
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