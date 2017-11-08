#include "RenderManager.h"

RenderManager *RenderManager::mInstance = nullptr;
RenderManager::CGarbo RenderManager::mGarbo;

RenderManager::RenderManager()
{
	mRenderMode = RenderMode::Shading;
	mWorldMat = Matrix4x4::identity;
}

RenderManager::~RenderManager()
{
	if (mCurrentShader)
	{
		delete mCurrentShader;
		mCurrentShader = nullptr;
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
}

RenderManager *RenderManager::Instance()
{
	if (mInstance == nullptr)
		mInstance = new RenderManager();
	return mInstance;
}

void RenderManager::SetWorldMat(Matrix4x4 worldMat)
{
	mWorldMat = worldMat;
	mITWorldMat = worldMat.InverseTranspose();
}

void RenderManager::Render()
{
	if (mCurrentShader == nullptr)
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

void RenderManager::Pipeline(const VertexIn &v0, const VertexIn &v1, const VertexIn &v2, const Matrix4x4 &vp)
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

	VertexOut v2f0 = VertexOperation(v0);
	VertexOut v2f1 = VertexOperation(v1);
	VertexOut v2f2 = VertexOperation(v2);

	//3D裁剪，相对来说比较麻烦，建议在光栅化阶段进行裁剪
	//优化：这里只要有一个点在外面就整个三角形丢掉了，更好的做法是截取出一个多边形，然后生成一个或多个三角形
	if (!Clip(v2f0.clipPos) || !Clip(v2f1.clipPos) || !Clip(v2f2.clipPos))
		return;

	v2f0.clipPos /= v2f0.clipPos.w;
	v2f1.clipPos /= v2f1.clipPos.w;
	v2f2.clipPos /= v2f2.clipPos.w;

	Matrix4x4 screenMat = Matrix4x4::ScreenTransform(SCREEN_WIDTH, SCREEN_HEIGHT);
	v2f0.screenPos = v2f0.clipPos * screenMat;
	v2f1.screenPos = v2f1.clipPos * screenMat;
	v2f2.screenPos = v2f2.clipPos * screenMat;

	if (mRenderMode == RenderMode::WireFrame)
		Drawing::Instance()->DrawTriangleWire(v2f0, v2f1, v2f2);
	else
		Drawing::Instance()->DrawTriangle(v2f0, v2f1, v2f2, mCurrentShader);
}

bool RenderManager::CullingFace(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const
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

VertexOut RenderManager::VertexOperation(const VertexIn &appdata)
{
	if (mCurrentShader == nullptr)
	{
		cout << "No shader has been set" << endl;
		return VertexOut();
	}

	mCurrentShader->SetWorldMat(mWorldMat);
	mCurrentShader->SetITWorldMat(mITWorldMat);
	mCurrentShader->SetMVP(mMVP);
	mCurrentShader->SetLight(*mMainLight);

	VertexOut v2f = mCurrentShader->VertexShader(appdata);
	v2f.BeginPerspectiveCorrectInterpolation();

	return v2f;
}

//TODO
bool RenderManager::Clip(const Vector4 &p) const
{
	if (/*p.x >= -p.w && p.x <= p.w &&
		p.y >= -p.w && p.y <= p.w &&*/
		p.z >= 0.0f && p.z <= p.w)
		return true;
	
	return false;
}

void RenderManager::glGetIntegerv(GLenum pname, int *data)
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

void RenderManager::glGetBooleanv(GLenum pname, bool *data)
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

void RenderManager::glGetFloatv(GLenum pname, float *data)
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

bool RenderManager::glIsEnabled(GLenum cap)
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

void RenderManager::glEnable(GLenum cap)
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

void RenderManager::glDisable(GLenum cap)
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

void RenderManager::glFrontFace(GLenum mode)
{
	if (mode != GL_CW || mode != GL_CCW)
	{
		cout << "FrontFace only accept GL_CW or GL_CCW" << endl;
		AddError(GL_INVALID_ENUM);
		return;
	}

	mFrontFace = mode;
}

void RenderManager::glCullFace(GLenum mode)
{
	if (mode != GL_FRONT || mode != GL_BACK || mode != GL_FRONT_AND_BACK)
	{
		cout << "CullFace only accept GL_FRONT or GL_BACK or GL_FRONT_AND_BACK" << endl;
		AddError(GL_INVALID_ENUM);
		return;
	}

	mCullFace = mode;
}

GLenum RenderManager::glGetError()
{
	if (mErrors.size() == 0)
		return GL_NO_ERROR;

	GLenum error = mErrors.top();
	mErrors.pop();
	return error;
}

void RenderManager::AddError(GLenum error)
{
	if (error != GL_INVALID_ENUM || error != GL_INVALID_VALUE || error != GL_INVALID_OPERATION ||
		error != GL_INVALID_FRAMEBUFFER_OPERATION || error != GL_OUT_OF_MEMORY)
		return;

	mErrors.push(error);
}

void RenderManager::glClear(GLbitfield mask)
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