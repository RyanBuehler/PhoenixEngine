//------------------------------------------------------------------------------
// File:    Camera.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Camera class declarations
//------------------------------------------------------------------------------
#pragma once
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
  /// Gets the combined View Perspective Transform Matrix
  /// </summary>
  /// <returns>4x4 Matrix of the View Perspective Transform Matrix</returns>
  const glm::mat4& GetVPMatrix() noexcept;

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

  /// <summary>
  /// Moves the camera forward
  /// </summary>
  /// <param name="distance">The world distance to move the camera forward</param>
  void MoveForward(float distance) noexcept;

  /// <summary>
  /// Moves the camera backward
  /// </summary>
  /// <param name="distance">The world distance to move the camera backward</param>
  void MoveBackward(float distance) noexcept;

  /// <summary>
  /// Moves the camera directly to the camera's right
  /// </summary>
  /// <param name="distance">The world distance to move the camera right</param>
  void MoveRight(float distance) noexcept;

  /// <summary>
  /// Moves the camera directly to the camera's left
  /// </summary>
  /// <param name="distance">The world distance to move the camera left</param>
  void MoveLeft(float distance) noexcept;

  /// <summary>
  /// Moves the camera up
  /// </summary>
  /// <param name="distance">The world distance to move the camera up</param>
  void MoveUp(float distance) noexcept;

  /// <summary>
  /// Moves the camera down
  /// </summary>
  /// <param name="distance">The world distance to move the camera down</param>
  void MoveDown(float distance) noexcept;

  /// <summary>
  /// Sets the camera's yaw
  /// </summary>
  /// <param name="degrees">Yaw in degrees</param>
  void SetYaw(float degrees);

  /// <summary>
  /// Sets the camera's pitch
  /// </summary>
  /// <param name="degrees">Pitch in degrees</param>
  void SetPitch(float degrees);

  /// <summary>
  /// Sets the camera's roll
  /// </summary>
  /// <param name="degrees">Roll in degrees</param>
  void SetRoll(float degrees);

  /// <summary>
  /// Sets the viewing data for the camera
  /// </summary>
  /// <param name="viewData">FOV, Aspect Ratio, Culling Distance</param>
  void SetViewData(const ViewData& viewData);

  /// <summary>
  /// Gets the camera's position in world space
  /// </summary>
  /// <returns>[Const Ref] The camera's position</returns>
  const vec3& GetPosition() const noexcept;

  /// <summary>
  /// Gets the camera's forward vector
  /// </summary>
  /// <returns>[Const Ref] The camera's forward vector</returns>
  const vec3& GetForwardVector() const noexcept;

  /// <summary>
  /// Gets the camera's up vector
  /// </summary>
  /// <returns>[Const Ref] The camera's up vector</returns>
  const vec3& GetUpVector() const noexcept;

  /// <summary>
  /// Makes the Camera look at a target position
  /// </summary>
  /// <param name="position">Vec3 World Position</param>
  void LookAt(const vec3& forward, const vec3& up);

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
  void updateOrientation() noexcept;

  // Camera Viewing Data
  ViewData m_ViewData;

  // Camera world data
  vec3 m_Position;
  vec3 m_Forward;
  vec3 m_Up;

  // Euler Rotation
  float m_Pitch;
  float m_Roll;
  float m_Yaw;

  // Matrices
  mat4 m_PersMatrix;
  mat4 m_ViewMatrix;
  mat4 m_VPMatrix;

  bool m_IsEnabled;
  bool m_PerspectiveIsDirty;
  bool m_ViewIsDirty;

  // Camera Target
  const Transform* m_Target;

  // Camera Name
  string m_Name;
};
