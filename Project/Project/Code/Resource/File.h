#pragma once

#include <fstream>
#include <string>
using namespace std;

class File
{
public:
	File(const string path);
	~File();
	string Read();
	void Write(const string content);
private:
	string mPath;
	fstream mStream;
};