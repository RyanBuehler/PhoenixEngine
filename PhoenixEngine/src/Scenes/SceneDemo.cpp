//------------------------------------------------------------------------------
// File:    SceneDemo.cpp
// Author:  Ryan Buehler
// Created: 09/27/21
// Desc:    A test scene for experimentation
//------------------------------------------------------------------------------
#include "pch.h"
#include "SceneDemo.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "DebugRenderer.h"
#include "ImGUIManager.h"
#include "Colors.h"
#include "MeshComponent.h"
#include "AssetLoader.h"

#define AMBFACTOR 0.05f
#define DIFFFACTOR 0.9f
#define SPECFACTOR 1.0f

SceneDemo::SceneDemo() noexcept :
  IScene("Demo Scene"),
  m_MainCamera(m_CameraManager.GetDefaultCamera()),
  m_Time(0.f)
{
  m_MainCamera.SetName("Demo Scene Camera");
  Log::Trace("'Demo' Scene Created.");
  ImGui::Manager->SetOnDemoObjectHandler([this] { OnDemoObjectChangeEvent(); });
  AssetLoader::LoadFBX(("res/models/susanne.fbx"));

}

void SceneDemo::OnLoad() noexcept
{
  Log::Trace("Demo Scene Loaded.");
}

void SceneDemo::OnInit() noexcept
{
  m_Time = 0;
  switch (ImGui::SceneScenario)
  {
  case 2:
    Scenario2();
    break;
  case 3:
    Scenario3();
    break;
  default:
    Scenario1();
    break;
  }
  Log::Trace("Demo Scene Initialized.");
}

void SceneDemo::OnUpdate(const float Dt) noexcept
{
  m_Time += Dt;

  for (int i = 0; i < 16; ++i)
  {
    if (i + 1 <= ImGui::LightingActiveLights)
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
      m_GameObjectArray[i].RotateAround(5.f * Dt, vec3(0.f, 1.f, 0.f));
    }
    ImGui::LightingDataArray[i].Position = vec4(m_GameObjectArray[i].GetPosition(), 1.f);
    ImGui::LightingDataArray[i].Direction = vec4(vec3(0.f, -0.3f, 0.f) - m_GameObjectArray[i].GetPosition(), 1.f);
  }
}

void SceneDemo::OnShutdown() noexcept
{
  m_GameObjectArray.clear();
  Log::Trace("Demo Scene Shutdown.");
}

void SceneDemo::OnUnload() noexcept
{
  Log::Trace("Demo Scene Unloaded.");
}

void SceneDemo::OnPollInput(GLFWwindow* window, float dt) noexcept
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

void SceneDemo::OnDemoObjectChangeEvent()
{
  auto MC = m_GameObjectArray[17].GetFirstComponentByType(Component::Type::MESH);
  auto MCP = dynamic_pointer_cast<MeshComponent>(MC.value());
  MCP->SetMeshFileName(ImGui::DemoObjectFile);
}

Camera& SceneDemo::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}

void SceneDemo::ChangeScenario(int which) noexcept
{
}

void SceneDemo::Scenario1() noexcept
{
  for (int i = 0; i < 18; ++i)
  {
    m_GameObjectArray.emplace_back();
  }

  m_MainCamera.SetPosition({ 2.f, 2.f, 10.f });

  // Lights
  for (int i = 0; i < 8; ++i)
  {
    auto comp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[i].AddComponent(Component::Type::MESH));
    comp->SetMeshFileName("sphere");
    comp->SetMaterial(Material::Type::LIGHT);
    comp->SetIsActive(false);
    m_GameObjectArray[i].SetPosition(vec3(2.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(360.f / 8.f * i, vec3(0.f, 1.f, 0.f));
    m_GameObjectArray[i].ScaleBy(0.1f);
    m_GameObjectArray[i].SetMaterial(Material::Type::LIGHT);
    m_GameObjectArray[i].SetIsActive(false);
  }
  for (int i = 8; i < 16; ++i)
  {
    auto comp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[i].AddComponent(Component::Type::MESH));
    comp->SetMeshFileName("sphere");
    comp->SetMaterial(Material::Type::LIGHT);
    comp->SetIsActive(false);
    m_GameObjectArray[i].SetPosition(vec3(2.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(22.5f + 360.f / 8.f * i, vec3(0.f, 1.f, 0.f));
    m_GameObjectArray[i].ScaleBy(0.12f);
    m_GameObjectArray[i].SetMaterial(Material::Type::LIGHT);
    m_GameObjectArray[i].SetIsActive(false);
  }

  Material mat = Material::Type::LIGHT;

  for (int i = 0; i < 8; ++i)
  {
    ImGui::LightingDataArray[i].Type = Light::POINT_LIGHT;
    ImGui::LightingDataArray[i].DiffuseIntensity = DIFFFACTOR * Colors::CYAN;
    ImGui::LightingDataArray[i].AmbientIntensity = AMBFACTOR * Colors::CYAN;
    ImGui::LightingDataArray[i].SpecularIntensity = SPECFACTOR * Colors::CYAN;
    mat.SetEmissive(Colors::CYAN);
    m_GameObjectArray[i].SetMaterial(mat);
  }

  // Lines
  GameObject temp;
  temp.SetPosition(vec3(2.0f, 0.f, 0.f));
  for (int i = 2; i <= 360; i += 2)
  {
    vec3 pt1 = temp.GetPosition();
    temp.RotateAround(glm::radians(static_cast<float>(i)), vec3(0.f, 1.f, 0.f));
    vec3 pt2 = temp.GetPosition();
    DebugRenderer::I().AddPermanentLine(pt1, pt2);
  }
  temp.SetIsActive(false);

  // Plane
  auto planeMeshComp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[16].AddComponent(Component::Type::MESH));
  planeMeshComp->SetMeshFileName("quad.obj");
  planeMeshComp->SetMaterial(Material::Type::BASIC);
  planeMeshComp->SetIsActive(true);
  m_GameObjectArray[16].SetScale({ 5.f, 5.f, 1.f });
  m_GameObjectArray[16].SetPosition({ 0.f, -0.5f, 0.f });
  m_GameObjectArray[16].RotateX(-90.f);
  m_GameObjectArray[16].SetMaterial(Material::Type::BASIC);

  // Main object
  auto mainMeshComp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[17].AddComponent(Component::Type::MESH));
  mainMeshComp->SetMeshFileName(ImGui::DemoObjectFile);
  mainMeshComp->SetMaterial(Material::Type::BASIC);
  mainMeshComp->SetIsActive(true);
  m_GameObjectArray[17].SetPosition(vec3(0.f));
  m_GameObjectArray[17].ScaleBy(1.f);
  m_GameObjectArray[17].SetMaterial(Material::Type::GLOBAL);
  m_GameObjectArray[17].SetIsActive(false);

  // Target the camera
  m_MainCamera.SetTarget(&m_GameObjectArray[17].GetTransform());
}

void SceneDemo::Scenario2() noexcept
{
  for (int i = 0; i < 18; ++i)
  {
    m_GameObjectArray.emplace_back();
  }

  m_MainCamera.SetPosition({ 2.f, 2.f, 10.f });

  // Lights
  for (int i = 0; i < 8; ++i)
  {
    auto comp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[i].AddComponent(Component::Type::MESH));
    comp->SetMeshFileName("sphere");
    comp->SetMaterial(Material::Type::LIGHT);
    comp->SetIsActive(false);
    m_GameObjectArray[i].SetPosition(vec3(2.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(360.f / 8.f * i, vec3(0.f, 1.f, 0.f));
    m_GameObjectArray[i].ScaleBy(0.1f);
    m_GameObjectArray[i].SetMaterial(Material::Type::LIGHT);
    m_GameObjectArray[i].SetIsActive(false);
    ImGui::LightingDataArray[i].Type = Light::SPOT_LIGHT;
  }
  for (int i = 8; i < 16; ++i)
  {
    auto comp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[i].AddComponent(Component::Type::MESH));
    comp->SetMeshFileName("sphere");
    comp->SetMaterial(Material::Type::LIGHT);
    comp->SetIsActive(false);
    m_GameObjectArray[i].SetPosition(vec3(2.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(22.5f + 360.f / 8.f * i, vec3(0.f, 1.f, 0.f));
    m_GameObjectArray[i].ScaleBy(0.12f);
    m_GameObjectArray[i].SetMaterial(Material::Type::LIGHT);
    m_GameObjectArray[i].SetIsActive(false);
  }

  Material mat = Material::Type::LIGHT;
  ImGui::LightingDataArray[0].DiffuseIntensity = DIFFFACTOR * Colors::RED;
  ImGui::LightingDataArray[0].AmbientIntensity = AMBFACTOR * Colors::RED;
  ImGui::LightingDataArray[0].SpecularIntensity = Colors::RED;
  mat.SetEmissive(Colors::RED);

  m_GameObjectArray[0].SetMaterial(mat);

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

  // Lines
  GameObject temp;
  temp.SetPosition(vec3(2.0f, 0.f, 0.f));
  for (int i = 2; i <= 360; i += 2)
  {
    vec3 pt1 = temp.GetPosition();
    temp.RotateAround(glm::radians(static_cast<float>(i)), vec3(0.f, 1.f, 0.f));
    vec3 pt2 = temp.GetPosition();
    DebugRenderer::I().AddPermanentLine(pt1, pt2);
  }
  temp.SetIsActive(false);

  // Plane
  auto planeMeshComp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[16].AddComponent(Component::Type::MESH));
  planeMeshComp->SetMeshFileName("quad.obj");
  planeMeshComp->SetMaterial(Material::Type::BASIC);
  planeMeshComp->SetIsActive(true);
  m_GameObjectArray[16].SetScale({ 5.f, 5.f, 1.f });
  m_GameObjectArray[16].SetPosition({ 0.f, -0.5f, 0.f });
  m_GameObjectArray[16].RotateX(-90.f);

  // Main object
  auto mainMeshComp = dynamic_pointer_cast<MeshComponent>(m_GameObjectArray[17].AddComponent(Component::Type::MESH));
  mainMeshComp->SetMeshFileName(ImGui::DemoObjectFile);
  mainMeshComp->SetMaterial(Material::Type::GLOBAL);
  mainMeshComp->SetIsActive(true);
  m_GameObjectArray[17].SetPosition(vec3(0.f));
  m_GameObjectArray[17].ScaleBy(1.f);

  m_MainCamera.SetTarget(&m_GameObjectArray[17].GetTransform());
}

void SceneDemo::Scenario3() noexcept
{
  for (int i = 0; i < 18; ++i)
  {
    m_GameObjectArray.emplace_back();
  }

  m_MainCamera.SetPosition({ 2.f, 2.f, 10.f });

  // Lights
  for (int i = 0; i < 8; ++i)
  {
    m_GameObjectArray[i].SetPosition(vec3(2.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(360.f / 8.f * i, vec3(0.f, 1.f, 0.f));
    m_GameObjectArray[i].ScaleBy(0.1f);
    m_GameObjectArray[i].SetMaterial(Material::Type::LIGHT);
    m_GameObjectArray[i].SetIsActive(false);
    if (i % 2)
    {
      ImGui::LightingDataArray[i].Type = Light::SPOT_LIGHT;
    }
    else
    {
      ImGui::LightingDataArray[i].Type = Light::POINT_LIGHT;
    }
  }
  for (int i = 8; i < 16; ++i)
  {
    m_GameObjectArray[i].SetPosition(vec3(2.0f, 0.f, 0.f));
    m_GameObjectArray[i].RotateAround(22.5f + 360.f / 8.f * i, vec3(0.f, 1.f, 0.f));
    m_GameObjectArray[i].ScaleBy(0.12f);
    m_GameObjectArray[i].SetMaterial(Material::Type::LIGHT);
    m_GameObjectArray[i].SetIsActive(false);
    if (i % 2)
    {
      ImGui::LightingDataArray[i].Type = Light::SPOT_LIGHT;
    }
    else
    {
      ImGui::LightingDataArray[i].Type = Light::POINT_LIGHT;
    }
  }

  Material mat = Material::Type::LIGHT;
  ImGui::LightingDataArray[0].DiffuseIntensity = DIFFFACTOR * Colors::RED;
  ImGui::LightingDataArray[0].AmbientIntensity = AMBFACTOR * Colors::RED;
  ImGui::LightingDataArray[0].SpecularIntensity = SPECFACTOR * Colors::RED;
  mat.SetEmissive(Colors::RED);
  m_GameObjectArray[0].SetMaterial(mat);

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

  // Lines
  GameObject temp;
  temp.SetPosition(vec3(2.0f, 0.f, 0.f));
  for (int i = 2; i <= 360; i += 2)
  {
    vec3 pt1 = temp.GetPosition();
    temp.RotateAround(glm::radians(static_cast<float>(i)), vec3(0.f, 1.f, 0.f));
    vec3 pt2 = temp.GetPosition();
    DebugRenderer::I().AddPermanentLine(pt1, pt2);
  }
  temp.SetIsActive(false);


  // Plane
  m_GameObjectArray[16].SetScale({ 5.f, 0.01f, 5.f });
  m_GameObjectArray[16].SetPosition({ 0.f, -0.5f, 0.f });
  m_GameObjectArray[16].RotateX(0.f);
  m_GameObjectArray[16].SetMaterial(Material::Type::GLOBAL);

  m_GameObjectArray[17].SetPosition(vec3(0.f));
  m_GameObjectArray[17].ScaleBy(1.f);
  m_GameObjectArray[17].SetMaterial(Material::Type::TEXTURE);
  m_MainCamera.SetTarget(&m_GameObjectArray[17].GetTransform());
}
