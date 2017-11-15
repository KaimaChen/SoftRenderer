#include "Context.h"

Context *Context::mInstance = nullptr;
Context::CGarbo Context::mGarbo;

//*****************************************************************************
Context::Context()
{
	mRenderMode = RenderMode::Shading;
	mWorldMat = Matrix4x4::identity;
	
	for (int i = 1023; i >= 0; --i)
	{
		mBufferIds.push(i);
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
	SAFE_DELETE(mShaderProgram);
	mShaderProgram = program;
}

//*****************************************************************************
void Context::SetTexture0(Texture2D *texture)
{
	SAFE_DELETE(mTexture0);
	mTexture0 = texture;
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
	Matrix4x4 screenMat = Matrix4x4::ScreenTransform(SCREEN_WIDTH, SCREEN_HEIGHT);
	v2f0.screenPos = v2f0.clipPos * screenMat;
	v2f1.screenPos = v2f1.clipPos * screenMat;
	v2f2.screenPos = v2f2.clipPos * screenMat;

	if (mRenderMode == RenderMode::WireFrame)
		Drawing::Instance()->DrawTriangleWire(v2f0, v2f1, v2f2);
	else
		Drawing::Instance()->DrawTriangle(v2f0, v2f1, v2f2, mShaderProgram);
}

//*****************************************************************************
bool Context::CullingFace(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const
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
	case GL_STENCIL_WRITEMASK:
		*data = mStencilWriteMask;
		break;
	case GL_STENCIL_FUNC:
		*data = mStencilFunc;
		break;
	case GL_STENCIL_REF:
		*data = mStencilRef;
		break;
	case GL_STENCIL_BITS:
		*data = NUM_OF_BITS;
		break;
	case GL_STENCIL_FAIL:
		*data = mStencilFail;
		break;
	case GL_STENCIL_PASS_DEPTH_FAIL:
		*data = mStencilPassDepthFail;
		break;
	case GL_STENCIL_PASS_DEPTH_PASS:
		*data = mStencilDepthPass;
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
	default:
		cout << "Capability = " << cap << " doesn't exist !!" << endl;
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
	std::vector<GLenum> enums;
	enums.push_back(GL_ZERO);
	enums.push_back(GL_ONE);
	enums.push_back(GL_SRC_COLOR);
	enums.push_back(GL_ONE_MINUS_SRC_COLOR);
	enums.push_back(GL_DST_COLOR);
	enums.push_back(GL_ONE_MINUS_DST_COLOR);
	enums.push_back(GL_SRC_ALPHA);
	enums.push_back(GL_ONE_MINUS_SRC_ALPHA);
	enums.push_back(GL_DST_ALPHA);
	enums.push_back(GL_ONE_MINUS_DST_ALPHA);
	enums.push_back(GL_CONSTANT_COLOR);
	enums.push_back(GL_ONE_MINUS_CONSTANT_COLOR);
	enums.push_back(GL_CONSTANT_ALPHA);
	enums.push_back(GL_ONE_MINUS_CONSTANT_ALPHA);
	enums.push_back(GL_SRC_ALPHA_SATURATE);

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
	std::vector<GLenum> enums;
	enums.push_back(GL_ZERO);
	enums.push_back(GL_ONE);
	enums.push_back(GL_SRC_COLOR);
	enums.push_back(GL_ONE_MINUS_SRC_COLOR);
	enums.push_back(GL_DST_COLOR);
	enums.push_back(GL_ONE_MINUS_DST_COLOR);
	enums.push_back(GL_SRC_ALPHA);
	enums.push_back(GL_ONE_MINUS_SRC_ALPHA);
	enums.push_back(GL_DST_ALPHA);
	enums.push_back(GL_ONE_MINUS_DST_ALPHA);
	enums.push_back(GL_CONSTANT_COLOR);
	enums.push_back(GL_ONE_MINUS_CONSTANT_COLOR);
	enums.push_back(GL_CONSTANT_ALPHA);
	enums.push_back(GL_ONE_MINUS_CONSTANT_ALPHA);
	enums.push_back(GL_SRC_ALPHA_SATURATE);

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
	std::vector<GLenum> enums;
	enums.push_back(GL_FUNC_ADD);
	enums.push_back(GL_FUNC_SUBTRACT);
	enums.push_back(GL_FUNC_REVERSE_SUBTRACT);
	enums.push_back(GL_MIN);
	enums.push_back(GL_MAX);

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
	std::vector<GLenum> enums;
	enums.push_back(GL_FUNC_ADD);
	enums.push_back(GL_FUNC_SUBTRACT);
	enums.push_back(GL_FUNC_REVERSE_SUBTRACT);
	enums.push_back(GL_MIN);
	enums.push_back(GL_MAX);

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
	GLint maxValue = (int)(pow(2, NUM_OF_BITS) - 1);
	GLint value = s & maxValue;
	mStencilClearValue = value;
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