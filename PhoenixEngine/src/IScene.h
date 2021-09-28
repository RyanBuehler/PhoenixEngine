//------------------------------------------------------------------------------
// File:    IScene.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    Scene Interface
//------------------------------------------------------------------------------
#pragma once
#include <string>

class IScene
{
public:

  IScene(const char* sceneName) noexcept :
    m_SceneName(sceneName) {};
  virtual ~IScene() = default;
  IScene(const IScene&) = delete;
  IScene& operator=(const IScene&) = delete;
  IScene(IScene&&) = delete;
  IScene& operator=(IScene&&) = delete;

  virtual void OnLoad() noexcept = 0;
  virtual void OnInit() noexcept = 0;
  virtual void OnUpdate(float dt) noexcept = 0;
  virtual void OnShutdown() noexcept = 0;
  virtual void OnUnload() noexcept = 0;

private:
  std::string m_SceneName;
};
