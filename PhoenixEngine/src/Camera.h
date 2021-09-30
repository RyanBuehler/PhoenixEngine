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

  /// <summary>
  /// Gets the Perspective Transform Matrix
  /// </summary>
  /// <returns>4x4 Matrix of the Perspective Transform Matrix</returns>
  const glm::mat4& GetPersMatrix() noexcept;

  /// <summary>
  /// Gets the View Transform Matrix
  /// </summary>
  /// <returns>4x4 Matrix of the View Transform Matrix</returns>
  const glm::mat4& GetViewMatrix() noexcept;

  /// <summary>
  /// Sets the "LookAt" target for the camera
  /// </summary>
  /// <param name="target">The transform ptr of the target to watch.</param>
  void SetTarget(const Transform* target) noexcept;

  /// <summary>
  /// Clears the "LookAt" target
  /// </summary>
  void ClearTarget() noexcept;

  /// <summary>
  /// Sets the Camera's position
  /// </summary>
  /// <param name="position">Vec3 World Position</param>
  void SetPosition(vec3 position);

  void MoveForward(float distance) noexcept;
  void MoveBackward(float distance) noexcept;
  void MoveRight(float distance) noexcept;
  void MoveLeft(float distance) noexcept;
  void MoveUp(float distance) noexcept;
  void MoveDown(float distance) noexcept;

  void SetYaw(float degrees);
  void SetPitch(float degrees);
  void SetRoll(float degrees);

  vec3 GetPosition() const noexcept;
  vec3 GetForwardVector() const noexcept;
  vec3 GetUpVector() const noexcept;

  /// <summary>
  /// Makes the Camera look at a target position
  /// </summary>
  /// <param name="position">Vec3 World Position</param>
  void LookAt(vec3 position);

  /// <summary>
  /// Sets the Camera's name
  /// </summary>
  /// <param name="name">The new name for the Camera</param>
  void SetName(const string& name) noexcept;

  /// <summary>
  /// Gets the Camera's current name
  /// </summary>
  /// <returns>The Camera's current name</returns>
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
  const Transform* m_Target;

  // Camera Name
  string m_Name;
};
