//------------------------------------------------------------------------------
// File:    Camera.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    The Camera class adds all necessary functionality for Camera
//          representation and interaction with OpenGL
//------------------------------------------------------------------------------
#include "pch.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>   // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective
#include <glm/ext/scalar_constants.hpp>   // glm::pi
#include "Transform.h"

Camera::Camera(const string& name) noexcept :
  m_ViewData(),
  m_Position({ 0.f, 0.f, 30.f }),
  m_Forward({ 0.f, 0.f, -1.f }),
  m_Up({ 0.f, 1.f, 0.f }),
  m_Pitch(0.f),
  m_Roll(0.f),
  m_Yaw(0.f),
  m_PersMatrix(1.f),
  m_ViewMatrix(1.f),
  m_IsEnabled(false),
  m_PerspectiveIsDirty(true),
  m_ViewIsDirty(true),
  m_Target(nullptr),
  m_Name(name)
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
  if (m_PerspectiveIsDirty)
  {
    m_PersMatrix =
      glm::perspective(
        glm::radians(m_ViewData.FOV),
        m_ViewData.Aspect,
        m_ViewData.NearCull,
        m_ViewData.FarCull);
    m_PerspectiveIsDirty = false;
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
  else if (m_ViewIsDirty)
  {
    //TODO: Set up Pitch,Roll,Yaw
    //updateOrientation();
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    m_ViewIsDirty = false;
  }
  return m_ViewMatrix;
}

const glm::mat4& Camera::GetVPMatrix() noexcept
{
  if (m_PerspectiveIsDirty || m_ViewIsDirty)
  {
    m_VPMatrix = GetPersMatrix() * GetViewMatrix();
  }

  return m_VPMatrix;
}

void Camera::SetTarget(const Transform* target) noexcept
{
  m_Target = target;
  m_ViewIsDirty = true;
}

void Camera::ClearTarget() noexcept
{
  m_Target = nullptr;
  m_ViewIsDirty = true;
}

void Camera::SetPosition(vec3 position)
{
  m_Position = position;
  m_ViewIsDirty = true;
}

void Camera::MoveForward(float distance) noexcept
{
  m_Position += distance * m_Forward;
  m_ViewIsDirty = true;
}

void Camera::MoveBackward(float distance) noexcept
{
  MoveForward(-distance);
}

void Camera::MoveRight(float distance) noexcept
{
  m_Position += distance * glm::cross(m_Forward, m_Up);
  m_ViewIsDirty = true;
}

void Camera::MoveLeft(float distance) noexcept
{
  MoveRight(-distance);
}

void Camera::MoveUp(float distance) noexcept
{
  m_Position += distance * m_Up;
  m_ViewIsDirty = true;
}

void Camera::MoveDown(float distance) noexcept
{
  MoveUp(-distance);
}

void Camera::SetYaw(float degrees)
{
  m_Yaw = degrees;
  m_ViewIsDirty = true;
}

void Camera::SetPitch(float degrees)
{
  m_Pitch = degrees;
  m_ViewIsDirty = true;
}

void Camera::SetRoll(float degrees)
{
  m_Roll = degrees;
  m_ViewIsDirty = true;
}

void Camera::SetViewData(const ViewData& viewData)
{
  m_ViewData = viewData;
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

void Camera::LookAt(const vec3& forward, const vec3& up)
{
  if (glm::dot(forward, up) != 0)
  {
    Log::Warn("[Camera.cpp] Invalid vectors given to LookAt.");
    return;
  }

  m_Forward = forward;
  m_Up = up;
  m_ViewIsDirty = true;
}

void Camera::SetName(const string& name) noexcept
{
  m_Name = name;
}

const string& Camera::GetName() const noexcept
{
  return m_Name;
}

void Camera::updateOrientation() noexcept
{
  m_Forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Forward.y = sin(glm::radians(m_Pitch));
  m_Forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Forward = glm::normalize(m_Forward);
}
