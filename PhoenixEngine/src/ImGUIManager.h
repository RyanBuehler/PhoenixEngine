//------------------------------------------------------------------------------
// File:    ImGUIManager.h
// Author:  Ryan Buehler
// Created: 09/30/21
// Desc:    Interface to the ImGUI system
//------------------------------------------------------------------------------
#pragma once
#include "SceneManager.h"
#include "Renderer.h"

class ImGuiManager;
namespace ImGui
{
  extern unique_ptr<ImGuiManager> Manager;
  extern bool GraphicsWindowEnabled;
  extern bool GraphicsDebugRenderVertexNormals;
  extern bool GraphicsDebugRenderSurfaceNormals;
  extern float GraphicsDebugNormalLength;

  //TODO:
  enum class DemoObject
  {
    Bunny,
    BunnyHighPoly,
    Cube,
    Cube2,
    Cup,
    Lucy,
    Quad,
    Sphere,
    StarWars,
    Triangle
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

  void ShowMainMenu() noexcept;
  void ShowMainMenu_File() noexcept;
  void ShowMainMenu_Edit() noexcept;
  void ShowMainMenu_About() noexcept;

  bool m_bRenderAxes;
  float m_DebugLineWidth;
};
