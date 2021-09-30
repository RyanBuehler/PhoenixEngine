// ------------------------------------------------------------------------------
// File:    TestScene.h
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#pragma once
#include "IScene.h"
#include "GameObject.h"
#include "Camera.h"

class TestScene : public IScene
{
public:

  TestScene() noexcept;
  ~TestScene() = default;
  TestScene(const TestScene&) = delete;
  TestScene& operator=(const TestScene&) = delete;
  TestScene(TestScene&&) = delete;
  TestScene& operator=(TestScene&&) = delete;

  void OnLoad() noexcept;
  void OnInit() noexcept;
  void OnUpdate(float dt) noexcept;
  void OnShutdown() noexcept;
  void OnUnload() noexcept;

  void OnPollInput(GLFWwindow* windowPtr) noexcept;

  Camera& GetCurrentCamera() noexcept;

private:
  Camera& m_MainCamera;
  };
