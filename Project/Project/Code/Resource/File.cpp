#include "File.h"

//*****************************************************************************
File::File(const string path)
{
	mPath = path;
}

//*****************************************************************************
File::~File()
{
	if (mStream.is_open())
	{
		mStream.close();
	}
}

//*****************************************************************************
string File::Read()
{
	string content;
	mStream.open(mPath, ios::in);
	while (!mStream.eof())
	{
		char s[1024];
		mStream.getline(s, sizeof(s));
		content += s;
		content += "\n";
	}
	mStream.close();
	return content;
}

//*****************************************************************************
int File::Read(int offset, int count, void *result)
{
	mStream.open(mPath, ios::in);
	mStream.seekg(offset, ios_base::beg);
	mStream.read((char*)result, count);
	int gcount = mStream.gcount();

	mStream.close();

	return gcount;
}

//*****************************************************************************
void File::Write(const string content)
{
	mStream.open(mPath, ios::out | ios::app);
	mStream << content;
	mStream.close();
}