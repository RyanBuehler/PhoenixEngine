//------------------------------------------------------------------------------
// File:    IScene.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    Scene Interface
//------------------------------------------------------------------------------
#pragma once
#include <string>
#include "GameObject.h"

class IScene
{
public:

  IScene(const string& sceneName) noexcept :
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

  vector<GameObject>& GetGameObjectArray() noexcept { return m_GameObjectArray; }
  const string& GetSceneName() const noexcept { return m_SceneName; }

protected:
  vector<GameObject> m_GameObjectArray;

private:
  string m_SceneName;
};
