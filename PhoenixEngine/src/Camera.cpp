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
  m_PersMatrix(1.f),
  m_ViewMatrix(1.f),
  m_IsEnabled(false),
  m_bProjectionIsDirty(true),
  m_bViewIsDirty(true),
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
  if (m_bProjectionIsDirty)
  {
    m_PersMatrix =
      glm::perspective(
        glm::radians(m_ViewData.FOV),
        m_ViewData.Aspect,
        m_ViewData.NearCull,
        m_ViewData.FarCull);
    m_bProjectionIsDirty = false;
  }
  return m_PersMatrix;
}

const mat4& Camera::GetViewMatrix() noexcept
{
  // Convenience of targeting, the camera can have a follow target
  if (m_Target != nullptr)
  {
    m_Forward = glm::normalize(m_Target->GetPosition() - m_Position);
    m_ViewMatrix = glm::lookAt(m_Position, m_Forward, m_Up);
  }
  else if (m_bViewIsDirty)
  {
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    m_bViewIsDirty = false;
  }
  return m_ViewMatrix;
}

void Camera::SetTarget(const Transform* target) noexcept
{
  m_Target = target;
  m_bViewIsDirty = true;
}

void Camera::ClearTarget() noexcept
{
  m_Target = nullptr;
  m_bViewIsDirty = true;
}

void Camera::SetPosition(vec3 position)
{
  m_Position = position;
  m_bViewIsDirty = true;
}

void Camera::MoveForward(float distance) noexcept
{
  m_Position += distance * m_Forward;
  m_bViewIsDirty = true;
}

void Camera::MoveBackward(float distance) noexcept
{
  MoveForward(-distance);
}

void Camera::MoveRight(float distance) noexcept
{
  m_Position += distance * glm::cross(m_Forward, m_Up);
  m_bViewIsDirty = true;
}

void Camera::MoveLeft(float distance) noexcept
{
  MoveRight(-distance);
}

void Camera::MoveUp(float distance) noexcept
{
  m_Position += distance * m_Up;
  m_bViewIsDirty = true;
}

void Camera::MoveDown(float distance) noexcept
{
  MoveUp(-distance);
}

void Camera::SetYaw(float degrees)
{
  Log::Error("Camera::SetYaw Not implemented yet.");
}

void Camera::SetPitch(float degrees)
{
  Log::Error("Camera::SetPitch Not implemented yet.");
}

void Camera::SetRoll(float degrees)
{
  Log::Error("Camera::SetRoll Not implemented yet.");
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

void Camera::LookAt(vec3 position)
{
  m_Forward = glm::normalize(position - m_Position);
}

void Camera::SetName(const string& name) noexcept
{
  m_Name = name;
}

const string& Camera::GetName() const noexcept
{
  return m_Name;
}
