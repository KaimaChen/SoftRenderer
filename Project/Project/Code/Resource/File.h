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
	int Read(int offset, int count, void *result);
	void Write(const string content);
private:
	string mPath;
	fstream mStream;
};