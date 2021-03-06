Not finish yet........

# Soft Renderer

A Software Renderer with API like OpenGL.



## Development Environment

System: Win10

IDE: VS2015



## Environment Configuration

C/C++ -> General -> Additional Include Directories -> Add ```.\Code```




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




## Demo List

* AlphaBlendScene
* AlphaTestScene
* DiffuseScene
* MipmapScene
* StencilTestScene
* TextureWrapScene



## TODO-List

* Concurrency
* HLSL Compiler




## Reference

[如何开始用 C++ 写一个光栅化渲染器？](https://www.zhihu.com/question/24786878/answer/29039253)

[如何写一个软件渲染器？](http://www.skywind.me/blog/archives/1498)

[Line Generation Algorithm](https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm)

[Rasterization on Larrabee](https://software.intel.com/en-us/articles/rasterization-on-larrabee)

[TGA文件格式深度解析（一）](http://blog.sina.com.cn/s/blog_814e83d801014t3m.html)

[Loading Compressed And Uncompressed TGA's](http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga%27s/22001/)

OpenGL ES 3.0 Reference Pages

《The OpenGL Graphics System: A Specification (Version 2.1 - December 1, 2006)》

《OpenGL ES Version 3.0.5 (November 3, 2016)》

《3D游戏编程大师技巧》

Muli3D
