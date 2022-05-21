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

#define AMBIENT_FACTOR 0.05f
#define DIFFUSE_FACTOR 0.9f
#define SPECULAR_FACTOR 1.0f

SceneDemo::SceneDemo() noexcept :
  IScene("Demo Scene"),
  m_MainCamera(m_CameraManager.GetDefaultCamera()),
  m_Time(0.f)
{
  m_MainCamera.SetName("Demo Scene Camera");
  Log::trace("'Demo' Scene Created.");
  ImGui::MANAGER->SetOnDemoObjectHandler([this] { OnDemoObjectChangeEvent(); });
}

void SceneDemo::OnLoad() noexcept
{
  Log::trace("Demo Scene Loaded.");
}

void SceneDemo::OnInit() noexcept
{
  m_Time = 0;
  switch (ImGui::SCENE_SCENARIO)
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
  Log::trace("Demo Scene Initialized.");
}

void SceneDemo::OnUpdate(const float DeltaTime) noexcept
{
  m_Time += DeltaTime;

  for (int i = 0; i < 16; ++i)
  {
    if (i + 1 <= ImGui::LIGHTING_ACTIVE_LIGHTS)
    {
      m_GameObjectArray[i].SetIsActive(true);
      ImGui::LIGHTING_DATA_ARRAY[i].bIsActive = true;
    }
    else
    {
      m_GameObjectArray[i].SetIsActive(false);
      ImGui::LIGHTING_DATA_ARRAY[i].bIsActive = false;
    }
    if (ImGui::SCENE_ORBIT_OBJECTS)
    {
      m_GameObjectArray[i].RotateAround(5.f * DeltaTime, vec3(0.f, 1.f, 0.f));
    }
    ImGui::LIGHTING_DATA_ARRAY[i].Position = vec4(m_GameObjectArray[i].GetPosition(), 1.f);
    ImGui::LIGHTING_DATA_ARRAY[i].Direction = vec4(vec3(0.f, -0.3f, 0.f) - m_GameObjectArray[i].GetPosition(), 1.f);
  }
}

void SceneDemo::OnShutdown() noexcept
{
  m_GameObjectArray.clear();
  Log::trace("Demo Scene Shutdown.");
}

void SceneDemo::OnUnload() noexcept
{
  Log::trace("Demo Scene Unloaded.");
}

void SceneDemo::OnPollInput(GLFWwindow* WindowPtr, const float DeltaTime) noexcept
{
  static float moveSpeed = 10.f;
  if (glfwGetKey(WindowPtr, GLFW_KEY_W) == GLFW_PRESS)
  {
    m_MainCamera.MoveForward(moveSpeed * DeltaTime);
  }
  if (glfwGetKey(WindowPtr, GLFW_KEY_A) == GLFW_PRESS)
  {
    m_MainCamera.MoveLeft(moveSpeed * DeltaTime);
  }
  if (glfwGetKey(WindowPtr, GLFW_KEY_S) == GLFW_PRESS)
  {
    m_MainCamera.MoveBackward(moveSpeed * DeltaTime);
  }
  if (glfwGetKey(WindowPtr, GLFW_KEY_D) == GLFW_PRESS)
  {
    m_MainCamera.MoveRight(moveSpeed * DeltaTime);
  }
  if (glfwGetKey(WindowPtr, GLFW_KEY_Q) == GLFW_PRESS)
  {
    m_MainCamera.MoveUp(moveSpeed * DeltaTime);
  }
  if (glfwGetKey(WindowPtr, GLFW_KEY_E) == GLFW_PRESS)
  {
    m_MainCamera.MoveDown(moveSpeed * DeltaTime);
  }
}

void SceneDemo::OnDemoObjectChangeEvent()
{
  const auto mc = m_GameObjectArray[17].GetFirstComponentByType(Component::Type::MESH);
  const auto mcp = dynamic_pointer_cast<MeshComponent>(mc.value());
  mcp->SetMeshFileName(ImGui::DEMO_OBJECT_FILE);
}

Camera& SceneDemo::GetCurrentCamera() noexcept
{
  return m_MainCamera;
}

void SceneDemo::ChangeScenario(int Which) noexcept
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
    ImGui::LIGHTING_DATA_ARRAY[i].Type = Light::POINT_LIGHT;
    ImGui::LIGHTING_DATA_ARRAY[i].DiffuseIntensity = DIFFUSE_FACTOR * Colors::CYAN;
    ImGui::LIGHTING_DATA_ARRAY[i].AmbientIntensity = AMBIENT_FACTOR * Colors::CYAN;
    ImGui::LIGHTING_DATA_ARRAY[i].SpecularIntensity = SPECULAR_FACTOR * Colors::CYAN;
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
  mainMeshComp->SetMeshFileName(ImGui::DEMO_OBJECT_FILE);
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
    ImGui::LIGHTING_DATA_ARRAY[i].Type = Light::SPOT_LIGHT;
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
  ImGui::LIGHTING_DATA_ARRAY[0].DiffuseIntensity = DIFFUSE_FACTOR * Colors::RED;
  ImGui::LIGHTING_DATA_ARRAY[0].AmbientIntensity = AMBIENT_FACTOR * Colors::RED;
  ImGui::LIGHTING_DATA_ARRAY[0].SpecularIntensity = Colors::RED;
  mat.SetEmissive(Colors::RED);

  m_GameObjectArray[0].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[1].DiffuseIntensity = DIFFUSE_FACTOR * Colors::YELLOW;
  ImGui::LIGHTING_DATA_ARRAY[1].AmbientIntensity = AMBIENT_FACTOR * Colors::YELLOW;
  ImGui::LIGHTING_DATA_ARRAY[1].SpecularIntensity = SPECULAR_FACTOR * Colors::YELLOW;
  mat.SetEmissive(Colors::YELLOW);
  m_GameObjectArray[1].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[2].DiffuseIntensity = DIFFUSE_FACTOR * Colors::BLUE;
  ImGui::LIGHTING_DATA_ARRAY[2].AmbientIntensity = AMBIENT_FACTOR * Colors::BLUE;
  ImGui::LIGHTING_DATA_ARRAY[2].SpecularIntensity = SPECULAR_FACTOR * Colors::BLUE;
  mat.SetEmissive(Colors::BLUE);
  m_GameObjectArray[2].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[3].DiffuseIntensity = DIFFUSE_FACTOR * Colors::ORANGE;
  ImGui::LIGHTING_DATA_ARRAY[3].AmbientIntensity = AMBIENT_FACTOR * Colors::ORANGE;
  ImGui::LIGHTING_DATA_ARRAY[3].SpecularIntensity = SPECULAR_FACTOR * Colors::ORANGE;
  mat.SetEmissive(Colors::ORANGE);
  m_GameObjectArray[3].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[4].DiffuseIntensity = DIFFUSE_FACTOR * Colors::GREEN;
  ImGui::LIGHTING_DATA_ARRAY[4].AmbientIntensity = AMBIENT_FACTOR * Colors::GREEN;
  ImGui::LIGHTING_DATA_ARRAY[4].SpecularIntensity = SPECULAR_FACTOR * Colors::GREEN;
  mat.SetEmissive(Colors::GREEN);
  m_GameObjectArray[4].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[5].DiffuseIntensity = DIFFUSE_FACTOR * Colors::PURPLE;
  ImGui::LIGHTING_DATA_ARRAY[5].AmbientIntensity = AMBIENT_FACTOR * Colors::PURPLE;
  ImGui::LIGHTING_DATA_ARRAY[5].SpecularIntensity = SPECULAR_FACTOR * Colors::PURPLE;
  mat.SetEmissive(Colors::PURPLE);
  m_GameObjectArray[5].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[6].DiffuseIntensity = DIFFUSE_FACTOR * Colors::CYAN;
  ImGui::LIGHTING_DATA_ARRAY[6].AmbientIntensity = AMBIENT_FACTOR * Colors::CYAN;
  ImGui::LIGHTING_DATA_ARRAY[6].SpecularIntensity = SPECULAR_FACTOR * Colors::CYAN;
  mat.SetEmissive(Colors::CYAN);
  m_GameObjectArray[6].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[7].DiffuseIntensity = DIFFUSE_FACTOR * Colors::PINK;
  ImGui::LIGHTING_DATA_ARRAY[7].AmbientIntensity = AMBIENT_FACTOR * Colors::PINK;
  ImGui::LIGHTING_DATA_ARRAY[7].SpecularIntensity = SPECULAR_FACTOR * Colors::PINK;
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
  mainMeshComp->SetMeshFileName(ImGui::DEMO_OBJECT_FILE);
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
      ImGui::LIGHTING_DATA_ARRAY[i].Type = Light::SPOT_LIGHT;
    }
    else
    {
      ImGui::LIGHTING_DATA_ARRAY[i].Type = Light::POINT_LIGHT;
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
      ImGui::LIGHTING_DATA_ARRAY[i].Type = Light::SPOT_LIGHT;
    }
    else
    {
      ImGui::LIGHTING_DATA_ARRAY[i].Type = Light::POINT_LIGHT;
    }
  }

  Material mat = Material::Type::LIGHT;
  ImGui::LIGHTING_DATA_ARRAY[0].DiffuseIntensity = DIFFUSE_FACTOR * Colors::RED;
  ImGui::LIGHTING_DATA_ARRAY[0].AmbientIntensity = AMBIENT_FACTOR * Colors::RED;
  ImGui::LIGHTING_DATA_ARRAY[0].SpecularIntensity = SPECULAR_FACTOR * Colors::RED;
  mat.SetEmissive(Colors::RED);
  m_GameObjectArray[0].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[1].DiffuseIntensity = DIFFUSE_FACTOR * Colors::YELLOW;
  ImGui::LIGHTING_DATA_ARRAY[1].AmbientIntensity = AMBIENT_FACTOR * Colors::YELLOW;
  ImGui::LIGHTING_DATA_ARRAY[1].SpecularIntensity = SPECULAR_FACTOR * Colors::YELLOW;
  mat.SetEmissive(Colors::YELLOW);
  m_GameObjectArray[1].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[2].DiffuseIntensity = DIFFUSE_FACTOR * Colors::BLUE;
  ImGui::LIGHTING_DATA_ARRAY[2].AmbientIntensity = AMBIENT_FACTOR * Colors::BLUE;
  ImGui::LIGHTING_DATA_ARRAY[2].SpecularIntensity = SPECULAR_FACTOR * Colors::BLUE;
  mat.SetEmissive(Colors::BLUE);
  m_GameObjectArray[2].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[3].DiffuseIntensity = DIFFUSE_FACTOR * Colors::ORANGE;
  ImGui::LIGHTING_DATA_ARRAY[3].AmbientIntensity = AMBIENT_FACTOR * Colors::ORANGE;
  ImGui::LIGHTING_DATA_ARRAY[3].SpecularIntensity = SPECULAR_FACTOR * Colors::ORANGE;
  mat.SetEmissive(Colors::ORANGE);
  m_GameObjectArray[3].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[4].DiffuseIntensity = DIFFUSE_FACTOR * Colors::GREEN;
  ImGui::LIGHTING_DATA_ARRAY[4].AmbientIntensity = AMBIENT_FACTOR * Colors::GREEN;
  ImGui::LIGHTING_DATA_ARRAY[4].SpecularIntensity = SPECULAR_FACTOR * Colors::GREEN;
  mat.SetEmissive(Colors::GREEN);
  m_GameObjectArray[4].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[5].DiffuseIntensity = DIFFUSE_FACTOR * Colors::PURPLE;
  ImGui::LIGHTING_DATA_ARRAY[5].AmbientIntensity = AMBIENT_FACTOR * Colors::PURPLE;
  ImGui::LIGHTING_DATA_ARRAY[5].SpecularIntensity = SPECULAR_FACTOR * Colors::PURPLE;
  mat.SetEmissive(Colors::PURPLE);
  m_GameObjectArray[5].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[6].DiffuseIntensity = DIFFUSE_FACTOR * Colors::CYAN;
  ImGui::LIGHTING_DATA_ARRAY[6].AmbientIntensity = AMBIENT_FACTOR * Colors::CYAN;
  ImGui::LIGHTING_DATA_ARRAY[6].SpecularIntensity = SPECULAR_FACTOR * Colors::CYAN;
  mat.SetEmissive(Colors::CYAN);
  m_GameObjectArray[6].SetMaterial(mat);

  ImGui::LIGHTING_DATA_ARRAY[7].DiffuseIntensity = DIFFUSE_FACTOR * Colors::PINK;
  ImGui::LIGHTING_DATA_ARRAY[7].AmbientIntensity = AMBIENT_FACTOR * Colors::PINK;
  ImGui::LIGHTING_DATA_ARRAY[7].SpecularIntensity = SPECULAR_FACTOR * Colors::PINK;
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
