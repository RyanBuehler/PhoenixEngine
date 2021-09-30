//------------------------------------------------------------------------------
// File:    TestScene.cpp
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#include "pch.h"
#include "TestScene.h"
#include "Transform.h"

TestScene::TestScene() noexcept :
  IScene("Test Scene"),
  m_MainCamera(m_CameraManager.GetDefaultCamera())
{
  m_MainCamera.SetName("Test Scene Camera");
  Log::Trace("Test Scene Created");
}

void TestScene::OnLoad() noexcept
{
  Log::Trace("Test Scene Loaded");
  m_GameObjectArray.emplace_back("bunny.obj");
}

void TestScene::OnInit() noexcept
{
  Log::Trace("Test Scene Initialized");
}

void TestScene::OnUpdate(float dt) noexcept
{
  GameObject& go = m_GameObjectArray[0];
  go.RotateY(0.5f);
}

void TestScene::OnShutdown() noexcept
{
  Log::Trace("Test Scene Shutdown");
}

void TestScene::OnUnload() noexcept
{
  Log::Trace("Test Scene Unloaded");
}

void TestScene::OnPollInput(GLFWwindow* window) noexcept
{
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
  {
    Log::Trace("Test Scene: Key 1 pressed.");
  }
}

Camera& TestScene::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}
