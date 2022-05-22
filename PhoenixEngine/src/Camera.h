//------------------------------------------------------------------------------
// File:    Camera.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Camera class declarations
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

// Forward declarations
class Transform;

class Camera
{
public:
  struct Viewport
  {
    Viewport(
      const GLint X = 0,
      const GLint Y = 0,
      const GLint W = 1920,
      const GLint H = 1080
    ) : X(X), Y(Y), W(W), H(H) {}

    GLint X;
    GLint Y;
    GLint W;
    GLint H;
  };

  struct ViewData
  {
    ViewData(
      const float Fov = 45.f,
      const float Aspect = 16.f / 9.f,
      const float NearCull = 0.1f,
      const float FarCull = 1000.f
    ) :
      FOV(Fov), Aspect(Aspect), NearCull(NearCull), FarCull(FarCull) {}

    float FOV;
    float Aspect;
    float NearCull;
    float FarCull;
  };

  /// <summary>
  /// Camera Constructor
  /// </summary>
  /// <param name="Name">Name of the camera</param>
  /// <param name="Viewport">Viewport settings</param>
  Camera(string Name = "Unnamed Camera", const Viewport& Viewport = {0,0,1920,1080}) noexcept;

  /// <summary>
  /// The Camera destructor
  /// </summary>
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
  /// <param name="Target">The transform ptr of the target to watch.</param>
  void SetTarget(const Transform* Target) noexcept;

  /// <summary>
  /// Clears the "LookAt" target
  /// </summary>
  void ClearTarget() noexcept;

  /// <summary>
  /// Sets the Camera's position
  /// </summary>
  /// <param name="Position">Vec3 World Position</param>
  void SetPosition(vec3 Position);

  /// <summary>
  /// Moves the camera forward
  /// </summary>
  /// <param name="Distance">The world distance to move the camera forward</param>
  void MoveForward(float Distance) noexcept;

  /// <summary>
  /// Moves the camera backward
  /// </summary>
  /// <param name="Distance">The world distance to move the camera backward</param>
  void MoveBackward(float Distance) noexcept;

  /// <summary>
  /// Moves the camera directly to the camera's right
  /// </summary>
  /// <param name="Distance">The world distance to move the camera right</param>
  void MoveRight(float Distance) noexcept;

  /// <summary>
  /// Moves the camera directly to the camera's left
  /// </summary>
  /// <param name="Distance">The world distance to move the camera left</param>
  void MoveLeft(float Distance) noexcept;

  /// <summary>
  /// Moves the camera up
  /// </summary>
  /// <param name="Distance">The world distance to move the camera up</param>
  void MoveUp(float Distance) noexcept;

  /// <summary>
  /// Moves the camera down
  /// </summary>
  /// <param name="Distance">The world distance to move the camera down</param>
  void MoveDown(float Distance) noexcept;

  /// <summary>
  /// Sets the camera's yaw
  /// </summary>
  /// <param name="Degrees">Yaw in degrees</param>
  void SetYaw(float Degrees);

  /// <summary>
  /// Sets the camera's pitch
  /// </summary>
  /// <param name="Degrees">Pitch in degrees</param>
  void SetPitch(float Degrees);

  /// <summary>
  /// Sets the camera's roll
  /// </summary>
  /// <param name="Degrees">Roll in degrees</param>
  void SetRoll(float Degrees);

  /// <summary>
  /// Sets the viewing data for the camera
  /// </summary>
  /// <param name="ViewData">FOV, Aspect Ratio, Culling Distance</param>
  void SetViewData(const ViewData& ViewData);

  /// <summary>
  /// Gets the camera's position in world space
  /// </summary>
  /// <returns>[Const Ref] The camera's position</returns>
  [[nodiscard]] const vec3& GetPosition() const noexcept;

  /// <summary>
  /// Gets the camera's forward vector
  /// </summary>
  /// <returns>[Const Ref] The camera's forward vector</returns>
  [[nodiscard]] const vec3& GetForwardVector() const noexcept;

  /// <summary>
  /// Gets the camera's up vector
  /// </summary>
  /// <returns>[Const Ref] The camera's up vector</returns>
  [[nodiscard]] const vec3& GetUpVector() const noexcept;

  /// <summary>
  /// Set the camera's forward and up vector's directly
  /// </summary>
  /// <param name="Forward">Vec3 forward vector</param>
  /// <param name="Up">Vec3 forward vector</param>
  void LookAt(const vec3& Forward, const vec3& Up);

  /// <summary>
  /// Sets the camera's name
  /// </summary>
  /// <param name="Name">The new name for the Camera</param>
  void SetName(const string& Name) noexcept;

  /// <summary>
  /// Gets the camera's current name
  /// </summary>
  /// <returns>The camera's current name</returns>
  [[nodiscard]] const string& GetName() const noexcept;

  /// <summary>
  /// Gets the viewport object
  /// </summary>
  /// <returns>The viewport object</returns>
  [[nodiscard]] const Viewport& GetViewport() const noexcept;

  void SetViewport(const Viewport& Other) noexcept;

  /// <summary>
  /// Set the viewport data directly
  /// </summary>
  /// <param name="X">The 'x' coordinate of the viewport</param>
  /// <param name="Y">The 'y' coordinate of the viewport</param>
  /// <param name="W">The width of the viewport</param>
  /// <param name="H">The height of the viewport</param>
  void SetViewport(GLint X, GLint Y, GLint W, GLint H);

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

  bool m_bPerspectiveIsDirty;
  bool m_bViewIsDirty;

  // Camera Target
  const Transform* m_Target;

  // Camera Name
  string m_Name;

  // Viewport data
  Viewport m_Viewport;
};
