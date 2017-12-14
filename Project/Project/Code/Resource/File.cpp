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
void File::Write(const string content)
{
	mStream.open(mPath, ios::out | ios::app);
	mStream << content;
	mStream.close();
}