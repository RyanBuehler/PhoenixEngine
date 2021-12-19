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
  extern unique_ptr<ImGuiManager> Manager;
  extern bool GraphicsWindowEnabled;
  extern bool GraphicsDebugRenderVertexNormals;
  extern bool GraphicsDebugRenderSurfaceNormals;
  extern float GraphicsDebugNormalLength;
  extern bool GraphicsRebuildShaders;
  extern bool GraphicsRebuildMeshes;
  extern int GraphicsSelectedShader;
  extern UV::Generation GraphicsSelectedProjection;
  extern GLuint GraphicsDisplayTexture[6];
  extern float GraphicsRefractSlider;
  extern float GraphicsRedIOR;
  extern float GraphicsGreenIOR;
  extern float GraphicsBlueIOR;
  extern bool GraphicsRefractEnabled;
  extern bool GraphicsReflectEnabled;

  extern int SceneScenario;
  extern bool SceneDrawOrbit;
  extern bool SceneOrbitObjects;
  
  extern LightingSystem::GlobalLightingData LightingGlobalData;
  extern Light::Data LightingDataArray[16];
  extern int LightingCurrentLight;
  extern int LightingActiveLights;
  extern Material LightingGlobalMaterial;

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

  extern DemoObject DemoObjectMain;
  extern const char* DemoObjectFile;
}

struct GLFWwindow;

class ImGuiManager
{
public:
  ImGuiManager(GLFWwindow* window) noexcept;
  ~ImGuiManager() = default;
  ImGuiManager(const ImGuiManager&) = delete;
  ImGuiManager& operator=(const ImGuiManager&) = delete;
  ImGuiManager(ImGuiManager&&) = delete;
  ImGuiManager& operator=(ImGuiManager&&) = delete;

  void OnImGuiUpdateStart() noexcept;
  void OnImGuiUpdateEnd() noexcept;
  void OnImGuiClose() noexcept;
  void OnImGuiGraphicsUpdate() noexcept;

  void SetOnCloseHandler(function<void()> callback);
  void SetOnSceneChangeHandler(function<void(SceneManager::Scene)> callback);
  void SetOnDemoObjectHandler(function<void()> callback);

private:
  function<void()> m_dOnClose;
  function<void(SceneManager::Scene)> m_dOnSceneChange;
  function<void()> m_dOnDemoObjectChange;
  function<void()> m_dOnRebuildShaders;

  void ShowMainMenu() noexcept;
  void ShowMainMenu_File() noexcept;
  void ShowMainMenu_Edit() noexcept;
  void ShowMainMenu_About() noexcept;

  void graphicsUpdateStats() noexcept;
  void graphicsUpdateObjects() noexcept;
  void graphicsUpdateRendering() noexcept;
  void graphicsUpdateControls() noexcept;
  void graphicsUpdateLighting() noexcept;
  void graphicsUpdateTexture() noexcept;

  bool m_bRenderAxes;
  float m_DebugLineWidth;
};
