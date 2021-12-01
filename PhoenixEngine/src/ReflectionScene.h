//------------------------------------------------------------------------------
// File:    ReflectionScene.h
// Author:  Ryan Buehler
// Created: December 1, 2021
// Desc:    A reflection test scene
//------------------------------------------------------------------------------
#pragma once
#include "IScene.h"
#include "GameObject.h"
#include "Camera.h"

class ReflectionScene : public IScene
{
public:

  ReflectionScene() noexcept;
  ~ReflectionScene() = default;
  ReflectionScene(const ReflectionScene&) = delete;
  ReflectionScene& operator=(const ReflectionScene&) = delete;
  ReflectionScene(ReflectionScene&&) = delete;
  ReflectionScene& operator=(ReflectionScene&&) = delete;

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
