#pragma once

#include <iostream>
#include <cstdio>
using namespace std;

class Debug
{
public:
	static bool isOneFrameDone;
	static void Log(const char *msg) { cout << "[Info]: " << msg << endl; }
	static void Log(const double msg) { printf("[Info]: %.4f\n", msg); }
	static void LogWarning(const char *msg) { cout << "[Warning]: " << msg << endl; }
	static void LogError(const char *msg) { cerr << "[Error]: " << msg << endl; }
	static void LogOneFrame(const char *msg) { if (!isOneFrameDone) Log(msg); }
};

