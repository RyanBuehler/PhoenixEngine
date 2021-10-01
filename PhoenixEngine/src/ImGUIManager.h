//------------------------------------------------------------------------------
// File:    ImGUIManager.h
// Author:  Ryan Buehler
// Created: 09/30/21
// Desc:    Interface to the ImGUI system
//------------------------------------------------------------------------------
#pragma once

struct GLFWwindow;

class ImGUIManager
{
public:

  ImGUIManager(GLFWwindow* window) noexcept;
  ~ImGUIManager() = default;
  ImGUIManager(const ImGUIManager&) = delete;
  ImGUIManager& operator=(const ImGUIManager&) = delete;
  ImGUIManager(ImGUIManager&&) = delete;
  ImGUIManager& operator=(ImGUIManager&&) = delete;

  void OnUpdate() noexcept;
  void OnClose() noexcept;

private:

};
