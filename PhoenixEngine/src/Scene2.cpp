//------------------------------------------------------------------------------
// File:    Scene2.cpp
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#include "pch.h"
#include "Scene2.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "DebugRenderer.h"
#include "ImGUIManager.h"

Scene2::Scene2() noexcept :
  IScene("Scene 2"),
  m_MainCamera(m_CameraManager.GetDefaultCamera()),
  m_Time(0.f)
{
  m_MainCamera.SetName("Scene 2 Camera");
  Log::Trace("Scene 2 Created.");
  ImGui::Manager->SetOnDemoObjectHandler(std::bind(&Scene2::OnDemoObjectChangeEvent, this));
}

void Scene2::OnLoad() noexcept
{
  Log::Trace("Scene 2 Loaded.");
}

void Scene2::OnInit() noexcept
{
  m_Time = 0;

  Log::Trace("Scene 2 Initialized.");
  for (int i = 0; i < 8; ++i)
  {
    m_GameObjectArray.emplace_back("sphere");
  }
  m_GameObjectArray.emplace_back(ImGui::DemoObjectFile);

  m_MainCamera.SetPosition({ 1.f, 1.f, 10.f });

  for (int i = 0; i < 8; ++i)
  {
    //m_GameObjectArray[i].SetIsActive(false);
    m_GameObjectArray[i].SetPosition(vec3(2.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(360.f / 8.f * i, vec3(0.f, 0.f, 1.f));
    m_GameObjectArray[i].ScaleBy(0.1f);
  }

  GameObject temp("sphere.obj");
  temp.SetPosition(vec3(2.0f, 0.f, 0.f));
  for (int i = 2; i <= 360; i += 2)
  {
    vec3 pt1 = temp.GetPosition();
    temp.RotateAround(glm::radians(static_cast<float>(i)), vec3(0.f, 0.f, 1.f));
    vec3 pt2 = temp.GetPosition();
    DebugRenderer::I().AddPermanentLine(pt1, pt2);
  }
  temp.SetIsActive(false);
  //DebugRenderer::I().AddPermanentLine(temp.GetPosition(), vec3(1.f, 0.f, 0.f));

  //m_GameObjectArray[8].SetIsActive(false);
  m_GameObjectArray[8].SetPosition(vec3(0.f));
  m_GameObjectArray[8].ScaleBy(1.f);
  m_MainCamera.SetTarget(&m_GameObjectArray[8].GetTransform());

  m_Time = 0.f;
}

void Scene2::OnUpdate(float dt) noexcept
{
  m_Time += dt;
  for (int i = 0; i < 8; ++i)
  {
    m_GameObjectArray[i].RotateAround(2.f * dt, vec3(0.f, 0.f, 1.f));
  }
}

void Scene2::OnShutdown() noexcept
{
  m_GameObjectArray.clear();
  Log::Trace("Scene 2 Shutdown.");
}

void Scene2::OnUnload() noexcept
{
  Log::Trace("Scene 2 Unloaded.");
}

void Scene2::OnPollInput(GLFWwindow* window, float dt) noexcept
{
  static float move_speed = 10.f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    m_MainCamera.MoveForward(move_speed * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    m_MainCamera.MoveLeft(move_speed * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    m_MainCamera.MoveBackward(move_speed * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    m_MainCamera.MoveRight(move_speed * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    m_MainCamera.MoveUp(move_speed * dt);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    m_MainCamera.MoveDown(move_speed * dt);
  }
}

void Scene2::OnDemoObjectChangeEvent()
{
  m_GameObjectArray[8].SetMeshFileName(ImGui::DemoObjectFile);
}

Camera& Scene2::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}
