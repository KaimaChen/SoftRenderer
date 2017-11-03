#include "RenderManager.h"

RenderManager *RenderManager::mInstance = nullptr;
RenderManager::CGarbo RenderManager::mGarbo;

RenderManager::RenderManager()
{
	mRenderMode = RenderMode::Shading;
	mCulling = Culling::On;
	mWorldMat = Matrix4x4::identity;
}

RenderManager::~RenderManager()
{
	if (mCurrentShader)
	{
		delete mCurrentShader;
		mCurrentShader = nullptr;
	}
	if (mCurrentTexture)
	{
		delete mCurrentTexture;
		mCurrentTexture = nullptr;
	}
	if (mMainLight)
	{
		delete mMainLight;
		mMainLight = nullptr;
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

	if (BackFaceCulling(worldPos0, worldPos1, worldPos2) == false)
		return;

	VertexOut v2f0 = VertexOperation(v0);
	VertexOut v2f1 = VertexOperation(v1);
	VertexOut v2f2 = VertexOperation(v2);

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
	{
		LineDrawing::BresenhamDrawLine(v2f0.screenPos.x, v2f0.screenPos.y, v2f1.screenPos.x, v2f1.screenPos.y);
		LineDrawing::BresenhamDrawLine(v2f0.screenPos.x, v2f0.screenPos.y, v2f2.screenPos.x, v2f2.screenPos.y);
		LineDrawing::BresenhamDrawLine(v2f1.screenPos.x, v2f1.screenPos.y, v2f2.screenPos.x, v2f2.screenPos.y);
	}
	else
	{
		Drawing::Instance()->DrawTriangle(v2f0, v2f1, v2f2, mCurrentShader);
	}
}

bool RenderManager::BackFaceCulling(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const
{
	if (mRenderMode == RenderMode::WireFrame)
		return true;

	//ƒÊ ±’ÎÀ„’˝√Ê
	Vector4 d01 = p1 - p0;
	Vector4 d12 = p2 - p1;
	Vector4 normal = d01.Cross(d12);
	Vector4 viewDir = p0 - mMainCamera.eye;
	
	if (normal.Dot(viewDir) < 0)
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

	return v2f;
}

bool RenderManager::Clip(const Vector4 &p) const
{
	if (p.x >= -p.w && p.x <= p.w &&
		p.y >= -p.w && p.y <= p.w &&
		p.z >= 0.0f && p.z <= p.w)
		return true;
	
	return false;
}