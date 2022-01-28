//------------------------------------------------------------------------------
// File:    SceneSingleObject.cpp
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for expirementation
//------------------------------------------------------------------------------
#include "pch.h"
#include "SceneSingleObject.h"
#include "../Transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../ImGUIManager.h"

SceneSingleObject::SceneSingleObject() noexcept :
  IScene("Scene Single Object"),
  m_MainCamera(m_CameraManager.GetDefaultCamera()),
  m_Time(0.f)
{
  m_MainCamera.SetName("Scene Single Object Camera");
  Log::Trace("Scene Single Object Created.");
  ImGui::Manager->SetOnDemoObjectHandler(std::bind(&SceneSingleObject::OnDemoObjectChangeEvent, this));
}

void SceneSingleObject::OnLoad() noexcept
{
  Log::Trace("Scene Single Object Loaded.");
}

void SceneSingleObject::OnInit() noexcept
{
  m_Time = 0;

  Log::Trace("Scene Single Object Initialized.");
  m_GameObjectArray.emplace_back(ImGui::DemoObjectFile);

  m_MainCamera.SetPosition({ 1.f, 1.f, 10.f });

  m_GameObjectArray[0].SetPosition(vec3(0.f));
  m_GameObjectArray[0].ScaleBy(1.f);
  m_GameObjectArray[0].SetMaterial(Material(Material::Type::GLOBAL));
  m_MainCamera.SetTarget(&m_GameObjectArray[0].GetTransform());
}

void SceneSingleObject::OnUpdate(float dt) noexcept
{
  m_Time += dt;
}

void SceneSingleObject::OnShutdown() noexcept
{
  m_GameObjectArray.clear();
  Log::Trace("Scene Single Object Shutdown.");
}

void SceneSingleObject::OnUnload() noexcept
{
  Log::Trace("Scene Single Object Unloaded.");
}

void SceneSingleObject::OnPollInput(GLFWwindow* window, float dt) noexcept
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

void SceneSingleObject::OnDemoObjectChangeEvent()
{
  m_GameObjectArray[0].SetMeshFileName(ImGui::DemoObjectFile);
}

Camera& SceneSingleObject::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}
