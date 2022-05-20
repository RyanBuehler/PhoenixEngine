// ------------------------------------------------------------------------------
// File:    SceneDemo.h
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for experimentation
//------------------------------------------------------------------------------
#pragma once
#include "IScene.h"
#include "Camera.h"

class SceneDemo final : public IScene
{
public:

  SceneDemo() noexcept;
  ~SceneDemo() override = default;
  SceneDemo(const SceneDemo&) = delete;
  SceneDemo& operator=(const SceneDemo&) = delete;
  SceneDemo(SceneDemo&&) = delete;
  SceneDemo& operator=(SceneDemo&&) = delete;

  void OnLoad() noexcept override;
  void OnInit() noexcept override;
  void OnUpdate(float Dt) noexcept override;
  void OnShutdown() noexcept override;
  void OnUnload() noexcept override;

  void OnPollInput(GLFWwindow* WindowPtr, float DeltaTime) noexcept override;
  void OnDemoObjectChangeEvent();

  Camera& GetCurrentCamera() noexcept override;

  void ChangeScenario(int Which) noexcept;

private:
  void Scenario1() noexcept;
  void Scenario2() noexcept;
  void Scenario3() noexcept;

  Camera& m_MainCamera;

  float m_Time;
  };
