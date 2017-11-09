#pragma once

#include <vector>
#include <stack>
#include <map>

#include "Misc\Settings.h"
#include "Misc\RenderState.h"
#include "Misc\Defines.h"
#include "Graphics\DataStructure\Camera.h"
#include "Graphics\DataStructure\Vertex.h"
#include "Graphics\ShaderProgram.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Drawing.h"
#include "Graphics\Texture2D.h"
#include "Graphics\DataStructure\Light.h"
#include "Graphics\DataStructure\BufferObject.h"

class Context
{
	DECLARE_SINGLETON_CG(Context)

public:
	static Context *Instance();

	Camera *MainCamera() { return mMainCamera; }
	Light *MainLight() { return mMainLight; }

	void SetMainCamera(Camera *cam) { mMainCamera = cam; }
	void SetMainLight(Light *light) { mMainLight = light; }
	void SetShaderProgram(ShaderProgram *program);
	void SetTexture0(Texture2D *texture) { mTexture0 = texture; }
	void SetTexture1(Texture2D *texture) { mTexture1 = texture; }
	void SetVertices(std::vector<VertexIn> vertices) { mVertices = vertices; }
	void SetIndices(std::vector<int> indices) { mIndices = indices; }
	void SetWorldMat(Matrix4x4 mat);
	void SetRenderMode(RenderMode mode) { mRenderMode = mode; }
	void SetDepthFunc(GLenum func) { mDepthFunc = func; }

	Matrix4x4 GetViewMat() { return mMainCamera->ViewMat(); }
	Matrix4x4 GetPerspectiveMat() { return mMainCamera->PerspectiveMat(); }
	Texture2D *GetTexture0() { return mTexture0; }
	Texture2D *GetTexture1() { return mTexture1; }
	RenderMode GetRenderMode() const { return mRenderMode; }
	GLenum GetDepthFunc() const { return mDepthFunc; }
	GLenum GetStencilFunc() const { return mStencilFunc; }

	void glStencilFunc(GLenum func, int ref, uint mask) { mStencilFunc = func; mStencilRef = ref; mStencilValueMask = mask; }
	void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) { mStencilFail = sfail; mStencilPassDepthFail = dpfail; mStencilDepthPass = dppass; }
	void glStencilMask(uint mask) { mStencilWriteMask = mask; }

	void glDepthMask(bool flag) { mDepthWriteMask = flag; }

	void glFrontFace(GLenum mode);
	void glCullFace(GLenum mode);

	//TODO: ��ֵͬҲ�ܷ��ʣ�����GetIntegerҲ�ܷ���boolֵ��ֻ��Ҫת��
	void glGetBooleanv(GLenum pname, bool *data);
	void glGetIntegerv(GLenum pname, int *data);
	void glGetFloatv(GLenum pname, float *data);

	bool glIsEnabled(GLenum cap);
	void glEnable(GLenum cap);
	void glDisable(GLenum cap);
	GLenum glGetError();
	void glClear(GLbitfield mask);
	
	void glGenBuffers(GLsizei n, GLuint *buffers);
	bool glIsBuffer(GLuint buffer);
	void glBindBuffer(GLenum target, GLuint buffer);
	void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

	void Render();

private:
	Context();
	~Context();
	void Pipeline(const VertexIn &v0, const VertexIn &v1, const VertexIn &v2, const Matrix4x4 &vp);
	bool CullingFace(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2) const;
	VertexOut VertexOperation(const VertexIn &appdata);
	bool Clip(const Vector4 &p) const;
	void AddError(GLenum error);

private:
	static Context* mInstance;
	
	RenderMode mRenderMode;

	std::stack<uint> mBufferIds;
	std::vector<uint> mGenBufferIds;

	std::map<uint, BufferObject*> mArrayBuffers;
	uint mCurrentArrayBufferId = 0;

	Color mColorClearValue = Color::black;
	float mDepthClearValue = 1;
	int mStencilClearValue = 0;

	///��ü�
	bool mIsCullFaceEnabled = false;
	GLenum mCullFace = GL_BACK;
	GLenum mFrontFace = GL_CCW;

	///͸���Ȼ��
	bool mIsBlendEnabled = false;	

	///��Ȳ���
	bool mIsDepthTestEnabled = false;
	GLenum mDepthFunc = GL_LESS;
	bool mDepthWriteMask = true;

	///ģ�����
	bool mIsStencilTestEnabled = false;					//�Ƿ���ģ�����
	int mStencilDefault = 0;									//ģ����Ĭ�ϴ�ŵ�ֵ
	GLenum mStencilFunc = GL_EQUAL;					//ģ��ȽϷ�ʽ
	int mStencilRef = 1;											//ģ��ȽϵĲο�ֵ
	uint mStencilValueMask = 0xff;							//ģ��Ƚ�ʱ������
	uint mStencilWriteMask = 0xff;							//д��ģ��ֵʱ������Щλ���Ա�д��
	GLenum mStencilFail = GL_KEEP;						//ģ�����ʧ��
	GLenum mStencilPassDepthFail = GL_KEEP;		//ģ�����ͨ������Ȳ���ʧ��
	GLenum mStencilDepthPass = GL_KEEP;			//ģ������Ȳ��Զ�ͨ��

	///����
	std::stack<GLenum> mErrors;

	Camera *mMainCamera;
	Light *mMainLight;
	ShaderProgram *mShaderProgram;

	Texture2D *mTexture0;
	Texture2D *mTexture1;

	std::vector<VertexIn> mVertices;
	std::vector<int> mIndices;

	Matrix4x4 mWorldMat;
	Matrix4x4 mITWorldMat;
	Matrix4x4 mMVP;
};