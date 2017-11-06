# Soft Renderer



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
* Alpha Test (Use discard defined in Shader.h)
* Alpha Blend
* Face Culling: Front, Back, FrontAndBack
  * Set Front Face as CW or CCW