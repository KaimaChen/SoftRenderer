#pragma once

#include <vector>

#include "Misc\Settings.h"
#include "Misc\RenderState.h"
#include "Misc\Defines.h"
#include "Graphics\DataStructure\Camera.h"
#include "Graphics\DataStructure\Vertex.h"
#include "Graphics\Shader.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Drawing.h"
#include "Graphics\Texture2D.h"
#include "Graphics\DataStructure\Light.h"

class RenderManager
{
	DECLARE_SINGLETON_CG(RenderManager)

public:
	static RenderManager *Instance();

	Camera *MainCamera() { return mMainCamera; }
	Light *MainLight() { return mMainLight; }
	
	void SetMainCamera(Camera *cam) { mMainCamera = cam; }
	void SetMainLight(Light *light) { mMainLight = light; }
	void SetCurrentShader(Shader *shader) { mCurrentShader = shader; }
	void SetTexture0(Texture2D *texture) { mTexture0 = texture; }
	void SetTexture1(Texture2D *texture) { mTexture1 = texture; }
	void SetVertices(std::vector<VertexIn> vertices) { mVertices = vertices; }
	void SetIndices(std::vector<int> indices) { mIndices = indices; }
	void SetWorldMat(Matrix4x4 mat);
	void SetCullState(bool cull) { mIsCullingEnabled = cull; }
	void SetCullFace(CullFace cullFace) { mCullFace = cullFace; }
	void SetFrontFace(ClockDirection dir) { mFrontFace = dir; }
	void SetBlendState(bool isEnabled) { mIsBlendEnabled = isEnabled; }
	void SetRenderMode(RenderMode mode) { mRenderMode = mode; }

	RenderMode GetRenderMode() const { return mRenderMode; }
	bool IsCullingEnabled() const { return mIsCullingEnabled; }
	CullFace GetCullFace() const { return mCullFace; }
	ClockDirection GetFrontFace() const { return mFrontFace; }
	bool IsBlendEnabled() const { return mIsBlendEnabled; }
	Matrix4x4 GetViewMat() { return mMainCamera->ViewMat(); }
	Matrix4x4 GetPerspectiveMat() { return mMainCamera->PerspectiveMat(); }	
	Texture2D *GetTexture0() { return mTexture0; }
	Texture2D *GetTexture1() { return mTexture1; }

	void Render();

private:
	RenderManager();
	~RenderManager();
	void Pipeline(const VertexIn &v0, const VertexIn &v1, const VertexIn &v2, const Matrix4x4 &vp);
	bool CullingFace(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const;
	VertexOut VertexOperation(const VertexIn &appdata);
	bool Clip(const Vector4 &p) const;

private:
	static RenderManager* mInstance;
	
	RenderMode mRenderMode;

	bool mIsCullingEnabled;
	CullFace mCullFace;
	ClockDirection mFrontFace;

	bool mIsBlendEnabled;

	Camera *mMainCamera;
	Light *mMainLight;
	Shader *mCurrentShader;

	Texture2D *mTexture0;
	Texture2D *mTexture1;

	std::vector<VertexIn> mVertices;
	std::vector<int> mIndices;

	Matrix4x4 mWorldMat;
	Matrix4x4 mITWorldMat;
	Matrix4x4 mMVP;
};