//------------------------------------------------------------------------------
// File:    ImGUIManager.h
// Author:  Ryan Buehler
// Created: 09/30/21
// Desc:    Interface to the ImGUI system
//------------------------------------------------------------------------------
#pragma once
#include "SceneManager.h"
#include "MeshRenderer.h"

#ifndef _IMGUI_ENABLED
class ImGuiManager;
namespace ImGui
{
  extern unique_ptr<ImGuiManager> Manager;
  extern bool GraphicsWindowEnabled;
}
#define _IMGUI_ENABLED
#endif

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
  void OnImGuiGraphicsUpdate(MeshRenderer& renderer) noexcept;

  void SetOnCloseHandler(function<void()> callback);
  void SetOnSceneChangeHandler(function<void(SceneManager::Scene)> callback);

private:

  function<void()> m_dOnClose;
  function<void(SceneManager::Scene)> m_dOnSceneChange;

  void ShowMainMenu() noexcept;
  void ShowMainMenu_File() noexcept;
  void ShowMainMenu_Edit() noexcept;
  void ShowMainMenu_About() noexcept;
};
