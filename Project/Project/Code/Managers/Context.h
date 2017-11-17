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
	bool IsCurrentFaceFront() const { return mIsCurrentFaceFront; }

	Matrix4x4 GetViewMat() { return mMainCamera->ViewMat(); }
	Matrix4x4 GetPerspectiveMat() { return mMainCamera->PerspectiveMat(); }
	Texture2D *GetTexture0() { return mTexture0; }
	Texture2D *GetTexture1() { return mTexture1; }
	RenderMode GetRenderMode() const { return mRenderMode; }

	void glStencilFunc(GLenum func, GLint ref, GLuint mask);
	void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
	void glStencilMask(GLuint mask);
	void glStencilMaskSeparate(GLenum face, GLuint mask);
	void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
	
	void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);

	void glDepthMask(bool flag) { mDepthWriteMask = flag; }
	void glDepthFunc(GLenum func);

	void glFrontFace(GLenum mode);
	void glCullFace(GLenum mode);

	void glBlendFunc(GLenum sfactor, GLenum dfactor);
	void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	void glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void glBlendEquation(GLenum mode);
	void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);

	void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

	//TODO: 不同值也能访问，比如GetInteger也能访问bool值，只是要转换
	void glGetBooleanv(GLenum pname, bool *data);
	void glGetIntegerv(GLenum pname, int *data);
	void glGetFloatv(GLenum pname, float *data);

	bool glIsEnabled(GLenum cap);
	void glEnable(GLenum cap);
	void glDisable(GLenum cap);
	GLenum glGetError();
	void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

	void glClear(GLbitfield mask);
	void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void glClearDepthf(GLfloat depth);
	void glClearStencil(GLint s);
	
	void glGenBuffers(GLsizei n, GLuint *buffers);
	bool glIsBuffer(GLuint buffer);
	void glBindBuffer(GLenum target, GLuint buffer);
	void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

	//void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

	void Render();

private:
	Context();
	~Context();
	void Pipeline(const VertexIn &v0, const VertexIn &v1, const VertexIn &v2, const Matrix4x4 &vp);
	bool CullingFace(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2);
	VertexOut VertexOperation(const VertexIn &appdata);
	bool Clip(const Vector4 &p) const;
	void AddError(GLenum error);
	bool CheckEnum(GLenum target, const std::vector<GLenum> &enums);

private:
	///Buffers
	std::stack<GLuint> mBufferIds; //存放未Gen的缓冲区ID
	std::vector<GLuint> mGenBufferIds; //存放已经Gen的缓冲区ID
	std::map<GLuint, BufferObject*> mArrayBuffers;
	GLuint mCurrentArrayBufferId = 0;

	///面裁剪
	bool mIsCullFaceEnabled = false;
	GLenum mCullFace = GL_BACK;
	GLenum mFrontFace = GL_CCW;

	///透明度混合
	bool mIsBlendEnabled = false;	
	GLenum mSrcRGBBlendFunc = GL_ONE;
	GLenum mSrcAlphaBlendFunc = GL_ONE;
	GLenum mDstRGBBlendFunc = GL_ZERO;
	GLenum mDstAlphaBlendFunc = GL_ZERO;
	Color mBlendColor = Color::black;
	GLenum mRGBBlendEquation = GL_FUNC_ADD;
	GLenum mAlphaBlendEquation = GL_FUNC_ADD;

	///深度测试
	bool mIsDepthTestEnabled = false;
	GLenum mDepthFunc = GL_LESS;
	bool mDepthWriteMask = true;
	float mDepthClearValue = 1;

	///Stencil Test
	GLboolean mIsStencilTestEnabled = false;					
	GLint mStencilDefault = 0;									
	GLenum mStencilFunc = GL_EQUAL;					
	GLenum mStencilBackFunc = GL_EQUAL;
	GLint mStencilRef = 1;
	GLint mStencilBackRef = 1;
	GLuint mStencilValueMask = 0xff;	
	GLuint mStencilBackValueMask = 0xff;
	GLuint mStencilWriteMask = 0xff;
	GLuint mStencilBackWriteMask = 0xff;
	GLenum mStencilFail = GL_KEEP;
	GLenum mStencilBackFail = GL_KEEP;
	GLenum mStencilPassDepthFail = GL_KEEP;
	GLenum mStencilBackPassDepthFail = GL_KEEP;
	GLenum mStencilPassDepthPass = GL_KEEP;
	GLenum mStencilBackPassDepthPass = GL_KEEP;
	GLint mStencilClearValue = 0;

	///Scissor Test
	bool mIsScissorTestEnabled = false;
	GLint mScissorBoxX = 0;
	GLint mScissorBoxY = 0;
	GLsizei mScissorBoxWidth = SCREEN_WIDTH;
	GLsizei mScissorBoxHeight = SCREEN_HEIGHT;

	///错误
	std::stack<GLenum> mErrors;

	///颜色
	Color mColorClearValue = Color::black;
	GLboolean mRedMask = GL_TRUE; //是否能写入Frame Buffer的红色分量
	GLboolean mGreenMask = GL_TRUE; //是否能写入Frame Buffer的绿色分量
	GLboolean mBlueMask = GL_TRUE; //是否能写入Frame Buffer的蓝色分量
	GLboolean mAlphaMask = GL_TRUE; //是否能写入Frame Buffer的Alpha分量

	///其他
	GLint mViewportX = 0;
	GLint mViewportY = 0;
	GLsizei mViewportWidth = SCREEN_WIDTH;
	GLsizei mViewportHeight = SCREEN_HEIGHT;

private: //settings
	GLint mMaxVertexAttribs = 16;
	GLsizei mMaxViewportDims[2];

private:
	static Context* mInstance;

	RenderMode mRenderMode;

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

	bool mIsCurrentFaceFront = true; //当前处理的三角形是否为正面
};