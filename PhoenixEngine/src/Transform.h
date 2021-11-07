#pragma once
#include "GLEW/glew.h"
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

  //TODO: noexcept?
  void Translate(const vec3& translation);
  void RotateX(float degrees);
  void RotateY(float degrees);
  void RotateZ(float degrees);

  void RotateAround(float degrees, vec3 axis);

  void ScaleBy(float factor);

  void SetPosition(const vec3& pos) noexcept;
  void SetRotation(const vec3& rot) noexcept;
  void SetScale(const vec3& sca) noexcept;

  const vec3& GetPosition() const noexcept;
  const vec3& GetRotation() const noexcept;
  const vec3& GetScale() const noexcept;

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
