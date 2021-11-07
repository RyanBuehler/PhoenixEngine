// ------------------------------------------------------------------------------
// File:    Scene1.h
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#pragma once
#include "IScene.h"
#include "GameObject.h"
#include "Camera.h"

class Scene1 : public IScene
{
public:

  Scene1() noexcept;
  ~Scene1() = default;
  Scene1(const Scene1&) = delete;
  Scene1& operator=(const Scene1&) = delete;
  Scene1(Scene1&&) = delete;
  Scene1& operator=(Scene1&&) = delete;

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
