#pragma once

#include <vector>
#include <stack>
#include <map>

#include "Misc\Settings.h"
#include "Misc\RenderState.h"
#include "Misc\Defines.h"
#include "Graphics\DataStructure\Camera.h"
#include "Graphics\Shaders\Vertex.h"
#include "Graphics\Shaders\ShaderProgram.h"
#include "Graphics\Helper\LineDrawing.h"
#include "Graphics\Drawing.h"
#include "Graphics\Textures\Texture2D.h"
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
	void SetTexture0(Texture2D *texture) { SAFE_DELETE(mTexture0); mTexture0 = texture; }
	void SetTexture1(Texture2D *texture) { mTexture1 = texture; }
	void SetVertices(std::vector<VertexIn> vertices) { mVertices = vertices; }
	void SetIndices(std::vector<int> indices) { mIndices = indices; }
	void SetWorldMat(Matrix4x4 mat);
	void SetRenderMode(RenderMode mode) { mRenderMode = mode; }
	bool IsCurrentFaceFront() const { return mIsCurrentFaceFront; }

	Texture2D *GetTexture0() { return mTexture0; }
	Texture2D *GetTexture1() { return mTexture1; }
	Matrix4x4 GetViewMat() { return mMainCamera->ViewMat(); }
	Matrix4x4 GetPerspectiveMat() { return mMainCamera->PerspectiveMat(); }
	Texture2D *GetTexture2D(int index);
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

	void glUniform1f(GLint location, GLfloat v0);
	void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
	void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void glUniform1i(GLint location, GLint v0);
	void glUniform2i(GLint location, GLint v0, GLint v1);
	void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
	void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	void glUniform1ui(GLint location, GLuint v0);
	void glUniform2ui(GLint location, GLuint v0, GLuint v1);
	void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
	void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

	void glVertexAttrib1f(GLuint index, GLfloat v0);
	void glVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1);
	void glVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2);
	void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	
	void glGenBuffers(GLsizei n, GLuint *buffers);
	bool glIsBuffer(GLuint buffer);
	void glBindBuffer(GLenum target, GLuint buffer);
	void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

	void glGenTextures(GLsizei n, GLuint *textures);
	void glBindTexture(GLenum target, GLuint texture);
	//TODO：现在只支持RGB与RGBA，类型太多了，真不好弄~_~
	void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
	bool glIsTexture(GLuint texture);
	void glActiveTexture(GLenum texture);
	//TODO: 未实现全部pname
	void glTexParameteri(GLenum target, GLenum pname, GLint param);
	void glDeleteTextures(GLsizei n, const GLuint *textures);

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
	bool IsUnsizedInternalFormats(GLenum internalFormat, GLenum format, GLenum type);
	bool IsSizedInternalFormats(GLenum internalFormat, GLenum format, GLenum type);

private:
	///Buffers
	std::stack<GLuint> mBufferIds; //存放未Gen的缓冲区ID
	std::vector<GLuint> mGenBufferIds; //存放已经Gen的缓冲区ID
	std::map<GLuint, BufferObject*> mArrayBuffers;
	GLuint mCurrentArrayBufferId = 0;

	///Textures
	std::stack<GLuint> mTextureIds;
	std::vector<GLuint> mGenTextureIds;
	std::map<GLenum, std::vector<GLuint>> mBindedTextureIds; //绑定过的texture id
	std::map<GLenum, GLuint> mBindingTextureId; //当前绑定中的texture id
	std::map<GLuint, Texture2D*> mTexture2Ds;
	GLenum mActiveTextureUnit = GL_TEXTURE0;
	std::map<GLenum, GLuint> mTextureUnits;

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
	GLint mMaxTextureSize = 2048;
	GLint mMaxCombinedTextureUnits = 32;
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