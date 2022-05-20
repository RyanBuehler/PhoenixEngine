//------------------------------------------------------------------------------
// File:    Transform.h
// Author:  Ryan Buehler
// Created: Friday, May 20, 2022
// Desc:    Handles the Euclidean 3D Transform
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

class Transform
{
public:
  Transform() noexcept;
  ~Transform() = default;
  Transform(const Transform&);
  Transform& operator=(const Transform&);
  Transform(Transform&&) = delete;
  Transform& operator=(Transform&&) = delete;

  void Translate(const vec3& Translation);
  void RotateX(float Degrees);
  void RotateY(float Degrees);
  void RotateZ(float Degrees);

  void RotateAround(float Degrees, vec3 Axis);

  void ScaleBy(float Factor);

  void SetPosition(const vec3& Position) noexcept;
  void SetRotation(const vec3& Rotation) noexcept;
  void SetScale(const vec3& Scale) noexcept;

  [[nodiscard]] const vec3& GetPosition() const noexcept;
  [[nodiscard]] const vec3& GetRotation() const noexcept;
  [[nodiscard]] const vec3& GetScale() const noexcept;

  const mat4& GetMatrix() noexcept;

private:
  vec3 m_Position;
  vec3 m_Rotation;
  vec3 m_Scale;

  mat4 m_Matrix;

  bool m_bIsDirty;

#ifdef _IMGUI
  friend class ImGuiManager;
#endif
};
