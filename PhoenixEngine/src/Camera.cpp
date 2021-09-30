//------------------------------------------------------------------------------
// File:    Camera.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    The Camera class adds all necessary functionality for Camera
//          representation and interaction with OpenGL
//------------------------------------------------------------------------------
#include "pch.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include "Transform.h"

Camera::Camera(const string& name) noexcept :
  m_ViewData(),
  m_Position({ 0.f, 0.f, 3.f }),
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
  Log::Trace("Camera created.");
}

Camera::~Camera()
{
  Log::Trace(string("Camera '") + m_Name + "' destroyed.");
}

void Camera::Update(float dt) noexcept
{
  if (!m_IsEnabled)
    return;
}

void Camera::EnableCamera() noexcept
{
  m_IsEnabled = true;
}

void Camera::DisableCamera() noexcept
{
  m_IsEnabled = false;
}

const mat4& Camera::GetPersMatrix() noexcept
{
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
  if (m_Target != nullptr)
  {
    m_ViewMatrix = glm::lookAt(m_Position, glm::normalize(m_Target->GetPosition()), m_Up);
  }
  else if (m_bViewIsDirty)
  {
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    m_bViewIsDirty = false;
  }
  return m_ViewMatrix;
}

void Camera::SetTarget(Transform* target) noexcept
{
  m_Target = target;
}

void Camera::ClearTarget() noexcept
{
  m_Target = nullptr;
}

void Camera::SetPosition(vec3 position)
{
  m_Position = position;
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
