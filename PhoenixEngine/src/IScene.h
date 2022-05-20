//------------------------------------------------------------------------------
// File:    IScene.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    Scene Interface
//------------------------------------------------------------------------------
#pragma once
#include <string>
#include "GameObject.h"
#include "CameraManager.h"
#include "GLFW/glfw3.h"

class IScene
{
public:

  IScene(string SceneName) noexcept :
    m_SceneName(std::move(SceneName))
  {}

  virtual ~IScene() = default;
  IScene(const IScene&) = delete;
  IScene& operator=(const IScene&) = delete;
  IScene(IScene&&) = delete;
  IScene& operator=(IScene&&) = delete;

  virtual void OnLoad() noexcept = 0;
  virtual void OnInit() noexcept = 0;
  virtual void OnUpdate(float DeltaTime) noexcept = 0;
  virtual void OnShutdown() noexcept = 0;
  virtual void OnUnload() noexcept = 0;

  virtual void OnPollInput(GLFWwindow* WindowPtr, float DeltaTime) noexcept = 0;

  vector<GameObject>& GetGameObjectArray() noexcept { return m_GameObjectArray; }
  [[nodiscard]] const string& GetSceneName() const noexcept { return m_SceneName; }
  virtual Camera& GetCurrentCamera() noexcept = 0;

protected:
  vector<GameObject> m_GameObjectArray;
  CameraManager m_CameraManager;

private:
  string m_SceneName;
};
