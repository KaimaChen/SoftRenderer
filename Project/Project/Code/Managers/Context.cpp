#include "Context.h"

Context *Context::mInstance = nullptr;
Context::CGarbo Context::mGarbo;

//*****************************************************************************
Context::Context()
{
	mRenderMode = RenderMode::Shading;
	mWorldMat = Matrix4x4::identity;
}

//*****************************************************************************
Context::~Context()
{
	SAFE_DELETE(mShaderProgram);
	SAFE_DELETE(mTexture0);
	SAFE_DELETE(mTexture1);
	SAFE_DELETE(mMainLight);
	SAFE_DELETE(mMainCamera);
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