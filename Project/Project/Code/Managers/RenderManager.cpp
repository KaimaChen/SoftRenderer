#include "RenderManager.h"

RenderManager *RenderManager::mInstance = nullptr;

RenderManager::RenderManager()
{
	mRenderMode = RenderMode::Shading;
	mCulling = Culling::On;
	mWorldMat = Matrix4x4::identity;
}

RenderManager::~RenderManager()
{
	delete mCurrentShader;
	mCurrentShader = nullptr;
}

RenderManager *RenderManager::Instance()
{
	if (mInstance == nullptr)
		mInstance = new RenderManager();
	return mInstance;
}

void RenderManager::Render()
{
	Vector4 p0, p1, p2;
	Matrix4x4 vp = GetViewMat() * GetPerspectiveMat();
	
	for (int i = 0; i < mIndices.size(); i += 3)
	{
		p0 = mVertices[mIndices[i]].position;
		p1 = mVertices[mIndices[i + 1]].position;
		p2 = mVertices[mIndices[i + 2]].position;

		Pipeline(p0, p1, p2, vp);
	}
}

void RenderManager::Pipeline(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2, const Matrix4x4 &vp)
{
	Vector4 worldPos0, worldPos1, worldPos2;
	worldPos0 = p0 * mWorldMat;
	worldPos1 = p1 * mWorldMat;
	worldPos2 = p2 * mWorldMat;

	if (BackFaceCulling(worldPos0, worldPos1, worldPos2) == false)
		return;

	Vector4 clipPos0, clipPos1, clipPos2;
	clipPos0 = worldPos0 * vp;
	clipPos1 = worldPos1 * vp;
	clipPos2 = worldPos2 * vp;

	clipPos0 /= clipPos0.w;
	clipPos1 /= clipPos1.w;
	clipPos2 /= clipPos2.w;

	Vector4 screenPos0, screenPos1, screenPos2;
	Matrix4x4 screenMat = Matrix4x4::ScreenTransform(SCREEN_WIDTH, SCREEN_HEIGHT);
	screenPos0 = clipPos0 * screenMat;
	screenPos1 = clipPos1 * screenMat;
	screenPos2 = clipPos2 * screenMat;

	if (mRenderMode == RenderMode::WireFrame)
	{
		LineDrawing::BresenhamDrawLine(screenPos0.x, screenPos0.y, screenPos1.x, screenPos1.y);
		LineDrawing::BresenhamDrawLine(screenPos0.x, screenPos0.y, screenPos2.x, screenPos2.y);
		LineDrawing::BresenhamDrawLine(screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y);
	}
	else
	{
		Drawing::Instance()->DrawTriangle(screenPos0, screenPos1, screenPos2, Color::red);
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