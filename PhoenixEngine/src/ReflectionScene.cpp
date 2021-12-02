//------------------------------------------------------------------------------
// File:    ReflectionScene.cpp
// Author:  Ryan Buehler
// Created: December 1, 2021
// Desc:    A reflection test scene
//------------------------------------------------------------------------------
#include "pch.h"
#include "ReflectionScene.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "DebugRenderer.h"
#include "ImGUIManager.h"

ReflectionScene::ReflectionScene() noexcept :
  IScene("Reflection Scene"),
  m_MainCamera(m_CameraManager.GetDefaultCamera()),
  m_Time(0.f)
{
  m_MainCamera.SetName("Reflection Scene Camera");
  Log::Trace("Reflection Scene Created.");
  ImGui::Manager->SetOnDemoObjectHandler(std::bind(&ReflectionScene::OnDemoObjectChangeEvent, this));
}

void ReflectionScene::OnLoad() noexcept
{
  Log::Trace("Reflection Scene Loaded.");
}

void ReflectionScene::OnInit() noexcept
{
  m_Time = 0;

  Log::Trace("Reflection Scene Initialized.");
  m_GameObjectArray.emplace_back(ImGui::DemoObjectFile);
  m_GameObjectArray.emplace_back("sphere");

  m_MainCamera.SetPosition({ 0.f, 0.f, 2.f });

  m_GameObjectArray[0].SetPosition(vec3(0.f));
  m_GameObjectArray[0].ScaleBy(1.f);
  m_GameObjectArray[0].SetMaterial(Material::Type::REFLECTIVE);
  m_MainCamera.SetTarget(&m_GameObjectArray[0].GetTransform());

  m_GameObjectArray[1].SetPosition(vec3(4.f, 0.f, 0.f));
  m_GameObjectArray[1].ScaleBy(0.5f);
  m_GameObjectArray[1].SetMaterial(Material::Type::LIGHT);
}

void ReflectionScene::OnUpdate(float dt) noexcept
{
  m_Time += dt;
}

void ReflectionScene::OnShutdown() noexcept
{
  m_GameObjectArray.clear();
  Log::Trace("Reflection Scene Shutdown.");
}

void ReflectionScene::OnUnload() noexcept
{
  Log::Trace("Reflection Scene Unloaded.");
}

void ReflectionScene::OnPollInput(GLFWwindow* window, float dt) noexcept
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
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
  {
    m_MainCamera.SetPosition({ 0.f, 0.f, 2.f });
  }

}

void ReflectionScene::OnDemoObjectChangeEvent()
{
  m_GameObjectArray[0].SetMeshFileName(ImGui::DemoObjectFile);
}

Camera& ReflectionScene::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}
