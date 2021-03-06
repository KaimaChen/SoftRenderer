描述：
API设计为OpenGL类似的
渲染状态都存放在Context
坐标系为右手系，如下: 
		^ y
		 |
		 ------> x
	   /
	 /
	z


功能点：
* 线框模式与着色模式
* Z缓冲：可以设置比较方式（即glDepthFunc）
* 模板测试：支持glStencilFunc, glStencilOp的功能（但不支持Separate版的，因为我用不上）
* C++类形式的Vertex与Fragment Shader（参考Demo/Shaders/*）
* 逐像素光照（参考DiffuseShader）
* 贴图
  * 过滤模式：Nearest, Linear
  * 环绕(Wrap)：Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
* 透明度测试（使用Shader.h中定义的discard）
* 透明度混合
* 面剔除：正面、背面、双面
  * 可以设置CW或CCW为正面顶点顺序
* Mipmap
* ​

TODO list：
* 改用智能指针
* VAO, VBO, EBO -> 与Shader的数据交互 -> Shader编译
* 使用多线程和SIMD提高性能
* 可以自定义顶点着色器输出结构而不是写死哪些部分可以插值
* 多边形剪裁
* fbx导入
* 场景管理
* 场景切割
* DSL
* 片元的dxy
* glEnable那一系列功能
* MSAA
* 天空盒子
* Anisotropic Filtering
* 全体基类，去掉new/delete
* Triangle fan / strip
* Quaternion
* float-point depth buffer & fixed-point depth buffer
* 读取贴图时怎么判断是放大(MagFiler)还是缩小(MinFilter)
* Vector与fvec融合

Bug:
* 当两个三角形共边时，渲染谁的？（比如两个面共边，一个面看不到，但是这条共边渲染了这个看不见的面） —— 左上填充规则？
* 边缘会有白点闪烁
* 距离稍微远点时有颗粒闪烁的感觉(mipmap?)

Some Done Bug:
* Blend时部分地方的颜色错误 —— 因为没有对Shader计算出来的颜色进行Clamp，导致有些分量的值超过1
