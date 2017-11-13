# Soft Renderer

A Software Renderer with API like OpenGL.



## Development Environment

System: Win10

IDE: VS2015



## Environment Configuration

C/C++ -> General -> Additional Include Directories -> Add ```.\Code```



## Controls

WSAD or Arrow Keys: Controls Camera moving.

Mouse Middle: Print current pixel information(color, depth).



## Function Point

* Wire Frame and Shading
* Vertex Shader and Fragment Shader in C++ class type (See "Demo/Shaders/\*")
* Per-Fragment Lighting (See "Demo/Shaders/DiffuseShader.h")
* Texture
  * Filter: Nearest, Linear
  * Wrap: Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
* Depth Test
* Stencil Test
* Alpha Test (Use discard defined in Shader.h)
* Alpha Blend
* Face Culling: Front, Back, FrontAndBack
  * Set Front Face as CW or CCW



## Reference

[如何开始用 C++ 写一个光栅化渲染器？](https://www.zhihu.com/question/24786878/answer/29039253)

[开源一个简单的c++软光栅渲染器](http://www.cnblogs.com/zhangbaochong/p/5751111.html)

[纹理映射的双线性插值滤波](http://dev.gameres.com/Program/Visual/3D/Bilinear.htm)

[用C#实现一个简易的软件光栅化渲染器](http://blog.csdn.net/aceyan0718/article/details/51659381)

[如何写一个软件渲染器？](http://www.skywind.me/blog/archives/1498)

[Line Generation Algorithm](https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm)

[Rasterization on Larrabee](https://software.intel.com/en-us/articles/rasterization-on-larrabee)

OpenGL ES 3.0 Reference Pages

《The OpenGL Graphics System: A Specification (Version 2.1 - December 1, 2006)》

《3D游戏编程大师技巧》

Muli3D