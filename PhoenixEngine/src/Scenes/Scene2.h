// ------------------------------------------------------------------------------
// File:    Scene2.h
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#pragma once
#include "IScene.h"
#include "GameObject.h"
#include "Camera.h"

class Scene2 : public IScene
{
public:

  Scene2() noexcept;
  ~Scene2() = default;
  Scene2(const Scene2&) = delete;
  Scene2& operator=(const Scene2&) = delete;
  Scene2(Scene2&&) = delete;
  Scene2& operator=(Scene2&&) = delete;

  void OnLoad() noexcept;
  void OnInit() noexcept;
  void OnUpdate(float dt) noexcept;
  void OnShutdown() noexcept;
  void OnUnload() noexcept;

  void OnPollInput(GLFWwindow* windowPtr, float dt) noexcept;
  void OnDemoObjectChangeEvent();

  Camera& GetCurrentCamera() noexcept;

  void ChangeScenario(int which) noexcept;

private:
  void Scenario1() noexcept;
  void Scenario2() noexcept;
  void Scenario3() noexcept;

private:
  Camera& m_MainCamera;

  float m_Time;

  };
