#include "Context.h"

Context *Context::mInstance = nullptr;
Context::CGarbo Context::mGarbo;

Context::Context()
{
	mRenderMode = RenderMode::Shading;
	mWorldMat = Matrix4x4::identity;
	
	for (int i = 1023; i >= 0; --i)
	{
		mBufferIds.push(i);
	}
}

Context::~Context()
{
	if (mShaderProgram)
	{
		delete mShaderProgram;
		mShaderProgram = nullptr;
	}
	if (mTexture0)
	{
		delete mTexture0;
		mTexture0 = nullptr;
	}
	if (mMainLight)
	{
		delete mMainLight;
		mMainLight = nullptr;
	}
	if (mMainCamera)
	{
		delete mMainCamera;
		mMainCamera = nullptr;
	}

	auto it = mArrayBuffers.begin();
	while (it != mArrayBuffers.end())
	{
		delete it->second;
		it->second = nullptr;
		mArrayBuffers.erase(it++);
	}
}

Context *Context::Instance()
{
	if (mInstance == nullptr)
		mInstance = new Context();
	return mInstance;
}

void Context::SetWorldMat(Matrix4x4 worldMat)
{
	mWorldMat = worldMat;
	mITWorldMat = worldMat.InverseTranspose();
}

void Context::SetShaderProgram(ShaderProgram *program)
{
	if (mShaderProgram)
	{
		delete mShaderProgram;
		mShaderProgram = nullptr;
	}

	mShaderProgram = program;
}

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

	//3D�ü��������˵�Ƚ��鷳�������ڹ�դ���׶ν��вü�
	//�Ż�������ֻҪ��һ��������������������ζ����ˣ���ȷ�������ǽ�ȡ��һ������Σ�Ȼ������һ������������
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

bool Context::CullingFace(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const
{
	if (mRenderMode == RenderMode::WireFrame)
		return true;

	if (mIsCullFaceEnabled == false)
		return true;

	if (mCullFace == GL_FRONT_AND_BACK)
		return false;

	int dir = (mFrontFace == GL_CCW) ? 1 : -1; //����Ķ���˳��
	Vector4 d01 = p1 - p0;
	Vector4 d12 = p2 - p1;
	Vector4 normal = d01.Cross(d12) * dir;
	Vector4 viewDir = p0 - mMainCamera->eye;
	
	int judgement = (mCullFace == GL_BACK) ? 1 : -1; //�޳��������

	if (normal.Dot(viewDir) * judgement < 0)
		return true;

	return false;
}

VertexOut Context::VertexOperation(const VertexIn &appdata)
{
	if (mShaderProgram == nullptr)
	{
		cout << "No shader program has been set" << endl;
		return VertexOut();
	}

	mShaderProgram->SetWorldMat(mWorldMat);
	mShaderProgram->SetITWorldMat(mITWorldMat);
	mShaderProgram->SetMVP(mMVP);
	mShaderProgram->SetLight(*mMainLight);

	VertexOut v2f = mShaderProgram->ExecuteVertexShader(appdata);
	v2f.BeginPerspectiveCorrectInterpolation();

	return v2f;
}

//TODO
bool Context::Clip(const Vector4 &p) const
{
	if (/*p.x >= -p.w && p.x <= p.w &&
		p.y >= -p.w && p.y <= p.w &&*/
		p.z >= 0.0f && p.z <= p.w)
		return true;
	
	return false;
}

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
	default:
		AddError(GL_INVALID_ENUM);
		break;
	}
}

void Context::glGetBooleanv(GLenum pname, bool *data)
{
	switch (pname)
	{
	case GL_DEPTH_WRITEMASK:
		*data = mDepthWriteMask;
		break;
	default:
		AddError(GL_INVALID_ENUM);
		break;
	}
}

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
	default:
		AddError(GL_INVALID_ENUM);
		break;
	}
}

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

GLenum Context::glGetError()
{
	if (mErrors.size() == 0)
		return GL_NO_ERROR;

	GLenum error = mErrors.top();
	mErrors.pop();
	return error;
}

void Context::AddError(GLenum error)
{
	if (error != GL_INVALID_ENUM || error != GL_INVALID_VALUE || error != GL_INVALID_OPERATION ||
		error != GL_INVALID_FRAMEBUFFER_OPERATION || error != GL_OUT_OF_MEMORY)
		return;

	mErrors.push(error);
}

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

void Context::glGenBuffers(GLsizei n, GLuint *buffers)
{
	if (n < 0)
	{
		AddError(GL_INVALID_VALUE);
		return;
	}

	//TODO: �о�OpenGL�ڲ�������洢δʹ�õ�Buffer����ΪglGenBuffers��û�г���Buffer���޵Ĵ���
	if (mBufferIds.size() < n) 
		return;

	for (int i = 0; i < n; ++i)
	{
		buffers[i] = mBufferIds.top();
		mGenBufferIds.push_back(mBufferIds.top());
		mBufferIds.pop();
	}
}

bool Context::glIsBuffer(GLuint buffer)
{
	auto it = mArrayBuffers.find(buffer);
	if (it != mArrayBuffers.end())
		return true;

	//TODO: ��������Buffer����

	return false;
}

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

//����usage������ʵ����û�õģ���Ϊ���ݲ�û����Ҫ���ݵ��Դ棬����Ϊ��һ���Ի��Ǳ���
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