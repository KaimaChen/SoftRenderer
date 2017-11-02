#pragma once

#include <vector>

#include "Misc\Settings.h"
#include "Misc\RenderState.h"
#include "Graphics\DataStructure\Camera.h"
#include "Graphics\DataStructure\Vertex.h"
#include "Graphics\ShaderBase.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Drawing.h"

class RenderManager
{
public:
	static RenderManager *Instance();
	Camera &MainCamera() { return mMainCamera; }
	
	void SetMainCamera(Camera cam) { mMainCamera = cam; }
	void SetCurrentShader(ShaderBase *shader) { mCurrentShader = shader; }
	void SetVertices(std::vector<VertexIn> vertices) { mVertices = vertices; }
	void SetIndices(std::vector<int> indices) { mIndices = indices; }
	void SetWorldMat(Matrix4x4 mat) { mWorldMat = mat; }

	RenderMode GetRenderMode() const { return mRenderMode; }
	Culling GetCulling() const { return mCulling; }
	Matrix4x4 GetViewMat() { return mMainCamera.ViewMat(); }
	Matrix4x4 GetPerspectiveMat() { return mMainCamera.PerspectiveMat(); }	

	void Render();

private:
	RenderManager();
	~RenderManager();
	void Pipeline(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2, const Matrix4x4 &vp);
	bool BackFaceCulling(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const;
	VertexOut VertexOperation();
	Color FragmentOperation();
	bool Clip(const Vector4 &p) const;

private:
	static RenderManager* mInstance;
	
	RenderMode mRenderMode;
	Culling mCulling;
	Camera mMainCamera;
	ShaderBase *mCurrentShader;

	std::vector<VertexIn> mVertices;
	std::vector<int> mIndices;

	Matrix4x4 mWorldMat;
};