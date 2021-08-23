#include "pch.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include "Transform.h"

using glm::vec3;
using glm::mat4;

Camera::Camera() noexcept :
  m_ViewData(),
  m_Position({ 0.f, 0.f, 3.f }),
  m_Forward({ 0.f, 0.f, -1.f }),
  m_Up({ 0.f, 1.f, 0.f }),
  m_PersMatrix(1.f),
  m_ViewMatrix(1.f),
  m_IsEnabled(false),
  m_bProjectionIsDirty(true),
  m_bViewIsDirty(true),
  m_PersAttributeID(-1),
  m_ViewAttributeID(-1),
  m_Target(nullptr)
{
  Log::Trace("Camera created.");
}

void Camera::Init(GLuint program) noexcept
{
  m_PersAttributeID = glGetUniformLocation(program, "pers_matrix");
  m_ViewAttributeID = glGetUniformLocation(program, "view_matrix");
}

void Camera::Update(float dt) noexcept
{
  if (!m_IsEnabled)
    return;
}

void Camera::Bind() noexcept
{
  if (!m_IsEnabled)
    return;

  // Set View Matrix
  glUniformMatrix4fv(m_ViewAttributeID, 1, false, &GetViewMatrix()[0][0]);
}

void Camera::EnableCamera() noexcept
{
  // Set Perspective Matrix
  glUniformMatrix4fv(m_PersAttributeID, 1, false, &GetPersMatrix()[0][0]);

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
