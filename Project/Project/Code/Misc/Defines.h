#pragma once

#include <stddef.h>

//*****************************************************************************
using ubyte = unsigned char;
using uint = unsigned int;

//*****************************************************************************
using GLboolean = bool; //1
using GLbyte = signed char; //8
using GLubyte = unsigned char; //8
using GLchar = char; //8
using GLshort = short; //16
using GLushort = unsigned short; //16
using GLint = int; //32
using GLuint = unsigned int; //32
using GLint64 = long long int; //64
using GLuint64 = unsigned long long int; //64
using GLfixed = int; //32
using GLsizei = int; //32
using GLenum = unsigned int; //32
using GLintptr = ptrdiff_t; //ptrbits
using GLsizeiptr = ptrdiff_t; //ptrbits
using GLbitfield = unsigned int; //32
using GLhalf = unsigned short; //16
using GLfloat = float; //32
using GLclampf = float; //32, [0,1]

using GLvoid = void;

//*****************************************************************************
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

#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr;} }
#define SAFE_FREE(p) { if(p) { free(p); p = nullptr;} }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] p; p = nullptr;} }


//*****************************************************************************
#define GL_FALSE														0
#define GL_TRUE														1
#define GL_ZERO														0
#define GL_ONE														1

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
#define GL_REPLACE													0x0011
#define GL_INCR														0x0012
#define GL_INCR_WRAP											0x0013
#define GL_DECR														0x0014
#define GL_DECR_WRAP											0x0015
#define GL_INVERT													0x0016

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
#define GL_STENCIL_BACK_FUNC								0x0109
#define	 GL_STENCIL_BACK_VALUE_MASK					0x010A
#define GL_STENCIL_BACK_REF									0x010B
#define GL_STENCIL_BACK_WRITEMASK					0x010C
#define GL_STENCIL_BACK_FAIL								0x010D
#define GL_STENCIL_BACK_PASS_DEPTH_PASS			0x010E
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL			0x010F

//深度测试相关
#define GL_DEPTH_WRITEMASK								0x0110
#define GL_DEPTH_CLEAR_VALUE								0x0111
#define GL_DEPTH_FUNC											0x0112

//颜色缓冲相关
#define GL_COLOR_CLEAR_VALUE								0x0120
#define GL_COLOR_WRITEMASK								0x0121

//功能
#define GL_BLEND													0x0130
#define GL_CULL_FACE												0x0131
#define GL_DEPTH_TEST											0x0132
#define GL_STENCIL_TEST											0x0133
#define GL_SCISSOR_TEST											0x0134

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

//Textures
#define GL_NEAREST												0x0200
#define GL_LINEAR													0x0201
#define GL_REPEAT													0x0210
#define GL_MIRRORED_REPEAT								0x0211
#define GL_CLAMP_TO_EDGE									0x0212
#define GL_CLAMP_TO_BORDER								0x0213

//*****************************************************************************
//Blend
#define GL_SRC_COLOR											0x0220
#define GL_ONE_MINUS_SRC_COLOR						0x0221
#define GL_DST_COLOR											0x0222
#define GL_ONE_MINUS_DST_COLOR						0x0223
#define GL_SRC_ALPHA												0x0224
#define GL_ONE_MINUS_SRC_ALPHA						0x0225
#define GL_DST_ALPHA												0x0226
#define GL_ONE_MINUS_DST_ALPHA						0x0227
#define GL_CONSTANT_COLOR								0x0228
#define GL_ONE_MINUS_CONSTANT_COLOR			0x0229
#define GL_CONSTANT_ALPHA									0x022A
#define GL_ONE_MINUS_CONSTANT_ALPHA			0x022B
#define GL_SRC_ALPHA_SATURATE							0x022C

#define GL_BLEND_COLOR										0x022F

#define GL_FUNC_ADD												0x0230
#define GL_FUNC_SUBTRACT									0x0231
#define GL_FUNC_REVERSE_SUBTRACT						0x0232
#define GL_MIN														0x0233
#define GL_MAX														0x0234

#define GL_BLEND_EQUATION_RGB							0x0240
#define GL_BLEND_EQUATION_ALPHA						0x0241
#define GL_BLEND_SRC												0x0242
#define GL_BLEND_DST												0x0243
#define GL_BLEND_SRC_RGB										0x0244
#define GL_BLEND_SRC_ALPHA									0x0245
#define GL_BLEND_DST_RGB										0x0246
#define GL_BLEND_DST_ALPHA									0x0247

//*****************************************************************************
//Texture
#define GL_TEXTURE_2D											0x0250
#define GL_TEXTURE_3D											0x0251
#define GL_TEXTURE_2D_ARRAY								0x0252
#define GL_TEXTURE_CUBE_MAP								0x0253
#define GL_TEXTURE_BINDING_2D							0x0254
#define GL_TEXTURE_BINDING_3D							0x0255
#define GL_TEXTURE_BINDING_2D_ARRAY				0x0256
#define GL_TEXTURE_BINDING_CUBE_MAP				0x0257
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X			0x0258
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X			0x0259
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y			0x025A
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y			0x025B
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z			0x025C
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z			0x025D

//*****************************************************************************
//其他
#define GL_MAX_VERTEX_ATTRIBS							0x0300
#define GL_MAX_VIEWPORT_DIMS							0x0301
#define GL_VIEWPORT												0x0302
#define GL_SCISSOR_BOX											0x0303

//*****************************************************************************
//数据类型
#define GL_BYTE														0x1000
#define GL_UNSIGNED_BYTE									0x1001
#define GL_SHORT													0x1002
#define GL_UNSIGNED_SHORT									0x1003
#define GL_INT															0x1004
#define GL_UNSIGNED_INT										0x1005
#define GL_HALF_FLOAT											0x1006
#define GL_FLOAT													0x1007
#define GL_FIXED														0x1008
#define GL_INT_2_10_10_10_REV								0x1009
#define GL_UNSIGNED_INT_2_10_10_10_REV				0x100A

//*****************************************************************************
//错误
#define GL_NO_ERROR												0			//该值特殊，需要保证为0以便和uint的默认值一致
#define GL_INVALID_ENUM										0xfff0
#define GL_INVALID_VALUE										0xfff1
#define GL_INVALID_OPERATION								0xfff2
#define GL_INVALID_FRAMEBUFFER_OPERATION		0xfff3
#define GL_OUT_OF_MEMORY									0xfff4