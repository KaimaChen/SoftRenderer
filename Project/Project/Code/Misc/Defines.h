#pragma once

#include <stddef.h>

using ubyte = unsigned char;
using uint = unsigned int;

using GLenum = unsigned int;
using GLbitfield = unsigned int;
using GLsizei = int;
using GLuint = unsigned int;
using GLsizeiptr = ptrdiff_t;

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



//Buffer Bit
#define GL_DEPTH_BUFFER_BIT									0x00000100
#define GL_STENCIL_BUFFER_BIT								0x00000400
#define GL_COLOR_BUFFER_BIT								0x00004000

//比较
#define GL_EQUAL													0x0001
#define GL_GREATER												0x0002
#define GL_GEQUAL													0x0003
#define GL_LESS														0x0004
#define GL_LEQUAL													0x0005
#define GL_NOTEQUAL												0x0006
#define GL_NEVER													0x0007
#define GL_ALWAYS													0x0008

//行为
#define GL_KEEP														0x0010
#define GL_ZERO														0x0011
#define GL_REPLACE													0x0012
#define GL_INCR														0x0013
#define GL_INCR_WRAP											0x0014
#define GL_DECR														0x0015
#define GL_DECR_WRAP											0x0016
#define GL_INVERT													0x0017

//Cull Face相关
#define GL_CW															0x0020
#define GL_CCW														0x0021
#define GL_FRONT_FACE											0x0022
#define GL_CULL_FACE_MODE									0x0023 

//Face
#define GL_FRONT													0x0030
#define GL_BACK														0x0031
#define GL_FRONT_AND_BACK									0x0032 //不绘制多边形，但是线与点模式则仍然绘制

//模板测试相关
#define GL_STENCIL_VALUE_MASK							0x0100
#define GL_STENCIL_WRITEMASK								0x0101
#define GL_STENCIL_FUNC										0x0102
#define GL_STENCIL_REF											0x0103
#define GL_STENCIL_BITS											0x0104
#define GL_STENCIL_FAIL											0x0105
#define GL_STENCIL_PASS_DEPTH_FAIL						0x0106
#define GL_STENCIL_PASS_DEPTH_PASS					0x0107
#define GL_STENCIL_CLEAR_VALUE							0x0108

//深度测试相关
#define GL_DEPTH_WRITEMASK								0x0110
#define GL_DEPTH_CLEAR_VALUE								0x0111

//颜色缓冲相关
#define GL_COLOR_CLEAR_VALUE								0x0120

//功能
#define GL_BLEND													0x0130
#define GL_CULL_FACE												0x0131
#define GL_DEPTH_TEST											0x0132
#define GL_STENCIL_TEST											0x0133

//Buffer
#define GL_ARRAY_BUFFER										0x0140

//Usage
#define GL_STREAM_DRAW										0x0150
#define GL_STREAM_READ										0x0151
#define GL_STREAM_COPY										0x0152
#define GL_STATIC_DRAW										0x0153
#define GL_STATIC_READ											0x0154
#define GL_STATIC_COPY											0x0155
#define GL_DYNAMIC_DRAW									0x0156
#define GL_DYNAMIC_READ										0x0157
#define GL_DYNAMIC_COPY										0x0158

//错误
#define GL_NO_ERROR												0			//该值特殊，需要保证为0以便和uint的默认值一致
#define GL_INVALID_ENUM										0xfff0
#define GL_INVALID_VALUE										0xfff1
#define GL_INVALID_OPERATION								0xfff2
#define GL_INVALID_FRAMEBUFFER_OPERATION		0xfff3
#define GL_OUT_OF_MEMORY									0xfff4