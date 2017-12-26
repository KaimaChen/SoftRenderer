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

///Filter
#define GL_NEAREST												0x0200
#define GL_LINEAR													0x0201
#define GL_NEAREST_MIPMAP_NEAREST					0x0202
#define GL_LINEAR_MIPMAP_NEAREST						0x0203
#define GL_NEAREST_MIPMAP_LINEAR						0x0204
#define GL_LINEAR_MIPMAP_LINEAR						0x0205
///Wrap
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

#define GL_TEXTURE_BASE_LEVEL								0x0270
#define GL_TEXTURE_COMPARE_FUNC						0x0271
#define GL_TEXTURE_COMPARE_MODE					0x0272
#define GL_TEXTURE_MIN_FILTER								0x0273
#define GL_TEXTURE_MAG_FILTER							0x0274
#define GL_TEXTURE_MIN_LOD									0x0275
#define GL_TEXTURE_MAX_LOD								0x0276
#define GL_TEXTURE_MAX_LEVEL								0x0277
#define GL_TEXTURE_SWIZZLE_R								0x0278
#define GL_TEXTURE_SWIZZLE_G								0x0279
#define GL_TEXTURE_SWIZZLE_B								0x027A
#define GL_TEXTURE_SWIZZLE_A								0x027B
#define GL_TEXTURE_WRAP_S									0x027C
#define GL_TEXTURE_WRAP_T									0x027D
#define GL_TEXTURE_WRAP_R									0x027E

#define GL_TEXTURE0												0x0280
#define GL_TEXTURE1												0x0281
#define GL_TEXTURE2												0x0282
#define GL_TEXTURE3												0x0283
#define GL_TEXTURE4												0x0284
#define GL_TEXTURE5												0x0285
#define GL_TEXTURE6												0x0286
#define GL_TEXTURE7												0x0287
#define GL_TEXTURE8												0x0288
#define GL_TEXTURE9												0x0289
#define GL_TEXTURE10												0x028A
#define GL_TEXTURE11												0x028B
#define GL_TEXTURE12												0x028C
#define GL_TEXTURE13												0x028D
#define GL_TEXTURE14												0x028E
#define GL_TEXTURE15												0x028F
#define GL_TEXTURE16												0x0290
#define GL_TEXTURE17												0x0291
#define GL_TEXTURE18												0x0292
#define GL_TEXTURE19												0x0293
#define GL_TEXTURE20												0x0294
#define GL_TEXTURE21												0x0295
#define GL_TEXTURE22												0x0296
#define GL_TEXTURE23												0x0297
#define GL_TEXTURE24												0x0298
#define GL_TEXTURE25												0x0299
#define GL_TEXTURE26												0x029A
#define GL_TEXTURE27												0x029B
#define GL_TEXTURE28												0x029C
#define GL_TEXTURE29												0x029D
#define GL_TEXTURE30												0x029E
#define GL_TEXTURE31												0x029F

//*****************************************************************************
//其他
#define GL_MAX_VERTEX_ATTRIBS							0x0300
#define GL_MAX_VIEWPORT_DIMS							0x0301
#define GL_VIEWPORT												0x0302
#define GL_SCISSOR_BOX											0x0303
#define GL_MAX_TEXTURE_SIZE								0x0304
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS	0x0305

//*****************************************************************************
///Type
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
#define GL_UNSIGNED_SHORT_5_6_5						0x100B
#define GL_UNSIGNED_SHORT_4_4_4_4						0x100C
#define GL_UNSIGNED_SHORT_5_5_5_1						0x100D
#define GL_UNSIGNED_INT_10F_11F_11F_REV			0x100E
#define GL_UNSIGNED_INT_5_9_9_9_REV					0x100F
#define GL_UNSIGNED_INT_24_8								0x1010
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV		0x1011

//*****************************************************************************
///Format
#define GL_RGB														0x2000
#define GL_RGBA														0x2001
#define GL_LUMINANCE_ALPHA								0x2002
#define GL_LUMINANCE											0x2003
#define GL_ALPHA													0x2004
#define GL_R8															0x2005
#define GL_R8_SNORM												0x2006
#define GL_R16F														0x2007
#define GL_R32F														0x2008
#define GL_R8UI														0x2009
#define GL_R8I															0x200A
#define GL_R16UI														0x200B
#define GL_R16I														0x200C
#define GL_R32UI														0x200D
#define GL_R32I														0x200E
#define GL_RG8														0x200F
#define GL_RG8_SNORM											0x2010
#define GL_RG16F														0x2011
#define GL_RG32F														0x2012
#define GL_RG8UI														0x2013
#define GL_RG8I														0x2014
#define GL_RG16UI													0x2015
#define GL_RG16I														0x2016
#define GL_RG32UI													0x2017
#define GL_RG32I														0x2018
#define GL_RGB8														0x2019
#define GL_SRGB8													0x201A
#define GL_RGB565													0x201B
#define GL_RGB8_SNORM											0x201C
#define GL_R11F_G11F_B10F										0x201D
#define GL_RGB9_E5													0x201E
#define GL_RGB16F													0x201F
#define GL_RGB32F													0x2020
#define GL_RGB8UI													0x2021
#define GL_RGB8I														0x2022
#define GL_RGB16UI													0x2023
#define GL_RGB16I													0x2024
#define GL_RGB32UI													0x2025
#define GL_RGB32I													0x2026
#define GL_RGBA8													0x2027
#define GL_SRGB8_ALPHA8										0x2028
#define GL_RGBA8_SNORM										0x2029
#define GL_RGB5_A1													0x202A
#define GL_RGBA4													0x202B
#define GL_RGB10_A2												0x202C
#define GL_RGBA16F												0x202D
#define GL_RGBA32F												0x202E
#define GL_RGBA8UI													0x202F
#define GL_RGBA8I													0x2030
#define GL_RGB10_A2UI											0x2031
#define GL_RGBA16UI												0x2032
#define GL_RGBA16I													0x2033
#define GL_RGBA32I													0x2034
#define GL_RGBA32UI												0x2035
#define GL_DEPTH_COMPONENT16							0x2036
#define GL_DEPTH_COMPONENT24							0x2037
#define GL_DEPTH_COMPONENT32F						0x2038
#define GL_DEPTH24_STENCIL8								0x2039
#define GL_DEPTH32F_STENCIL8								0x203A
#define GL_RED														0x203B
#define GL_RED_INTEGER											0x203C
#define GL_RG															0x203D
#define GL_RG_INTEGER											0x203E
#define GL_RGB_INTEGER											0x203F
#define GL_RGBA_INTEGER										0x2040
#define GL_DEPTH_COMPONENT								0x2041
#define GL_DEPTH_STENCIL										0x2042

//*****************************************************************************
///Error
#define GL_NO_ERROR												0			//该值特殊，需要保证为0以便和uint的默认值一致
#define GL_INVALID_ENUM										0xfff0
#define GL_INVALID_VALUE										0xfff1
#define GL_INVALID_OPERATION								0xfff2
#define GL_INVALID_FRAMEBUFFER_OPERATION		0xfff3
#define GL_OUT_OF_MEMORY									0xfff4