![](https://github.com/xlxlzh/ghost/raw/master/Images/logo.jpg)
# GHOST

This project is a personal learning sandbox for **computer graphics** and **modern C++**. The architecture is intentionally kept simple and minimal — the goal is to understand how things work under the hood, not to ship a production engine.

- **Graphics APIs**: Covers as many low-level graphics APIs as possible (D3D11, D3D12, Vulkan, etc.).
- **Modern C++**: Uses the latest C++ standards and language features wherever appropriate to explore new techniques and best practices.
- **Simplicity first**: Frameworks and systems are implemented in the simplest way that works. Refactoring may happen as features grow.

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
You can use the script **Build.py** to generate the project files and build the project. In order to run the script, you need to instanll some tools.
|Tools|Version|Platform|
|:-----:|:------:|:-----:|
|CMake|3.14 and later|All|
|Visual Studio|2019 and later|Windows|
|Python|3.10 and later|All|
|TODO|TODO|TODO|

You can directly use the **Build.py**, and you can also run the batch command files on Windows platform.