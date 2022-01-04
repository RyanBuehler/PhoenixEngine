# Phoenix Engine - *Ryan Buehler's Independent Rendering Engine*

The Phoenix Engine is a humble rendering engine and wrapper I utilize as a professional development project to explore, learn, experiment and present as code example to interested parties. It is a Work In Progress and will be continuously evolving.

OpenGL accessed through [GLEW](http://glew.sourceforge.net/) and [GLFW](https://www.glfw.org/) <br>
UI Windows built with [DearImGui](https://github.com/ocornut/imgui) <br>
Logging built with [spdlog](https://github.com/gabime/spdlog) <br>

## Currently Implemented Features
- [x] Reflection
- [x] Refraction
- [x] Fresnel Effect (Reflect/Refract)
- [x] Cube Map Environmental Mapping
- [x] Cube Map UV Projection
- [X] Texture Loading/Mapping
- [X] Phong/Blinn-Phong lighting with custom materials
- [x] Context Manager allows for easy switching of a single render style
- [x] ImGui integration
- [x] Custom loader for .obj files
- [x] Normal Generation
- [x] GameObject class
- [x] Scene and Scene Manager

## Short Term Roadmap
* Refactor of MVP code
* Deferred Rendering
* SSAO
* Environment Mapping

## Long Term Roadmap
* BRDF Shading
* Post Processing
* 2D Sprite Renderer (GUI)
* Animation
* PBR
