#pragma once
#include "IBindable.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include "GLEW/glew.h"

// Forward Declarations
class Transform;

class Camera : public IBindable
{
public:
  struct ViewData
  {
    ViewData(
      float fov = 45.f,
      float aspect = 16.f / 9.f,
      float nearCull = 0.1f,
      float farCull = 1000.f
    ) :
    FOV(fov), Aspect(aspect), NearCull(nearCull), FarCull(farCull) {}

    float FOV;
    float Aspect;
    float NearCull;
    float FarCull;
  };
public:

  Camera() noexcept;
  ~Camera() = default;
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;
  Camera(Camera&&) = delete;
  Camera& operator=(Camera&&) = delete;

  void Init(GLuint program) noexcept;
  void Update(float dt) noexcept;
  void Bind() noexcept;

  void EnableCamera() noexcept;
  void DisableCamera() noexcept;

  const glm::mat4& GetPersMatrix() noexcept;
  const glm::mat4& GetViewMatrix() noexcept;

  void SetTarget(Transform* target) noexcept;
  void ClearTarget() noexcept;

private:
  // Camera Viewing Data
  ViewData m_ViewData;

  // Camera world data
  glm::vec3 m_Position;
  glm::vec3 m_Forward;
  glm::vec3 m_Up;

  // Matrices
  glm::mat4 m_PersMatrix;
  glm::mat4 m_ViewMatrix;

  bool m_IsEnabled;
  bool m_bProjectionIsDirty;
  bool m_bViewIsDirty;

  // Shader Data
  GLint m_PersAttributeID;
  GLint m_ViewAttributeID;

  // Camera Target
  Transform* m_Target;
};
