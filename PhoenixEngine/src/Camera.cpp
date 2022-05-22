//------------------------------------------------------------------------------
// File:    Camera.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    The Camera class adds all necessary functionality for Camera
//          representation and interaction with OpenGL
//------------------------------------------------------------------------------
#include "pch.h"
#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective
#include <glm/ext/matrix_transform.hpp>   // glm::translate, glm::rotate, glm::scale
#include "Transform.h"

Camera::Camera(string Name, const Viewport& Viewport) noexcept :
  m_Position({ 0.f, 0.f, 30.f }),
  m_Forward({ 0.f, 0.f, -1.f }),
  m_Up({ 0.f, 1.f, 0.f }),
  m_Pitch(0.f),
  m_Roll(0.f),
  m_Yaw(0.f),
  m_PersMatrix(1.f),
  m_ViewMatrix(1.f),
  m_VPMatrix(1.f),
  m_bPerspectiveIsDirty(true),
  m_bViewIsDirty(true),
  m_Target(nullptr),
  m_Name(std::move(Name)),
  m_Viewport(Viewport)
{
  Log::Trace("Camera '" + m_Name + "' created.");
}

Camera::~Camera()
{
  Log::Trace(string("Camera '") + m_Name + "' destroyed.");
}

const mat4& Camera::GetPersMatrix() noexcept
{
  // Only update the matrix if dirty
  if (m_bPerspectiveIsDirty)
  {
    m_PersMatrix =
      glm::perspective(
        glm::radians(m_ViewData.FOV),
        m_ViewData.Aspect,
        m_ViewData.NearCull,
        m_ViewData.FarCull);
    m_bPerspectiveIsDirty = false;
  }
  return m_PersMatrix;
}

const mat4& Camera::GetViewMatrix() noexcept
{
  // Convenience of targeting, the camera can have a follow target
  if (m_Target != nullptr)
  {
    m_Forward = glm::normalize(m_Target->GetPosition() - m_Position);
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
  }
  else if (m_bViewIsDirty)
  {
    //TODO: Set up Pitch,Roll,Yaw
    //updateOrientation();
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    m_bViewIsDirty = false;
  }
  return m_ViewMatrix;
}

const glm::mat4& Camera::GetVPMatrix() noexcept
{
  if (m_bPerspectiveIsDirty || m_bViewIsDirty)
  {
    m_VPMatrix = GetPersMatrix() * GetViewMatrix();
  }

  return m_VPMatrix;
}

void Camera::SetTarget(const Transform* Target) noexcept
{
  m_Target = Target;
  m_bViewIsDirty = true;
}

void Camera::ClearTarget() noexcept
{
  m_Target = nullptr;
  m_bViewIsDirty = true;
}

void Camera::SetPosition(const vec3 Position)
{
  m_Position = Position;
  m_bViewIsDirty = true;
}

void Camera::MoveForward(const float Distance) noexcept
{
  m_Position += Distance * m_Forward;
  m_bViewIsDirty = true;
}

void Camera::MoveBackward(const float Distance) noexcept
{
  MoveForward(-Distance);
}

void Camera::MoveRight(const float Distance) noexcept
{
  m_Position += Distance * glm::cross(m_Forward, m_Up);
  m_bViewIsDirty = true;
}

void Camera::MoveLeft(const float Distance) noexcept
{
  MoveRight(-Distance);
}

void Camera::MoveUp(const float Distance) noexcept
{
  m_Position += Distance * m_Up;
  m_bViewIsDirty = true;
}

void Camera::MoveDown(const float Distance) noexcept
{
  MoveUp(-Distance);
}

void Camera::SetYaw(const float Degrees)
{
  m_Yaw = Degrees;
  m_bViewIsDirty = true;
}

void Camera::SetPitch(const float Degrees)
{
  m_Pitch = Degrees;
  m_bViewIsDirty = true;
}

void Camera::SetRoll(const float Degrees)
{
  m_Roll = Degrees;
  m_bViewIsDirty = true;
}

void Camera::SetViewData(const ViewData& ViewData)
{
  m_ViewData = ViewData;
}

const vec3& Camera::GetPosition() const noexcept
{
  return m_Position;
}

const vec3& Camera::GetForwardVector() const noexcept
{
  return m_Forward;
}

const vec3& Camera::GetUpVector() const noexcept
{
  return m_Up;
}

void Camera::LookAt(const vec3& Forward, const vec3& Up)
{
  if (glm::dot(Forward, Up) != 0.f)
  {
    Log::Warn("[Camera.cpp] Invalid vectors given to LookAt.");
    return;
  }

  m_Forward = Forward;
  m_Up = Up;
  m_bViewIsDirty = true;
}

void Camera::SetName(const string& Name) noexcept
{
  m_Name = Name;
}

const string& Camera::GetName() const noexcept
{
  return m_Name;
}

const Camera::Viewport& Camera::GetViewport() const noexcept
{
  return m_Viewport;
}

void Camera::SetViewport(const Camera::Viewport& Other) noexcept
{
  m_Viewport = Other;
}

void Camera::SetViewport(const GLint X, const GLint Y, const GLint W, const GLint H)
{
  m_Viewport.X = X;
  m_Viewport.Y = Y;
  m_Viewport.W = W;
  m_Viewport.H = H;
}

void Camera::updateOrientation() noexcept
{
  m_Forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Forward.y = sin(glm::radians(m_Pitch));
  m_Forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Forward = glm::normalize(m_Forward);
}
