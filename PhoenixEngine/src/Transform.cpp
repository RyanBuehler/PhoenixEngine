#include "pch.h"
#include "Transform.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale

Transform::Transform() noexcept :
  m_Position(0.f),
  m_Rotation(0.f),
  m_Scale(0.25f),
  m_Matrix(1.f),
  m_bIsDirty(true)
{}

Transform::Transform(const Transform& other) :
  m_Position(other.m_Position),
  m_Rotation(other.m_Rotation),
  m_Scale(other.m_Scale),
  m_Matrix(other.m_Matrix),
  m_bIsDirty(true)
{
  *this = other;
}

Transform& Transform::operator=(const Transform& other)
{
  m_Position = other.m_Position;
  m_Rotation = other.m_Rotation;
  m_Scale = other.m_Scale;
  m_Matrix = other.m_Matrix;
  m_bIsDirty = true;
  return *this;
}

//void Transform::Init(GLuint program) noexcept
//{
//  m_ModelAttributeID = glGetUniformLocation(program, "model_matrix");
//}
//
//void Transform::Bind() noexcept
//{
//  glUniformMatrix4fv(m_ModelAttributeID, 1, false, &GetMatrix()[0][0]);
//}

void Transform::Translate(const vec3& translation)
{
  m_Position += translation;
  m_bIsDirty = true;
}

void Transform::RotateX(float degrees)
{
  m_Rotation.x += degrees;
  m_bIsDirty = true;
}

void Transform::RotateY(float degrees)
{
  m_Rotation.y += degrees;
  m_bIsDirty = true;
}

void Transform::RotateZ(float degrees)
{
  m_Rotation.z += degrees;
  m_bIsDirty = true;
}

void Transform::ScaleBy(float factor)
{
  m_Scale *= factor;
  m_bIsDirty = true;
}

void Transform::SetPosition(const vec3& pos) noexcept
{
  if (pos == m_Position) return;
  m_Position = pos;
  m_bIsDirty = true;
}

void Transform::SetRotation(const vec3& rot) noexcept
{
  if (rot == m_Rotation) return;
  m_Rotation = rot;
  m_bIsDirty = true;
}

void Transform::SetScale(const vec3& sca) noexcept
{
  if (sca == m_Scale) return;
  m_Scale = sca;
  m_bIsDirty = true;
}

const vec3& Transform::GetPosition() const noexcept
{
  return m_Position;
}

const vec3& Transform::GetRotation() const noexcept
{
  return m_Rotation;
}

const vec3& Transform::GetScale() const noexcept
{
  return m_Scale;
}

const mat4& Transform::GetMatrix() noexcept
{
  if (m_bIsDirty)
  {
    m_Matrix = glm::translate(mat4(1.f), m_Position);
    m_Matrix = glm::rotate(m_Matrix, glm::radians(m_Rotation.x), { 1.f, 0.f, 0.f });
    m_Matrix = glm::rotate(m_Matrix, glm::radians(m_Rotation.y), { 0.f, 1.f, 0.f });
    m_Matrix = glm::rotate(m_Matrix, glm::radians(m_Rotation.z), { 0.f, 0.f, 1.f });
    m_Matrix = glm::scale(m_Matrix, m_Scale);
  }

  return m_Matrix;
}
