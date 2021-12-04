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

#define AMBFACTOR 0.05f
#define DIFFFACTOR 0.9f
#define SPECFACTOR 1.0f

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

  m_MainCamera.SetPosition({ 2.f, 2.f, 10.f });

  m_GameObjectArray[0].SetPosition(vec3(0.f));
  m_GameObjectArray[0].ScaleBy(2.f);
  //m_GameObjectArray[0].SetMaterial(Material::Type::GLOBAL);
  m_GameObjectArray[0].SetMaterial(Material::Type::REFLECTREFRACT);
  m_MainCamera.SetTarget(&m_GameObjectArray[0].GetTransform());

  for (int i = 1; i < 9; ++i)
  {
    m_GameObjectArray.emplace_back("sphere");
    // Lights
    m_GameObjectArray[i].SetPosition(vec3(3.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(360.f / 8.f * i, vec3(0.f, 1.f, 0.f));
    m_GameObjectArray[i].ScaleBy(0.2f);
    m_GameObjectArray[i].SetMaterial(Material::Type::LIGHT);
    m_GameObjectArray[i].SetIsActive(false);
    ImGui::LightingDataArray[i].Type = Light::POINT_LIGHT;
  }

  Material mat = Material::Type::LIGHT;
  ImGui::LightingDataArray[8].DiffuseIntensity = DIFFFACTOR * Colors::RED;
  ImGui::LightingDataArray[8].AmbientIntensity = AMBFACTOR * Colors::RED;
  ImGui::LightingDataArray[8].SpecularIntensity = Colors::RED;
  mat.SetEmissive(Colors::RED);
  m_GameObjectArray[8].SetMaterial(mat);

  ImGui::LightingDataArray[1].DiffuseIntensity = DIFFFACTOR * Colors::YELLOW;
  ImGui::LightingDataArray[1].AmbientIntensity = AMBFACTOR * Colors::YELLOW;
  ImGui::LightingDataArray[1].SpecularIntensity = SPECFACTOR * Colors::YELLOW;
  mat.SetEmissive(Colors::YELLOW);
  m_GameObjectArray[1].SetMaterial(mat);

  ImGui::LightingDataArray[2].DiffuseIntensity = DIFFFACTOR * Colors::BLUE;
  ImGui::LightingDataArray[2].AmbientIntensity = AMBFACTOR * Colors::BLUE;
  ImGui::LightingDataArray[2].SpecularIntensity = SPECFACTOR * Colors::BLUE;
  mat.SetEmissive(Colors::BLUE);
  m_GameObjectArray[2].SetMaterial(mat);

  ImGui::LightingDataArray[3].DiffuseIntensity = DIFFFACTOR * Colors::ORANGE;
  ImGui::LightingDataArray[3].AmbientIntensity = AMBFACTOR * Colors::ORANGE;
  ImGui::LightingDataArray[3].SpecularIntensity = SPECFACTOR * Colors::ORANGE;
  mat.SetEmissive(Colors::ORANGE);
  m_GameObjectArray[3].SetMaterial(mat);

  ImGui::LightingDataArray[4].DiffuseIntensity = DIFFFACTOR * Colors::GREEN;
  ImGui::LightingDataArray[4].AmbientIntensity = AMBFACTOR * Colors::GREEN;
  ImGui::LightingDataArray[4].SpecularIntensity = SPECFACTOR * Colors::GREEN;
  mat.SetEmissive(Colors::GREEN);
  m_GameObjectArray[4].SetMaterial(mat);

  ImGui::LightingDataArray[5].DiffuseIntensity = DIFFFACTOR * Colors::PURPLE;
  ImGui::LightingDataArray[5].AmbientIntensity = AMBFACTOR * Colors::PURPLE;
  ImGui::LightingDataArray[5].SpecularIntensity = SPECFACTOR * Colors::PURPLE;
  mat.SetEmissive(Colors::PURPLE);
  m_GameObjectArray[5].SetMaterial(mat);

  ImGui::LightingDataArray[6].DiffuseIntensity = DIFFFACTOR * Colors::CYAN;
  ImGui::LightingDataArray[6].AmbientIntensity = AMBFACTOR * Colors::CYAN;
  ImGui::LightingDataArray[6].SpecularIntensity = SPECFACTOR * Colors::CYAN;
  mat.SetEmissive(Colors::CYAN);
  m_GameObjectArray[6].SetMaterial(mat);

  ImGui::LightingDataArray[7].DiffuseIntensity = DIFFFACTOR * Colors::PINK;
  ImGui::LightingDataArray[7].AmbientIntensity = AMBFACTOR * Colors::PINK;
  ImGui::LightingDataArray[7].SpecularIntensity = SPECFACTOR * Colors::PINK;
  mat.SetEmissive(Colors::PINK);
  m_GameObjectArray[7].SetMaterial(mat);
}

void ReflectionScene::OnUpdate(float dt) noexcept
{
  m_Time += dt;

  for (int i = 1; i < 9; ++i)
  {
    if (i <= ImGui::LightingActiveLights)
    {
      m_GameObjectArray[i].SetIsActive(true);
      ImGui::LightingDataArray[i].IsActive = true;
    }
    else
    {
      m_GameObjectArray[i].SetIsActive(false);
      ImGui::LightingDataArray[i].IsActive = false;
    }
    if (ImGui::SceneOrbitObjects)
    {
      m_GameObjectArray[i].RotateAround(5.f * dt, vec3(0.f, 1.f, 0.f));
    }
    ImGui::LightingDataArray[i].Position = vec4(m_GameObjectArray[i].GetPosition(), 1.f);
    ImGui::LightingDataArray[i].Direction = vec4(vec3(0.f, -0.3f, 0.f) - m_GameObjectArray[i].GetPosition(), 1.f);
  }

  if (ImGui::GraphicsSelectedShader == 3)
  {
    m_GameObjectArray[0].SetMaterial(Material::Type::REFLECTREFRACT);
  }
  else
  {
    m_GameObjectArray[0].SetMaterial(Material::Type::GLOBAL);
  }
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
    if (glm::length(glm::abs(m_MainCamera.GetPosition())) > 1.f)
    {
      m_MainCamera.MoveForward(move_speed * dt);
    }
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
    m_MainCamera.SetPosition({ 0.f, 0.f, 4.f });
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
