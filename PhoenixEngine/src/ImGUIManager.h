//------------------------------------------------------------------------------
// File:    ImGUIManager.h
// Author:  Ryan Buehler
// Created: 09/30/21
// Desc:    Interface to the ImGUI system
//------------------------------------------------------------------------------
#pragma once
#include "SceneManager.h"
#include "Renderer.h"
#include "LightingSystem.h"
#include "Light.h"
#include "Material.h"

class ImGuiManager;
namespace ImGui
{
  extern unique_ptr<ImGuiManager> MANAGER;
  extern bool GRAPHICS_WINDOW_ENABLED;
  extern bool GRAPHICS_DEBUG_RENDER_VERTEX_NORMALS;
  extern bool GRAPHICS_DEBUG_RENDER_SURFACE_NORMALS;
  extern float GRAPHICS_DEBUG_NORMAL_LENGTH;
  extern float GRAPHICS_FPS;
  extern bool GRAPHICS_REBUILD_SHADERS;
  extern bool GRAPHICS_REBUILD_MESHES;
  extern int GRAPHICS_SELECTED_SHADER;
  extern UV::Generation GRAPHICS_SELECTED_PROJECTION;
  extern GLuint GRAPHICS_DISPLAY_TEXTURE[6];
  extern float GraphicsRefractSlider;
  extern float GraphicsRedIOR;
  extern float GraphicsGreenIOR;
  extern float GraphicsBlueIOR;
  extern bool GraphicsRefractEnabled;
  extern bool GraphicsReflectEnabled;

  extern int SCENE_SCENARIO;
  extern bool SCENE_DRAW_ORBIT;
  extern bool SCENE_ORBIT_OBJECTS;
  
  extern LightingSystem::GlobalLightingData LIGHTING_GLOBAL_DATA;
  extern Light::Data LIGHTING_DATA_ARRAY[16];
  extern int LIGHTING_CURRENT_LIGHT;
  extern int LIGHTING_ACTIVE_LIGHTS;
  extern Material LIGHTING_GLOBAL_MATERIAL;

  enum class DemoObject
  {
    Bunny,
    BunnyHighPoly,
    //Cube,
    Cube2,
    Cup,
    Lucy,
    Quad,
    Sphere,
    StarWars,
    COUNT
  };

  extern DemoObject DEMO_OBJECT_MAIN;
  extern const char* DEMO_OBJECT_FILE;
}

struct GLFWwindow;

class ImGuiManager
{
public:
  ImGuiManager(GLFWwindow* Window) noexcept;
  ~ImGuiManager() = default;
  ImGuiManager(const ImGuiManager&) = delete;
  ImGuiManager& operator=(const ImGuiManager&) = delete;
  ImGuiManager(ImGuiManager&&) = delete;
  ImGuiManager& operator=(ImGuiManager&&) = delete;

  void OnImGuiUpdateStart() noexcept;
  static void OnImGuiUpdateEnd() noexcept;
  static void OnImGuiClose() noexcept;
  void OnImGuiGraphicsUpdate() noexcept;

  void SetOnCloseHandler(function<void()> Callback);
  void SetOnSceneChangeHandler(function<void(SceneManager::Scene)> Callback);
  void SetOnDemoObjectHandler(function<void()> Callback);

private:
  function<void()> m_dOnClose;
  function<void(SceneManager::Scene)> m_dOnSceneChange;
  function<void()> m_dOnDemoObjectChange;
  function<void()> m_dOnRebuildShaders;

  void ShowMainMenu() noexcept;
  void ShowMainMenu_File() const noexcept;
  void ShowMainMenu_Edit() const noexcept;
  void ShowMainMenu_About() const noexcept;

  void graphicsUpdateStats() const noexcept;
  void graphicsUpdateObjects() const noexcept;
  void graphicsUpdateRendering() noexcept;
  static void graphicsUpdateControls() noexcept;
  void graphicsUpdateLighting() const noexcept;
  static void graphicsUpdateTexture() noexcept;

  bool m_bRenderAxes;
  float m_DebugLineWidth;
};
