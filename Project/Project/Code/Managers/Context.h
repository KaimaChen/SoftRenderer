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
	void SetTexture0(Texture2D *texture);
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

	void glStencilFunc(GLenum func, int ref, GLuint mask) { mStencilFunc = func; mStencilRef = ref; mStencilValueMask = mask; }
	void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) { mStencilFail = sfail; mStencilPassDepthFail = dpfail; mStencilDepthPass = dppass; }
	void glStencilMask(GLuint mask) { mStencilWriteMask = mask; }

	void glDepthMask(bool flag) { mDepthWriteMask = flag; }

	void glFrontFace(GLenum mode);
	void glCullFace(GLenum mode);

	void glBlendFunc(GLenum sfactor, GLenum dfactor);
	void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	void glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void glBlendEquation(GLenum mode);
	void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);

	void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

	//TODO: ��ֵͬҲ�ܷ��ʣ�����GetIntegerҲ�ܷ���boolֵ��ֻ��Ҫת��
	void glGetBooleanv(GLenum pname, bool *data);
	void glGetIntegerv(GLenum pname, int *data);
	void glGetFloatv(GLenum pname, float *data);

	bool glIsEnabled(GLenum cap);
	void glEnable(GLenum cap);
	void glDisable(GLenum cap);
	GLenum glGetError();

	void glClear(GLbitfield mask);
	void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void glClearDepthf(GLfloat depth);
	void glClearStencil(GLint s);
	
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
	bool CheckEnum(GLenum target, const std::vector<GLenum> &enums);

private:
	static Context* mInstance;
	
	RenderMode mRenderMode;

	std::stack<GLuint> mBufferIds;
	std::vector<GLuint> mGenBufferIds;

	std::map<GLuint, BufferObject*> mArrayBuffers;
	GLuint mCurrentArrayBufferId = 0;

	Color mColorClearValue = Color::black;
	float mDepthClearValue = 1;
	int mStencilClearValue = 0;

	///��ü�
	bool mIsCullFaceEnabled = false;
	GLenum mCullFace = GL_BACK;
	GLenum mFrontFace = GL_CCW;

	///͸���Ȼ��
	bool mIsBlendEnabled = false;	
	GLenum mSrcRGBBlendFunc = GL_ONE;
	GLenum mSrcAlphaBlendFunc = GL_ONE;
	GLenum mDstRGBBlendFunc = GL_ZERO;
	GLenum mDstAlphaBlendFunc = GL_ZERO;
	Color mBlendColor = Color::black;
	GLenum mRGBBlendEquation = GL_FUNC_ADD;
	GLenum mAlphaBlendEquation = GL_FUNC_ADD;

	///��Ȳ���
	bool mIsDepthTestEnabled = false;
	GLenum mDepthFunc = GL_LESS;
	bool mDepthWriteMask = true;

	///ģ�����
	bool mIsStencilTestEnabled = false;					//�Ƿ���ģ�����
	int mStencilDefault = 0;									//ģ����Ĭ�ϴ�ŵ�ֵ
	GLenum mStencilFunc = GL_EQUAL;					//ģ��ȽϷ�ʽ
	int mStencilRef = 1;											//ģ��ȽϵĲο�ֵ
	GLuint mStencilValueMask = 0xff;						//ģ��Ƚ�ʱ������
	GLuint mStencilWriteMask = 0xff;						//д��ģ��ֵʱ������Щλ���Ա�д��
	GLenum mStencilFail = GL_KEEP;						//ģ�����ʧ��
	GLenum mStencilPassDepthFail = GL_KEEP;		//ģ�����ͨ������Ȳ���ʧ��
	GLenum mStencilDepthPass = GL_KEEP;			//ģ������Ȳ��Զ�ͨ��

	///����
	std::stack<GLenum> mErrors;

	///����
	GLboolean mRedMask = GL_TRUE; //�Ƿ���д��Frame Buffer�ĺ�ɫ����
	GLboolean mGreenMask = GL_TRUE; //�Ƿ���д��Frame Buffer����ɫ����
	GLboolean mBlueMask = GL_TRUE; //�Ƿ���д��Frame Buffer����ɫ����
	GLboolean mAlphaMask = GL_TRUE; //�Ƿ���д��Frame Buffer��Alpha����

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