//------------------------------------------------------------------------------
// File:    Transform.cpp
// Author:  Ryan Buehler
// Created: Friday, May 20, 2022
// Desc:    Handles the Euclidean 3D Transform
//------------------------------------------------------------------------------
#include "pch.h"
#include "Transform.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtx/rotate_vector.hpp>

Transform::Transform() noexcept :
  m_Position(0.f),
  m_Rotation(0.f),
  m_Scale(1.f),
  m_Matrix(1.f),
  m_bIsDirty(true)
{}

Transform::Transform(const Transform& Other) :
  m_Position(Other.m_Position),
  m_Rotation(Other.m_Rotation),
  m_Scale(Other.m_Scale),
  m_Matrix(Other.m_Matrix),
  m_bIsDirty(true)
{
  *this = Other;

}

Transform& Transform::operator=(const Transform& Other)
{
  m_Position = Other.m_Position;
  m_Rotation = Other.m_Rotation;
  m_Scale = Other.m_Scale;
  m_Matrix = Other.m_Matrix;
  m_bIsDirty = true;
  return *this;
}

void Transform::Translate(const vec3& Translation)
{
  m_Position += Translation;
  m_bIsDirty = true;
}

void Transform::RotateX(const float Degrees)
{
  m_Rotation.x += Degrees;
  m_bIsDirty = true;
}

void Transform::RotateY(const float Degrees)
{
  m_Rotation.y += Degrees;
  m_bIsDirty = true;
}

void Transform::RotateZ(const float Degrees)
{
  m_Rotation.z += Degrees;
  m_bIsDirty = true;
}

void Transform::RotateAround(const float Degrees, const vec3 Axis)
{
  m_Position = glm::rotate(m_Position, glm::radians(Degrees), Axis);
}

void Transform::ScaleBy(const float Factor)
{
  m_Scale *= Factor;
  m_bIsDirty = true;
}

void Transform::SetPosition(const vec3& Position) noexcept
{
  if (Position == m_Position) return;
  m_Position = Position;
  m_bIsDirty = true;
}

void Transform::SetRotation(const vec3& Rotation) noexcept
{
  if (Rotation == m_Rotation) return;
  m_Rotation = Rotation;
  m_bIsDirty = true;
}

void Transform::SetScale(const vec3& Scale) noexcept
{
  if (Scale == m_Scale) return;
  m_Scale = Scale;
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
