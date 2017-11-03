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
	Camera &MainCamera() { return mMainCamera; }
	
	void SetMainCamera(Camera cam) { mMainCamera = cam; }
	void SetMainLight(Light *light) { mMainLight = light; }
	void SetCurrentShader(Shader *shader) { mCurrentShader = shader; }
	void SetCurrentTexture(Texture2D *texture) { mCurrentTexture = texture; }
	void SetVertices(std::vector<VertexIn> vertices) { mVertices = vertices; }
	void SetIndices(std::vector<int> indices) { mIndices = indices; }
	void SetWorldMat(Matrix4x4 mat);

	RenderMode GetRenderMode() const { return mRenderMode; }
	Culling GetCulling() const { return mCulling; }
	Matrix4x4 GetViewMat() { return mMainCamera.ViewMat(); }
	Matrix4x4 GetPerspectiveMat() { return mMainCamera.PerspectiveMat(); }	
	Texture2D *GetCurrentTexture() { return mCurrentTexture; }

	void Render();

private:
	RenderManager();
	~RenderManager();
	void Pipeline(const VertexIn &v0, const VertexIn &v1, const VertexIn &v2, const Matrix4x4 &vp);
	bool BackFaceCulling(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const;
	VertexOut VertexOperation(const VertexIn &appdata);
	bool Clip(const Vector4 &p) const;

private:
	static RenderManager* mInstance;
	
	RenderMode mRenderMode;
	Culling mCulling;
	Camera mMainCamera;
	Light *mMainLight;
	Shader *mCurrentShader;
	Texture2D *mCurrentTexture;

	std::vector<VertexIn> mVertices;
	std::vector<int> mIndices;

	Matrix4x4 mWorldMat;
	Matrix4x4 mITWorldMat;
	Matrix4x4 mMVP;
};