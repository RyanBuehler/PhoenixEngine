//------------------------------------------------------------------------------
// File:    Camera.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Camera class declarations
//------------------------------------------------------------------------------
#pragma once
#include "GLEW/glew.h"
#include "GraphicsCommon.h"

// Forward Declarations
class Transform;

class Camera
{
public:
  struct ViewData
  {
    ViewData(
      float fov = 45.f,
      float aspect = 16.f / 9.f,
      float nearCull = 0.1f,
      float farCull = 1000.f
    ) :
    FOV(fov), Aspect(aspect), NearCull(nearCull), FarCull(farCull) {}

    float FOV;
    float Aspect;
    float NearCull;
    float FarCull;
  };
public:

  Camera(const string& name = "Unnamed Camera") noexcept;
  ~Camera();
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;
  Camera(Camera&&) = delete;
  Camera& operator=(Camera&&) = delete;

  void Update(float dt) noexcept;

  void EnableCamera() noexcept;
  void DisableCamera() noexcept;

  const glm::mat4& GetPersMatrix() noexcept;
  const glm::mat4& GetViewMatrix() noexcept;

  void SetTarget(Transform* target) noexcept;
  void ClearTarget() noexcept;

  void SetPosition(vec3 position);
  void LookAt(vec3 position);

  void SetName(const string& name) noexcept;
  const string& GetName() const noexcept;

private:
  // Camera Viewing Data
  ViewData m_ViewData;

  // Camera world data
  vec3 m_Position;
  vec3 m_Forward;
  vec3 m_Up;

  // Matrices
  mat4 m_PersMatrix;
  mat4 m_ViewMatrix;

  bool m_IsEnabled;
  bool m_bProjectionIsDirty;
  bool m_bViewIsDirty;

  // Camera Target
  Transform* m_Target;

  // Camera Name
  string m_Name;
};
