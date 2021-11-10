// ------------------------------------------------------------------------------
// File:    SceneSingleObject.h
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#pragma once
#include "IScene.h"
#include "GameObject.h"
#include "Camera.h"

class SceneSingleObject : public IScene
{
public:

  SceneSingleObject() noexcept;
  ~SceneSingleObject() = default;
  SceneSingleObject(const SceneSingleObject&) = delete;
  SceneSingleObject& operator=(const SceneSingleObject&) = delete;
  SceneSingleObject(SceneSingleObject&&) = delete;
  SceneSingleObject& operator=(SceneSingleObject&&) = delete;

  void OnLoad() noexcept;
  void OnInit() noexcept;
  void OnUpdate(float dt) noexcept;
  void OnShutdown() noexcept;
  void OnUnload() noexcept;

  void OnPollInput(GLFWwindow* windowPtr, float dt) noexcept;
  void OnDemoObjectChangeEvent();

  Camera& GetCurrentCamera() noexcept;

private:
  Camera& m_MainCamera;

  float m_Time;
  };
