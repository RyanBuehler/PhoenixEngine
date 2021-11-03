//------------------------------------------------------------------------------
// File:    Scene1.cpp
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#include "pch.h"
#include "Scene1.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "DebugRenderer.h"
#include "ImGUIManager.h"

Scene1::Scene1() noexcept :
  IScene("Scene 1"),
  m_MainCamera(m_CameraManager.GetDefaultCamera()),
  m_Time(0.f)
{
  m_MainCamera.SetName("Scene 1 Camera");
  Log::Trace("Scene 1 Created.");
  ImGui::Manager->SetOnDemoObjectHandler(std::bind(&Scene1::OnDemoObjectChangeEvent, this));
}

void Scene1::OnLoad() noexcept
{
  Log::Trace("Scene 1 Loaded.");
}

void Scene1::OnInit() noexcept
{
  m_Time = 0;

  Log::Trace("Scene 1 Initialized.");
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

void Scene1::OnUpdate(float dt) noexcept
{
  m_Time += dt;
  for (int i = 0; i < 8; ++i)
  {
    m_GameObjectArray[i].RotateAround(2.f * dt, vec3(0.f, 0.f, 1.f));
  }
}

void Scene1::OnShutdown() noexcept
{
  m_GameObjectArray.clear();
  Log::Trace("Scene 1 Shutdown.");
}

void Scene1::OnUnload() noexcept
{
  Log::Trace("Scene 1 Unloaded.");
}

void Scene1::OnPollInput(GLFWwindow* window, float dt) noexcept
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

void Scene1::OnDemoObjectChangeEvent()
{
  m_GameObjectArray[8].SetMeshFileName(ImGui::DemoObjectFile);
}

Camera& Scene1::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}
