//------------------------------------------------------------------------------
// File:    TestScene.cpp
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#include "pch.h"
#include "TestScene.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "DebugRenderer.h"

TestScene::TestScene() noexcept :
  IScene("Test Scene"),
  m_MainCamera(m_CameraManager.GetDefaultCamera()),
  m_Time(0.f)
{
  m_MainCamera.SetName("Test Scene Camera");
  Log::Trace("Test Scene Created.");
}

void TestScene::OnLoad() noexcept
{
  Log::Trace("Test Scene Loaded.");
  for (int i = 0; i < 8; ++i)
  {
    m_GameObjectArray.emplace_back("sphere");
  }
  m_GameObjectArray.emplace_back("bunny.obj");
}

void TestScene::OnInit() noexcept
{
  Log::Trace("Test Scene Initialized.");

  for (int i = 0; i < 8; ++i)
  {
    m_GameObjectArray[i].SetPosition(vec3(1.f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(360.f / 8.f * i, vec3(0.f, 0.f, 1.f));
    //TODO: 0.1f?
    m_GameObjectArray[i].ScaleBy(0.1f);
  }

  GameObject temp("sphere");
  temp.SetPosition(vec3(1.f, 0.f, 0.f));
  for (int i = 2; i <= 360; i += 2)
  {
    vec3 pt1 = temp.GetPosition();
    temp.RotateAround(glm::radians(static_cast<float>(i)), vec3(0.f, 0.f, 1.f));
    vec3 pt2 = temp.GetPosition();
    DebugRenderer::I().AddPermanentLine(pt1, pt2);
  }
  //DebugRenderer::I().AddPermanentLine(temp.GetPosition(), vec3(1.f, 0.f, 0.f));

  m_GameObjectArray[8].SetPosition(vec3(0.f));
  m_GameObjectArray[8].ScaleBy(2.f);
  m_MainCamera.SetTarget(&m_GameObjectArray[8].GetTransform());

  m_Time = 0.f;
}

void TestScene::OnUpdate(float dt) noexcept
{
  m_Time += dt;
  for (int i = 0; i < 8; ++i)
  {
    m_GameObjectArray[i].RotateAround(2.f * dt, vec3(0.f, 0.f, 1.f));
  }
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
    m_MainCamera.MoveUp(0.75f * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    m_MainCamera.MoveDown(0.75f * dt);
  }
}

Camera& TestScene::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}
