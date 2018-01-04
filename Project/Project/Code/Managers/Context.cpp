#include "Context.h"

Context *Context::mInstance = nullptr;
Context::CGarbo Context::mGarbo;

//*****************************************************************************
Context::Context()
{
	mMaxViewportDims[0] = 16384;
	mMaxViewportDims[1] = 16384;

	mRenderMode = RenderMode::Shading;
	mWorldMat = Matrix4x4::identity;
	
	for (int i = 1023; i >= 0; --i)
	{
		mBufferIds.push(i);
		mTextureIds.push(i);
	}
}

//*****************************************************************************
Context::~Context()
{
	SAFE_DELETE(mShaderProgram);
	SAFE_DELETE(mTexture0);
	SAFE_DELETE(mTexture1);
	SAFE_DELETE(mMainLight);
	SAFE_DELETE(mMainCamera);

	auto it = mArrayBuffers.begin();
	while (it != mArrayBuffers.end())
	{
		delete it->second;
		it->second = nullptr;
		mArrayBuffers.erase(it++);
	}
}

//*****************************************************************************
Context *Context::Instance()
{
	if (mInstance == nullptr)
		mInstance = new Context();
	return mInstance;
}

//*****************************************************************************
void Context::SetWorldMat(Matrix4x4 worldMat)
{
	mWorldMat = worldMat;
	mITWorldMat = worldMat.InverseTranspose();
}

//*****************************************************************************
void Context::SetShaderProgram(ShaderProgram *program)
{
	SAFE_DELETE(mShaderProgram); //TODO：决定要不要delete上一个
	mShaderProgram = program;
}

//*****************************************************************************
void Context::Render()
{
	if (mShaderProgram == nullptr)
	{
		cout << "You need to set shader before call Render()." << endl;
		return;
	}

	Matrix4x4 vp = GetViewMat() * GetPerspectiveMat();
	mMVP = mWorldMat * vp;

	for (int i = 0; i < mIndices.size(); i += 3)
	{
		Pipeline(mVertices[mIndices[i]], mVertices[mIndices[i + 1]], mVertices[mIndices[i + 2]], vp);
	}
}

//*****************************************************************************
void Context::Pipeline(const VertexIn &v0, const VertexIn &v1, const VertexIn &v2, const Matrix4x4 &vp)
{
	mShaderProgram->InitShaderUniforms();
	mShaderProgram->InitShaderAttribs();
	mShaderProgram->PreExecute();

	Vector4 p0 = v0.position;
	Vector4 p1 = v1.position;
	Vector4 p2 = v2.position;

	Vector4 worldPos0, worldPos1, worldPos2;
	worldPos0 = p0 * mWorldMat;
	worldPos1 = p1 * mWorldMat;
	worldPos2 = p2 * mWorldMat;

	if (CullingFace(worldPos0, worldPos1, worldPos2) == false)
		return;

	//Object Space -> Clip Space
	VertexOut v2f0 = VertexOperation(v0);
	VertexOut v2f1 = VertexOperation(v1);
	VertexOut v2f2 = VertexOperation(v2);

	//3D裁剪，相对来说比较麻烦，建议在光栅化阶段进行裁剪
	//优化：这里只要有一个点在外面就整个三角形丢掉了，正确的做法是截取出一个多边形，然后生成一个或多个三角形
	//if (!Clip(v2f0.clipPos) || !Clip(v2f1.clipPos) || !Clip(v2f2.clipPos))
	//	return;

	//Clip Space -> NDC
	v2f0.clipPos /= v2f0.clipPos.w;
	v2f1.clipPos /= v2f1.clipPos.w;
	v2f2.clipPos /= v2f2.clipPos.w;

	//NDC -> Screen Coordinate
	Matrix4x4 screenMat = Matrix4x4::ScreenTransform(mViewportX, mViewportY, mViewportWidth, mViewportHeight);
	v2f0.screenPos = v2f0.clipPos * screenMat;
	v2f1.screenPos = v2f1.clipPos * screenMat;
	v2f2.screenPos = v2f2.clipPos * screenMat;

	if (mRenderMode == RenderMode::WireFrame)
		Drawing::Instance()->DrawTriangleWire(v2f0, v2f1, v2f2);
	else
		Drawing::Instance()->DrawTriangle(v2f0, v2f1, v2f2, mShaderProgram);
}

//*****************************************************************************
bool Context::CullingFace(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2)
{
	if (mRenderMode == RenderMode::WireFrame)
		return true;

	if (mIsCullFaceEnabled == false)
		return true;

	if (mCullFace == GL_FRONT_AND_BACK)
		return false;

	int dir = (mFrontFace == GL_CCW) ? 1 : -1; //正面的顶点顺序
	Vector4 d01 = p1 - p0;
	Vector4 d12 = p2 - p1;
	Vector4 normal = d01.Cross(d12) * dir;
	Vector4 viewDir = p0 - mMainCamera->eye;
	
	int judgement = (mCullFace == GL_BACK) ? 1 : -1; //剔除正面或背面

	if (normal.Dot(viewDir) * judgement < 0)
		return true;

	return false;
}

//*****************************************************************************
VertexOut Context::VertexOperation(const VertexIn &appdata)
{
	if (mShaderProgram == nullptr)
	{
		cout << "No shader program has been set" << endl;
		return VertexOut();
	}

	mShaderProgram->SetMatrix(mWorldMat, WORLD_MAT_INDEX);
	mShaderProgram->SetMatrix(mITWorldMat, IT_WORLD_MAT_INDEX);
	mShaderProgram->SetMatrix(mMVP, MVP_MAT_INDEX);
	mShaderProgram->SetLight(*mMainLight);

	VertexOut v2f = mShaderProgram->ExecuteVertexShader(appdata);
	v2f.BeginPerspectiveCorrectInterpolation();

	return v2f;
}

//*****************************************************************************
//TODO
bool Context::Clip(const Vector4 &p) const
{
	if (/*p.x >= -p.w && p.x <= p.w &&
		p.y >= -p.w && p.y <= p.w &&*/
		p.z >= 0.0f && p.z <= p.w)
		return true;
	
	return false;
}

//*****************************************************************************
void Context::glGetIntegerv(GLenum pname, int *data)
{
	switch (pname)
	{
	case GL_STENCIL_VALUE_MASK:
		*data = mStencilValueMask;
		break;
	case GL_STENCIL_BACK_VALUE_MASK:
		*data = mStencilBackValueMask;
		break;
	case GL_STENCIL_WRITEMASK:
		*data = mStencilWriteMask;
		break;
	case GL_STENCIL_BACK_WRITEMASK:
		*data = mStencilBackWriteMask;
		break;
	case GL_STENCIL_FUNC:
		*data = mStencilFunc;
		break;
	case GL_STENCIL_BACK_FUNC:
		*data = mStencilBackFunc;
		break;
	case GL_STENCIL_REF:
		*data = mStencilRef;
		break;
	case GL_STENCIL_BACK_REF:
		*data = mStencilBackRef;
		break;
	case GL_STENCIL_BITS:
		*data = NUM_OF_BITS;
		break;
	case GL_STENCIL_FAIL:
		*data = mStencilFail;
		break;
	case GL_STENCIL_BACK_FAIL:
		*data = mStencilBackFail;
		break;
	case GL_STENCIL_PASS_DEPTH_FAIL:
		*data = mStencilPassDepthFail;
		break;
	case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
		*data = mStencilBackPassDepthFail;
		break;
	case GL_STENCIL_PASS_DEPTH_PASS:
		*data = mStencilPassDepthPass;
		break;
	case GL_STENCIL_BACK_PASS_DEPTH_PASS:
		*data = mStencilBackPassDepthPass;
		break;
	case GL_STENCIL_CLEAR_VALUE:
		*data = mStencilClearValue;
		break;
	case GL_FRONT_FACE:
		*data = mFrontFace;
		break;
	case GL_BLEND_EQUATION_RGB:
		*data = mRGBBlendEquation;
		break;
	case GL_BLEND_EQUATION_ALPHA:
		*data = mAlphaBlendEquation;
		break;
	case GL_BLEND_SRC:
		*data = mSrcRGBBlendFunc;
		break;
	case GL_BLEND_DST:
		*data = mDstRGBBlendFunc;
		break;
	case GL_BLEND_SRC_RGB:
		*data = mSrcRGBBlendFunc;
		break;
	case GL_BLEND_SRC_ALPHA:
		*data = mSrcAlphaBlendFunc;
		break;
	case GL_BLEND_DST_RGB:
		*data = mDstRGBBlendFunc;
		break;
	case GL_BLEND_DST_ALPHA:
		*data = mDstAlphaBlendFunc;
		break;
	case GL_MAX_VERTEX_ATTRIBS:
		*data = mMaxVertexAttribs;
		break;
	case GL_MAX_VIEWPORT_DIMS:
		data[0] = mMaxViewportDims[0];
		data[1] = mMaxViewportDims[1];
		break;
	case GL_VIEWPORT:
		data[0] = mViewportX;
		data[1] = mViewportY;
		data[2] = mViewportWidth;
		data[3] = mViewportHeight;
		break;
	case GL_SCISSOR_BOX:
		data[0] = mScissorBoxX;
		data[1] = mScissorBoxY;
		data[2] = mScissorBoxWidth;
		data[3] = mScissorBoxHeight;
		break;
	case GL_DEPTH_FUNC:
		*data = mDepthFunc;
		break;
	case GL_TEXTURE_BINDING_2D:
		*data = mBindingTextureId[GL_TEXTURE_2D];
		break;
	case GL_TEXTURE_BINDING_3D:
		*data = mBindingTextureId[GL_TEXTURE_3D];
		break;
	case GL_TEXTURE_BINDING_2D_ARRAY:
		*data = mBindingTextureId[GL_TEXTURE_2D_ARRAY];
		break;
	case GL_TEXTURE_BINDING_CUBE_MAP:
		*data = mBindingTextureId[GL_TEXTURE_CUBE_MAP];
		break;
	case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
		*data = mMaxCombinedTextureUnits;
		break;
	default:
		AddError(GL_INVALID_ENUM);
		break;
	}
}

//*****************************************************************************
void Context::glGetBooleanv(GLenum pname, bool *data)
{
	switch (pname)
	{
	case GL_DEPTH_WRITEMASK:
		*data = mDepthWriteMask;
		break;
	case GL_COLOR_WRITEMASK:
		data[0] = mRedMask;
		data[1] = mGreenMask;
		data[2] = mBlueMask;
		data[3] = mAlphaMask;
		break;
	default:
		AddError(GL_INVALID_ENUM);
		break;
	}
}

//*****************************************************************************
void Context::glGetFloatv(GLenum pname, float *data)
{
	switch (pname)
	{
	case GL_DEPTH_CLEAR_VALUE:
		*data = mDepthClearValue;
		break;
	case GL_COLOR_CLEAR_VALUE:
		data[0] = mColorClearValue.r;
		data[1] = mColorClearValue.g;
		data[2] = mColorClearValue.b;
		data[3] = mColorClearValue.a;
		break;
	case GL_BLEND_COLOR:
		data[0] = mBlendColor.r;
		data[1] = mBlendColor.g;
		data[2] = mBlendColor.b;
		data[3] = mBlendColor.a;
		break;
	default:
		AddError(GL_INVALID_ENUM);
		break;
	}
}

//*****************************************************************************
bool Context::glIsEnabled(GLenum cap)
{
	switch (cap)
	{
	case GL_BLEND:
		return mIsBlendEnabled;
	case GL_CULL_FACE:
		return mIsCullFaceEnabled;
	case GL_DEPTH_TEST:
		return mIsDepthTestEnabled;
	case GL_STENCIL_TEST:
		return mIsStencilTestEnabled;
	case GL_SCISSOR_TEST:
		return mIsScissorTestEnabled;
	default:
		cout << "In glIsEnabled Capability = " << cap << " doesn't exist !!" << endl;
		AddError(GL_INVALID_ENUM);
		return false;
	}
}

//*****************************************************************************
void Context::glEnable(GLenum cap)
{
	switch (cap)
	{
	case GL_BLEND:
		mIsBlendEnabled = true;
		break;
	case GL_CULL_FACE:
		mIsCullFaceEnabled = true;
		break;
	case GL_DEPTH_TEST:
		mIsDepthTestEnabled = true;
		break;
	case GL_STENCIL_TEST:
		mIsStencilTestEnabled = true;
		break;
	case GL_SCISSOR_TEST:
		mIsScissorTestEnabled = true;
		break;
	default:
		cout << "Capability = " << cap << " doesn't exist !!" << endl;
		AddError(GL_INVALID_ENUM);
		break;
	}
}

//*****************************************************************************
void Context::glDisable(GLenum cap)
{
	switch (cap)
	{
	case GL_BLEND:
		mIsBlendEnabled = false;
		break;
	case GL_CULL_FACE:
		mIsCullFaceEnabled = false;
		break;
	case GL_DEPTH_TEST:
		mIsDepthTestEnabled = false;
		break;
	case GL_STENCIL_TEST:
		mIsStencilTestEnabled = false;
		break;
	default:
		cout << "Capability = " << cap << " doesn't exist !!" << endl;
		AddError(GL_INVALID_ENUM);
		break;
	}
}

//*****************************************************************************
Texture2D *Context::GetTexture2D(int index)
{
	GLenum textureUnit = GL_TEXTURE0 + index;
	GLuint textureId = mTextureUnits[textureUnit];
	Texture2D *texture = mTexture2Ds[textureId];
	
	return texture;
}

//*****************************************************************************
void Context::glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	std::vector<GLenum> enums = {
		GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS
	};
	if (!CheckEnum(func, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	ref = Math::Clamp(ref, 0, (1 << NUM_OF_BITS) - 1);
	mStencilFunc = func;
	mStencilRef = ref;
	mStencilValueMask = mask;
}

//*****************************************************************************
void Context::glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
	if (face != GL_FRONT || face != GL_BACK || face != GL_FRONT_AND_BACK)
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	std::vector<GLenum> enums = {
		GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS 
	};
	if (!CheckEnum(func, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	ref = Math::Clamp(ref, 0, (1 << NUM_OF_BITS) - 1);

	if (face == GL_FRONT)
	{
		mStencilFunc = func;
		mStencilRef = ref;
		mStencilValueMask = mask;
	}
	else if (face == GL_BACK)
	{
		mStencilBackFunc = func;
		mStencilBackRef = ref;
		mStencilBackValueMask = mask;
	}
	else
	{
		mStencilFunc = mStencilBackFunc = func;
		mStencilRef = mStencilBackRef = ref;
		mStencilValueMask = mStencilBackValueMask = mask;
	}
}

//*****************************************************************************
void Context::glStencilMask(GLuint mask)
{
	mask = Math::Clamp(mask, 0, (1 << NUM_OF_BITS) - 1);
	mStencilWriteMask = mask;
}

//*****************************************************************************
void Context::glStencilMaskSeparate(GLenum face, GLuint mask)
{
	if (face != GL_FRONT || face != GL_BACK || face != GL_FRONT_AND_BACK)
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mask = Math::Clamp(mask, 0, (1 << NUM_OF_BITS) - 1);
	
	if (face == GL_FRONT)
	{
		mStencilWriteMask = mask;
	}
	else if (face == GL_BACK)
	{
		mStencilBackWriteMask = mask;
	}
	else
	{
		mStencilWriteMask = mStencilBackWriteMask = mask;
	}
}

//*****************************************************************************
void Context::glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	std::vector<GLenum> enums = {
		GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT
	};
	if (!CheckEnum(sfail, enums) || !CheckEnum(dpfail, enums) || !CheckEnum(dppass, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mStencilFail = sfail;
	mStencilPassDepthFail = dpfail;
	mStencilPassDepthPass = dppass;
}

//*****************************************************************************
void Context::glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
	if (face != GL_FRONT || face != GL_BACK || face != GL_FRONT_AND_BACK)
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	std::vector<GLenum> enums = {
		GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT	
	};
	if (!CheckEnum(sfail, enums) || !CheckEnum(dpfail, enums) || !CheckEnum(dppass, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	if (face == GL_FRONT)
	{
		mStencilFail = sfail;
		mStencilPassDepthFail = dpfail;
		mStencilPassDepthPass = dppass;
	}
	else if (face == GL_BACK)
	{
		mStencilBackFail = sfail;
		mStencilBackPassDepthFail = dpfail;
		mStencilBackPassDepthPass = dppass;
	}
	else
	{
		mStencilFail = mStencilBackFail = sfail;
		mStencilPassDepthFail = mStencilBackPassDepthFail = dpfail;
		mStencilPassDepthPass = mStencilBackPassDepthPass = dppass;
	}
}

//*****************************************************************************
void Context::glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (width < 0 || height < 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	mScissorBoxX = x;
	mScissorBoxY = y;
	mScissorBoxWidth = width;
	mScissorBoxHeight = height;
}

//*****************************************************************************
void Context::glDepthFunc(GLenum func)
{
	std::vector<GLenum> enums = {
		GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS
	};
	if (!CheckEnum(func, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mDepthFunc = func;
}

//*****************************************************************************
void Context::glFrontFace(GLenum mode)
{
	if (mode != GL_CW || mode != GL_CCW)
	{
		cout << "FrontFace only accept GL_CW or GL_CCW" << endl;
		AddError(GL_INVALID_ENUM);
		return;
	}

	mFrontFace = mode;
}

//*****************************************************************************
void Context::glCullFace(GLenum mode)
{
	if (mode != GL_FRONT || mode != GL_BACK || mode != GL_FRONT_AND_BACK)
	{
		cout << "CullFace only accept GL_FRONT or GL_BACK or GL_FRONT_AND_BACK" << endl;
		AddError(GL_INVALID_ENUM);
		return;
	}

	mCullFace = mode;
}

//*****************************************************************************
void Context::glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	std::vector<GLenum> enums = { 
		GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE
	};
	if(!CheckEnum(sfactor, enums) || !CheckEnum(dfactor, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mSrcRGBBlendFunc = mSrcAlphaBlendFunc = sfactor;
	mDstRGBBlendFunc = mDstAlphaBlendFunc = dfactor;
}

//*****************************************************************************
void Context::glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	std::vector<GLenum> enums = {
		GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE
	};
	if (!CheckEnum(srcRGB, enums) || !CheckEnum(dstRGB, enums) || !CheckEnum(srcAlpha, enums) || !CheckEnum(dstAlpha, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mSrcRGBBlendFunc = srcRGB;
	mDstRGBBlendFunc = dstRGB;
	mSrcAlphaBlendFunc = srcAlpha;
	mDstAlphaBlendFunc = dstAlpha;
}

//*****************************************************************************
void Context::glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	mBlendColor = Color(red, green, blue, alpha);
}

//*****************************************************************************
void Context::glBlendEquation(GLenum mode)
{
	std::vector<GLenum> enums{
		GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX
	};
	if (!CheckEnum(mode, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mRGBBlendEquation = mAlphaBlendEquation = mode;
}

//*****************************************************************************
void Context::glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
	std::vector<GLenum> enums{
		GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX
	};
	if (!CheckEnum(modeRGB, enums) || !CheckEnum(modeAlpha, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mRGBBlendEquation = modeRGB;
	mAlphaBlendEquation = modeAlpha;
}

//*****************************************************************************
void Context::glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	mRedMask = red;
	mGreenMask = green;
	mBlueMask = blue;
	mAlphaMask = alpha;
}

//*****************************************************************************
GLenum Context::glGetError()
{
	if (mErrors.size() == 0)
		return GL_NO_ERROR;

	GLenum error = mErrors.top();
	mErrors.pop();
	return error;
}

//*****************************************************************************
void Context::glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (width < 0 || height < 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	mViewportX = x;
	mViewportY = y;
	mViewportWidth = width;
	mViewportHeight = height;
}

//*****************************************************************************
void Context::AddError(GLenum error)
{
	if (error != GL_INVALID_ENUM || error != GL_INVALID_VALUE || error != GL_INVALID_OPERATION ||
		error != GL_INVALID_FRAMEBUFFER_OPERATION || error != GL_OUT_OF_MEMORY)
		return;

	mErrors.push(error);
}

//*****************************************************************************
bool Context::CheckEnum(GLenum target, const std::vector<GLenum> &enums)
{
	for (int i = 0; i < enums.size(); ++i)
	{
		if (target == enums[i])
			return true;
	}

	return false;
}

//*****************************************************************************
bool Context::IsUnsizedInternalFormats(GLenum internalFormat, GLenum format, GLenum type)
{
	if ((internalFormat == GL_RGB && format == GL_RGB && (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT_5_6_5)) ||
		(internalFormat == GL_RGBA && format == GL_RGBA && (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT_4_4_4_4 || type == GL_UNSIGNED_SHORT_5_5_5_1)) ||
		(internalFormat == GL_LUMINANCE_ALPHA && format == GL_LUMINANCE_ALPHA && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_LUMINANCE && format == GL_LUMINANCE && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_ALPHA && format == GL_ALPHA && type == GL_UNSIGNED_BYTE))
		return true;
	else
		return false;
}

//*****************************************************************************
bool Context::IsSizedInternalFormats(GLenum internalFormat, GLenum format, GLenum type)
{
	if ((internalFormat == GL_R8 && format == GL_RED && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_R8_SNORM && format == GL_RED && type == GL_BYTE) ||
		(internalFormat == GL_R16F && format == GL_RED && (type == GL_HALF_FLOAT || type == GL_FLOAT)) ||
		(internalFormat == GL_R32F && format == GL_RED && type == GL_FLOAT) ||
		(internalFormat == GL_R8UI && format == GL_RED_INTEGER && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_R8I && format == GL_RED_INTEGER && type == GL_BYTE) ||
		(internalFormat == GL_R16UI && format == GL_RED_INTEGER && type == GL_UNSIGNED_SHORT) ||
		(internalFormat == GL_R16I && format == GL_RED_INTEGER && type == GL_SHORT) ||
		(internalFormat == GL_R32UI && format == GL_RED_INTEGER && type == GL_UNSIGNED_INT) ||
		(internalFormat == GL_R32I && format == GL_RED_INTEGER && type == GL_INT) ||
		(internalFormat == GL_RG8 && format == GL_RG && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_RG8_SNORM && format == GL_RG && type == GL_BYTE) ||
		(internalFormat == GL_RG16F && format == GL_RG && (type == GL_HALF_FLOAT || type == GL_FLOAT)) ||
		(internalFormat == GL_RG32F && format == GL_RG && type == GL_FLOAT) ||
		(internalFormat == GL_RG8UI && format == GL_RG_INTEGER && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_RG8I && format == GL_RG_INTEGER && type == GL_BYTE) ||
		(internalFormat == GL_RG16UI && format == GL_RG_INTEGER && type == GL_UNSIGNED_SHORT) ||
		(internalFormat == GL_RG16I && format == GL_RG_INTEGER && type == GL_SHORT) ||
		(internalFormat == GL_RG32UI && format == GL_RG_INTEGER && type == GL_UNSIGNED_INT) ||
		(internalFormat == GL_RG32I && format == GL_RG_INTEGER && type == GL_INT) ||
		(internalFormat == GL_RGB8 && format == GL_RGB && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_SRGB8 && format == GL_RGB && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_RGB565 && format == GL_RGB && (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT_5_6_5)) ||
		(internalFormat == GL_RGB8_SNORM && format == GL_RGB && type == GL_BYTE) ||
		(internalFormat == GL_R11F_G11F_B10F && format == GL_RGB && (type == GL_UNSIGNED_INT_10F_11F_11F_REV || type == GL_HALF_FLOAT || type == GL_FLOAT)) ||
		(internalFormat == GL_RGB9_E5 && format == GL_RGB && (type == GL_UNSIGNED_INT_5_9_9_9_REV || type == GL_HALF_FLOAT || type == GL_FLOAT)) ||
		(internalFormat == GL_RGB16F && format == GL_RGB && (type == GL_HALF_FLOAT || type == GL_FLOAT)) ||
		(internalFormat == GL_RGB32F && format == GL_RGB && type == GL_FLOAT) ||
		(internalFormat == GL_RGB8UI && format == GL_RGB_INTEGER && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_RGB8I && format == GL_RGB_INTEGER && type == GL_BYTE) ||
		(internalFormat == GL_RGB16UI && format == GL_RGB_INTEGER && type == GL_UNSIGNED_SHORT) ||
		(internalFormat == GL_RGB16I && format == GL_RGB_INTEGER && type == GL_SHORT) ||
		(internalFormat == GL_RGB32UI && format == GL_RGB_INTEGER && type == GL_UNSIGNED_INT) ||
		(internalFormat == GL_RGB32I && format == GL_RGB_INTEGER && type == GL_INT) ||
		(internalFormat == GL_RGBA8 && format == GL_RGBA && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_SRGB8_ALPHA8 && format == GL_RGBA && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_RGBA8_SNORM && format == GL_RGBA && type == GL_BYTE) ||
		(internalFormat == GL_RGB5_A1 && format == GL_RGBA && (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT_5_5_5_1 || type == GL_UNSIGNED_INT_2_10_10_10_REV)) ||
		(internalFormat == GL_RGBA4 && format == GL_RGBA && (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT_4_4_4_4)) ||
		(internalFormat == GL_RGB10_A2 && format == GL_RGBA && type == GL_UNSIGNED_INT_2_10_10_10_REV) ||
		(internalFormat == GL_RGBA16F && format == GL_RGBA && (type == GL_HALF_FLOAT || type == GL_FLOAT)) ||
		(internalFormat == GL_RGBA32F && format == GL_RGBA && type == GL_FLOAT) ||
		(internalFormat == GL_RGBA8UI && format == GL_RGBA_INTEGER && type == GL_UNSIGNED_BYTE) ||
		(internalFormat == GL_RGBA8I && format == GL_RGBA_INTEGER && type == GL_BYTE) ||
		(internalFormat == GL_RGB10_A2UI && format == GL_RGBA_INTEGER && type == GL_UNSIGNED_INT_2_10_10_10_REV) ||
		(internalFormat == GL_RGBA16UI && format == GL_RGBA_INTEGER && type == GL_UNSIGNED_SHORT) ||
		(internalFormat == GL_RGBA16I && format == GL_RGBA_INTEGER && type == GL_SHORT) ||
		(internalFormat == GL_RGBA32I && format == GL_RGBA_INTEGER && type == GL_INT) ||
		(internalFormat == GL_RGBA32UI && format == GL_RGBA_INTEGER && type == GL_UNSIGNED_INT) ||
		(internalFormat == GL_DEPTH_COMPONENT16 && format == GL_DEPTH_COMPONENT && (type == GL_UNSIGNED_SHORT || type == GL_UNSIGNED_INT)) ||
		(internalFormat == GL_DEPTH_COMPONENT24 && format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_INT) ||
		(internalFormat == GL_DEPTH_COMPONENT32F && format == GL_DEPTH_COMPONENT && type == GL_FLOAT) ||
		(internalFormat == GL_DEPTH24_STENCIL8 && format == GL_DEPTH_STENCIL && type == GL_UNSIGNED_INT_24_8) ||
		(internalFormat == GL_DEPTH32F_STENCIL8 && format == GL_DEPTH_STENCIL && type == GL_FLOAT_32_UNSIGNED_INT_24_8_REV))
		return true;
	else
		return false;
}

//*****************************************************************************
void Context::glClear(GLbitfield mask)
{
	if ((mask ^ GL_COLOR_BUFFER_BIT ^ GL_DEPTH_BUFFER_BIT ^ GL_STENCIL_BUFFER_BIT) != 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	if ((mask & GL_COLOR_BUFFER_BIT) == GL_COLOR_BUFFER_BIT)
		Drawing::Instance()->ClearColorBuffer(mColorClearValue);
	if ((mask & GL_DEPTH_BUFFER_BIT) == GL_DEPTH_BUFFER_BIT)
		Drawing::Instance()->ClearDepthBuffer(mDepthClearValue);
	if ((mask & GL_STENCIL_BUFFER_BIT) == GL_STENCIL_BUFFER_BIT)
		Drawing::Instance()->ClearStencilBuffer(mStencilClearValue);
}

//*****************************************************************************
void Context::glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	mColorClearValue = Color(red, green, blue, alpha);
}

//*****************************************************************************
void Context::glClearDepthf(GLfloat depth)
{
	//TODO: floating-point depth buffer直接赋值，但是fixed-point depth buffer需要先转换
	mDepthClearValue = depth;
}

//*****************************************************************************
void Context::glClearStencil(GLint s)
{
	GLint maxValue = (int)((1 << NUM_OF_BITS) - 1);
	GLint value = s & maxValue;
	mStencilClearValue = value;
}

//*****************************************************************************
void Context::glUniform1f(GLint location, GLfloat v0)
{
	if (mShaderProgram != nullptr)
	{
		mShaderProgram->SetUniform1f(location, v0);
	}
}

//*****************************************************************************
void Context::glUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
	if (mShaderProgram != nullptr)
	{
		GLfloat arr[2] = { v0, v1 };
		mShaderProgram->SetUniform2f(location, arr);
	}
}

//*****************************************************************************
void Context::glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	if (mShaderProgram != nullptr)
	{
		GLfloat arr[3] = { v0, v1, v2 };
		mShaderProgram->SetUniform3f(location, arr);
	}
}

//*****************************************************************************
void Context::glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	if (mShaderProgram != nullptr)
	{
		GLfloat arr[4] = { v0, v1, v2, v3 };
		bool result = mShaderProgram->SetUniform4f(location, arr);
		if (!result)
		{
			mShaderProgram->SetColorUniform(location, Color(v0, v1, v2, v3));
		}
	}
}

//*****************************************************************************
void Context::glUniform1i(GLint location, GLint v0)
{
	if (mShaderProgram != nullptr)
	{
		mShaderProgram->SetUniform1i(location, v0);
	}
}

//*****************************************************************************
void Context::glUniform2i(GLint location, GLint v0, GLint v1)
{
	if (mShaderProgram != nullptr)
	{
		GLint arr[2] = { v0, v1 };
		mShaderProgram->SetUniform2i(location, arr);
	}
}

//*****************************************************************************
void Context::glUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
	if (mShaderProgram != nullptr)
	{
		GLint arr[3] = { v0, v1, v2 };
		mShaderProgram->SetUniform3i(location, arr);
	}
}

//*****************************************************************************
void Context::glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	if (mShaderProgram != nullptr)
	{
		GLint arr[4] = { v0, v1, v2, v3 };
		mShaderProgram->SetUniform4i(location, arr);
	}
}

//*****************************************************************************
void Context::glUniform1ui(GLint location, GLuint v0)
{
	if (mShaderProgram != nullptr)
	{
		mShaderProgram->SetUniform1ui(location, v0);
	}
}

//*****************************************************************************
void Context::glUniform2ui(GLint location, GLuint v0, GLuint v1)
{
	if (mShaderProgram != nullptr)
	{
		GLuint arr[2] = { v0, v1 };
		mShaderProgram->SetUniform2ui(location, arr);
	}
}

//*****************************************************************************
void Context::glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
	if (mShaderProgram != nullptr)
	{
		GLuint arr[3] = { v0, v1, v2 };
		mShaderProgram->SetUniform3ui(location, arr);
	}
}

//*****************************************************************************
void Context::glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
	if (mShaderProgram != nullptr)
	{
		GLuint arr[4] = { v0, v1, v2, v3 };
		mShaderProgram->SetUniform4ui(location, arr);
	}
}

//*****************************************************************************
void Context::glVertexAttrib1f(GLuint index, GLfloat v0)
{
	if (mShaderProgram != nullptr)
	{
		mShaderProgram->SetAttrib1f(index, v0);
	}
}

//*****************************************************************************
void Context::glVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
{
	if (mShaderProgram != nullptr)
	{
		GLfloat arr[2] = { v0, v1 };
		mShaderProgram->SetAttrib2f(index, arr);
	}
}

//*****************************************************************************
void Context::glVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
{
	if (mShaderProgram != nullptr)
	{
		GLfloat arr[3] = { v0, v1, v2 };
		mShaderProgram->SetAttrib3f(index, arr);
	}
}

//*****************************************************************************
void Context::glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	if (mShaderProgram != nullptr)
	{
		GLfloat arr[4] = { v0, v1, v2, v3 };
		mShaderProgram->SetAttrib4f(index, arr);
	}
}

//*****************************************************************************
void Context::glGenBuffers(GLsizei n, GLuint *buffers)
{
	if (n < 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	//TODO: 感觉OpenGL内部并不会存储未使用的Buffer，因为glGenBuffers并没有超过Buffer上限的错误
	if (mBufferIds.size() < n) 
		return;

	for (int i = 0; i < n; ++i)
	{
		buffers[i] = mBufferIds.top();
		mGenBufferIds.push_back(mBufferIds.top());
		mBufferIds.pop();
	}
}

//*****************************************************************************
bool Context::glIsBuffer(GLuint buffer)
{
	auto it = mArrayBuffers.find(buffer);
	if (it != mArrayBuffers.end())
		return true;

	//TODO: 查找其他Buffer类型

	return false;
}

//*****************************************************************************
void Context::glBindBuffer(GLenum target, GLuint buffer)
{
	if (target != GL_ARRAY_BUFFER)
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	if (target == GL_ARRAY_BUFFER)
	{
		auto it = mArrayBuffers.find(buffer);
		if (it == mArrayBuffers.end())
		{
			mArrayBuffers.insert(map<GLuint, BufferObject*>::value_type(buffer, new BufferObject()));
		}
		else
		{
			mCurrentArrayBufferId = buffer;
		}
	}
}

//*****************************************************************************
//参数usage在这里实际是没用的，因为数据并没有需要传递到显存，不过为了一致性还是保留
void Context::glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
	if (target != GL_ARRAY_BUFFER)
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	if (usage != GL_STREAM_DRAW || usage != GL_STREAM_READ || usage != GL_STREAM_COPY ||
		usage != GL_STATIC_DRAW || usage != GL_STATIC_READ || usage != GL_STATIC_COPY ||
		usage != GL_DYNAMIC_DRAW || usage != GL_DYNAMIC_READ || usage != GL_DYNAMIC_COPY)
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	if (size < 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	if (target == 0)
	{
		AddError(GL_INVALID_OPERATION);
		return;
	}

	if (target == GL_ARRAY_BUFFER)
	{
		BufferObject *bufferObject = mArrayBuffers[mCurrentArrayBufferId];
		bool copyResult = bufferObject->Copy(size, data);
		if (!copyResult)
		{
			AddError(GL_OUT_OF_MEMORY);
			return;
		}
	}
}

//*****************************************************************************
void Context::glGenTextures(GLsizei n, GLuint *textures)
{
	if (n < 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	if (mTextureIds.size() < n)
		return;

	for (int i = 0; i < n; ++i)
	{
		textures[i] = mTextureIds.top();
		mGenTextureIds.push_back(mTextureIds.top());
		mTextureIds.pop();
	}
}

//*****************************************************************************
void Context::glBindTexture(GLenum target, GLuint texture)
{
	std::vector<GLenum> enums = {
		GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
	};
	if (!CheckEnum(target, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	for (auto it = mBindedTextureIds.begin(); it != mBindedTextureIds.end(); ++it)
	{
		if (it->first != target)
		{
			auto findResult = std::find(it->second.begin(), it->second.end(), texture);
			if (findResult != it->second.end())
			{
				AddError(GL_INVALID_OPERATION);
				return;
			}
		}
	}

	auto findResult = std::find(mBindedTextureIds[target].begin(), mBindedTextureIds[target].end(), texture);
	if (findResult == mBindedTextureIds[target].end())
	{
		mBindedTextureIds[target].push_back(texture);		
	}

	mBindingTextureId[target] = texture;
	mTextureUnits[mActiveTextureUnit] = texture;
}

//*****************************************************************************
void Context::glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data)
{
	std::vector<GLenum> enums = {
		GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	if (!CheckEnum(target, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	if (target != GL_TEXTURE_2D && (width != height))
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	std::vector<GLenum> typeEnums = {
		GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT, 
		GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_INT_2_10_10_10_REV, 
		GL_UNSIGNED_INT_10F_11F_11F_REV, GL_UNSIGNED_INT_5_9_9_9_REV, GL_UNSIGNED_INT_24_8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV
	};
	if (!CheckEnum(type, typeEnums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	std::vector<GLenum> internalFormatEnums = {
		GL_RGB, GL_RGBA, GL_LUMINANCE_ALPHA, GL_LUMINANCE, GL_ALPHA, GL_R8, GL_R8_SNORM, GL_R16F, GL_R32F, GL_R8UI, GL_R8I, GL_R16UI, GL_R16I,
		GL_R32UI, GL_R32I, GL_RG8, GL_RG8_SNORM, GL_RG16F, GL_RG32F, GL_RG8UI, GL_RG8I, GL_RG16UI, GL_RG16I, GL_RG32UI, GL_RG32I, GL_RGB8, GL_SRGB8,
		GL_RGB565, GL_RGB8_SNORM, GL_R11F_G11F_B10F, GL_RGB9_E5, GL_RGB16F, GL_RGB32F, GL_RGB8UI, GL_RGB8I, GL_RGB16UI, GL_RGB16I, GL_RGB32UI,
		GL_RGB32I, GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGBA8_SNORM, GL_RGB5_A1, GL_RGBA4, GL_RGB10_A2, GL_RGBA16F, GL_RGBA32F, GL_RGBA8UI, GL_RGBA8I,
		GL_RGB10_A2UI, GL_RGBA16UI, GL_RGBA16I, GL_RGBA32I, GL_RGBA32UI, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32F,
		GL_DEPTH24_STENCIL8, GL_DEPTH32F_STENCIL8
	};
	if (!CheckEnum(internalFormat, internalFormatEnums))
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	if (width < 0 || width > mMaxTextureSize || height < 0 || height > mMaxTextureSize)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	if (level < 0 || level > log2f(mMaxTextureSize))
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	if (border != 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	if (!IsUnsizedInternalFormats(internalFormat, format, type) && !IsSizedInternalFormats(internalFormat, format, type))
	{
		AddError(GL_INVALID_OPERATION);
		return;
	}
	//TODO: GL_INVALID_OPERATION 检查

	//TODO: 目前支持GL_UNSIGNED_BYTE与GL_RGB/GL_RGBA
	if (type == GL_UNSIGNED_BYTE)
	{
		Texture2D *tex2D = nullptr;
		if (format == GL_RGB)
		{
			tex2D = new Texture2D((ubyte*)data, width, height, 3);
		}
		else if (format == GL_RGBA)
		{
			tex2D = new Texture2D((ubyte*)data, width, height, 4);
		}

		if (tex2D != nullptr)
		{
			for (int i = 0; i < level; ++i)
			{
				Texture2D *mipmap = tex2D->GenMipMap();
				delete tex2D;
				tex2D = mipmap;
			}
		}

		GLuint texId = mBindingTextureId[target];
		mTexture2Ds.insert(std::make_pair(texId, tex2D));
	}
	
}

//*****************************************************************************
bool Context::glIsTexture(GLuint texture)
{
	for (auto it = mBindedTextureIds.begin(); it != mBindedTextureIds.end(); ++it)
	{
		auto findResult = std::find(it->second.begin(), it->second.end(), texture);
		if (findResult != it->second.end())
			return true;
	}

	return false;
}

//*****************************************************************************
void Context::glActiveTexture(GLenum texture)
{
	if (texture < GL_TEXTURE0 || texture >= (GL_TEXTURE0 + (GLuint)mMaxCombinedTextureUnits))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	mActiveTextureUnit = texture;
}

//*****************************************************************************
void Context::glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	std::vector<GLenum> enums =
	{
		GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
	};
	if (!CheckEnum(target, enums))
	{
		AddError(GL_INVALID_ENUM);
		return;
	}

	if (pname == GL_TEXTURE_BASE_LEVEL)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_COMPARE_FUNC)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_COMPARE_MODE)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_MIN_FILTER)
	{
		std::vector<GLenum> filterEnums = {
			GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
		};
		if (!CheckEnum(param, filterEnums))
		{
			AddError(GL_INVALID_ENUM);
			return;
		}

		if (target == GL_TEXTURE_2D)
		{
			GLuint binding = mBindingTextureId[target];
			mTexture2Ds[binding]->SetFilter(param); //TODO：删掉
			mTexture2Ds[binding]->SetMinFilter(param);
		}
		else
		{
			//TODO
		}
	}
	else if (pname == GL_TEXTURE_MAG_FILTER)
	{
		std::vector<GLenum> filterEnums = {
			GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
		};
		if (!CheckEnum(param, filterEnums))
		{
			AddError(GL_INVALID_ENUM);
			return;
		}

		if (target == GL_TEXTURE_2D)
		{
			GLuint binding = mBindingTextureId[target];
			mTexture2Ds[binding]->SetFilter(param); //TODO: 删掉
			mTexture2Ds[binding]->SetMagFilter(param);
		}
		else
		{
			//TODO
		}
	}
	else if (pname == GL_TEXTURE_MIN_LOD)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_MAX_LOD)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_MAX_LEVEL)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_SWIZZLE_R)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_SWIZZLE_G)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_SWIZZLE_B)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_SWIZZLE_A)
	{
		//TODO
	}
	else if (pname == GL_TEXTURE_WRAP_S)
	{
		std::vector<GLenum> wrapEnums = {
			GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE
		};
		if (!CheckEnum(param, wrapEnums))
		{
			AddError(GL_INVALID_ENUM);
			return;
		}

		if (target == GL_TEXTURE_2D)
		{
			GLuint binding = mBindingTextureId[target];
			mTexture2Ds[binding]->SetWrapS(param);
		}
		else
		{
			//TODO
		}
	}
	else if (pname == GL_TEXTURE_WRAP_T)
	{
		std::vector<GLenum> wrapEnums = {
			GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE
		};
		if (!CheckEnum(param, wrapEnums))
		{
			AddError(GL_INVALID_ENUM);
			return;
		}

		if (target == GL_TEXTURE_2D)
		{
			GLuint binding = mBindingTextureId[target];
			mTexture2Ds[binding]->SetWrapT(param);
		}
		else
		{
			//TODO
		}
	}
	else if (pname == GL_TEXTURE_WRAP_R)
	{
		//TODO
	}
	else
	{
		AddError(GL_INVALID_ENUM);
		return;
	}
}

//*****************************************************************************
void Context::glDeleteTextures(GLsizei n, const GLuint *textures)
{
	for (int texIndex = 0; texIndex < n; texIndex++)
	{
		GLuint id = textures[texIndex];
		if (id == 0)
			continue;

		auto unitIt = mTextureUnits.begin();
		while (unitIt != mTextureUnits.end())
		{
			if (unitIt->second == id)
			{
				mTextureUnits[unitIt->first] = 0;
				break;
			}
			unitIt++;
		}

		auto findIt = mTexture2Ds.find(id);
		if (findIt != mTexture2Ds.end())
		{
			Texture2D* tex2D = findIt->second;
			delete tex2D;
			mTexture2Ds.erase(id);
		}

		auto bindingIt = mBindingTextureId.begin();
		while (bindingIt != mBindingTextureId.end())
		{
			if (bindingIt->second == id)
			{
				mBindingTextureId[unitIt->first] = 0;
				break;
			}
			bindingIt++;
		}

		auto bindedIt = mBindedTextureIds.begin();
		while (bindedIt != mBindedTextureIds.end())
		{
			for (auto vit = bindedIt->second.begin(); vit != bindedIt->second.end(); vit++)
			{
				if (*vit == id)
				{
					bindedIt->second.erase(vit);
					break;
				}
			}
			bindedIt++;
		}

		for (auto it = mGenTextureIds.begin(); it != mGenTextureIds.end(); it++)
		{
			if (*it == id)
			{
				mGenTextureIds.erase(it);
				break;
			}
		}

		mTextureIds.push(id);
	}
}