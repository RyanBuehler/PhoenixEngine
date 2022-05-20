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
#include "Transform.h"

Camera::Camera(const string& name, const Viewport& viewport) noexcept :
  m_ViewData(),
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
  m_Name(name),
  m_Viewport(viewport)
{
  Log::trace("Camera '" + m_Name + "' created.");
}

Camera::~Camera()
{
  Log::trace(string("Camera '") + m_Name + "' destroyed.");
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
  m_Yaw = degrees;
  m_bViewIsDirty = true;
}

void Camera::SetPitch(float degrees)
{
  m_Pitch = degrees;
  m_bViewIsDirty = true;
}

void Camera::SetRoll(float degrees)
{
  m_Roll = degrees;
  m_bViewIsDirty = true;
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

void Camera::LookAt(const vec3& Forward, const vec3& Up)
{
  if (glm::dot(Forward, Up) != 0.f)
  {
    Log::warn("[Camera.cpp] Invalid vectors given to LookAt.");
    return;
  }

  m_Forward = Forward;
  m_Up = Up;
  m_bViewIsDirty = true;
}

void Camera::SetName(const string& name) noexcept
{
  m_Name = name;
}

const string& Camera::GetName() const noexcept
{
  return m_Name;
}

const Camera::Viewport& Camera::GetViewport() const noexcept
{
  return m_Viewport;
}

void Camera::SetViewport(const Camera::Viewport& other) noexcept
{
  m_Viewport = other;
}

void Camera::SetViewport(GLint X, GLint Y, GLint W, GLint H)
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
