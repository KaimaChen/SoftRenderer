#pragma once

using byte = unsigned char;
using uint = unsigned int;
using GLenum = unsigned int;

#define DECLARE_SINGLETON_CG(name) private: \
class CGarbo\
{\
public:\
	~CGarbo()\
	{\
		if (name::mInstance)\
		{\
			delete name::mInstance;\
			name::mInstance = nullptr;\
		}\
	}\
};\
static CGarbo mGarbo;

#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28

//比较
#define GL_EQUAL							0x0000
#define GL_GREATER						0x0001
#define GL_GEQUAL							0x0002
#define GL_LESS								0x0003
#define GL_LEQUAL							0x0004
#define GL_NOTEQUAL						0x0005
#define GL_NEVER							0x0006
#define GL_ALWAYS							0x0007

//行为
#define GL_KEEP								0x0010
#define GL_ZERO								0x0011
#define GL_REPLACE							0x0012
#define GL_INCR								0x0013
#define GL_INCR_WRAP					0x0014
#define GL_DECR								0x0015
#define GL_DECR_WRAP					0x0016
#define GL_INVERT							0x0017

//模板测试相关
#define GL_STENCIL_WRITEMASK		0x0100
#define GL_STENCIL_FUNC				0x0101
#define GL_STENCIL_REF					0x0102