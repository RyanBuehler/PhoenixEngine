//------------------------------------------------------------------------------
// File:    GameObject.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    The universal object to display in engine
//------------------------------------------------------------------------------
#pragma once
#include "Transform.h"

class GameObject
{
public:

  GameObject(const string& meshFileName = "cube.obj") noexcept;
  ~GameObject() = default;
  GameObject(const GameObject&) noexcept;
  GameObject& operator=(const GameObject&) noexcept;
  GameObject(GameObject&&) noexcept;
  GameObject& operator=(GameObject&&) noexcept;

  void SetTransform(const Transform& transform);
  const Transform& GetTransform() const noexcept;

  inline void SetMeshFileName(const string& fileName) noexcept { m_MeshFileName = fileName; }
  inline const string& GetMeshFileName() const noexcept { return m_MeshFileName; }

  inline void SetIsActive(bool isActive) noexcept { m_bIsActive = isActive; }
  inline bool IsActive() const noexcept { return m_bIsActive; }

#pragma region Transform Interface

  inline void Translate(const vec3& translation) { m_Transform.Translate(translation); }
  inline void RotateX(float degrees) { m_Transform.RotateX(degrees); }
  inline void RotateY(float degrees) { m_Transform.RotateY(degrees); }
  inline void RotateZ(float degrees) { m_Transform.RotateZ(degrees); }
  inline void ScaleBy(float factor) { m_Transform.ScaleBy(factor); }

  inline void SetPosition(const vec3& pos) noexcept { m_Transform.SetPosition(pos); }
  inline void SetRotation(const vec3& rot) noexcept { m_Transform.SetRotation(rot); }
  inline void SetScale(const vec3& sca) noexcept { m_Transform.SetScale(sca); }

  inline const vec3& GetPosition() const noexcept { return m_Transform.GetPosition(); }
  inline const vec3& GetRotation() const noexcept { return m_Transform.GetRotation(); }
  inline const vec3& GetScale() const noexcept { return m_Transform.GetScale(); }

  inline const mat4& GetMatrix() noexcept { return m_Transform.GetMatrix(); }

#pragma endregion

private:
  friend class MeshRenderer;

  Transform m_Transform;
  unsigned m_MeshID;
  string m_MeshFileName;

  bool m_bIsActive;
  bool m_bIsDirty;
};
