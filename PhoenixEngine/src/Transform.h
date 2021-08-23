#pragma once
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
//TODO:
#include "IBindable.h"
#include "GLEW/glew.h"

class Transform : public IBindable
{
public:

  Transform() noexcept;
  ~Transform() = default;
  Transform(const Transform&) = delete;
  Transform& operator=(const Transform&) = delete;
  Transform(Transform&&) = delete;
  Transform& operator=(Transform&&) = delete;

  void Init(GLuint program) noexcept;

  void Bind() noexcept;

  void Translate(const glm::vec3& translation);
  void RotateX(float degrees);
  void RotateY(float degrees);
  void RotateZ(float degrees);
  void ScaleBy(float factor);

  void SetPosition(const glm::vec3& pos) noexcept;
  void SetRotation(const glm::vec3& rot) noexcept;
  void SetScale(const glm::vec3& sca) noexcept;

  const glm::vec3& GetPosition() const noexcept;
  const glm::vec3& GetRotation() const noexcept;
  const glm::vec3& GetScale() const noexcept;

  const glm::mat4& GetMatrix() noexcept;

private:
  glm::vec3 m_Position;
  glm::vec3 m_Rotation;
  glm::vec3 m_Scale;

  glm::mat4 m_Matrix;

  bool m_bIsDirty;

  GLint m_ModelAttributeID;
};
