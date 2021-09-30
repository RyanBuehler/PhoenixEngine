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
  Log::Trace("Test Scene Created.");
}

void TestScene::OnLoad() noexcept
{
  Log::Trace("Test Scene Loaded.");
  m_GameObjectArray.emplace_back("bunny.obj");
}

void TestScene::OnInit() noexcept
{
  Log::Trace("Test Scene Initialized.");
  m_GameObjectArray[0].SetPosition(vec3(0.f));
  m_MainCamera.SetTarget(&m_GameObjectArray[0].GetTransform());
}

void TestScene::OnUpdate(float dt) noexcept
{
  GameObject& go = m_GameObjectArray[0];
  go.RotateY(0.5f);
}

void TestScene::OnShutdown() noexcept
{
  Log::Trace("Test Scene Shutdown.");
}

void TestScene::OnUnload() noexcept
{
  Log::Trace("Test Scene Unloaded.");
}

void TestScene::OnPollInput(GLFWwindow* window, float dt) noexcept
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    m_MainCamera.MoveForward(1.f * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    m_MainCamera.MoveLeft(1.f * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    m_MainCamera.MoveBackward(1.f * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    m_MainCamera.MoveRight(1.f * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    m_MainCamera.MoveUp(0.5f * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    m_MainCamera.MoveDown(0.5f * dt);
  }
}

Camera& TestScene::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}
