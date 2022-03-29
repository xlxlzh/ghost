# ghost
The project is a small graphics engine for learning. So most of frameworks will be implemented as simple as posible, maybe i will reconfigure the code when most features have been finished.

## Features

- Multiple graphics API(D3D11 D3D12 Vulkan)
- PBR
- Shadow
- PostProcessing
- Particle System
- Multipe render path(DR, TBDR, FORWARD+)
- GI
- SceneManager
- Material system
- Editor, such as scene editor 
- Cross paltform

## How to build
You need to have a C++ compiler and [CMake 3.14+](http://www.cmake.org/) installed. We require the `C++14`, so you should have a C++ compiler that support `C++14` or later.



### Build samples

In order to build the samples you need using `GHOST_BUILD_EXAMPLES` flag with CMake.


     cmake -DGHOST_BUILD_EXAMPLES=ON ..


### Build Editor
In order to build the samples you need using `GHOST_BUILD_EDITOR` flag with CMake.


     cmake -DGHOST_BUILD_EDITOR=ON ..

**I have not begin to implement a editor, it's a placeholder.**