//------------------------------------------------------------------------------
// File:    GameObject.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    The universal object to display in engine
//------------------------------------------------------------------------------
#pragma once
#include "Transform.h"
#include "Material.h"
#include "Component.h"

class GameObject
{
  public:

  GameObject() noexcept;
  ~GameObject() = default;
  GameObject(const GameObject&) noexcept;
  GameObject& operator=(const GameObject&) noexcept;
  GameObject(GameObject&&) noexcept;
  GameObject& operator=(GameObject&&) noexcept;

  void SetTransform(const Transform& Transform);
  [[nodiscard]] const Transform& GetTransform() const noexcept;

  void SetIsActive(const bool IsActive) noexcept { m_bIsActive = IsActive; }
  [[nodiscard]] bool IsActive() const noexcept { return m_bIsActive; }

#pragma region Transform Interface

  void Translate(const vec3& Translation) { m_Transform.Translate(Translation); }
  void RotateX(const float Degrees) { m_Transform.RotateX(Degrees); }
  void RotateY(const float Degrees) { m_Transform.RotateY(Degrees); }
  void RotateZ(const float Degrees) { m_Transform.RotateZ(Degrees); }

  void RotateAround(const float Degrees, const vec3& Axis)
  {
    m_Transform.RotateAround(Degrees, Axis);
  }

  void ScaleBy(const float Factor) { m_Transform.ScaleBy(Factor); }

  void SetPosition(const vec3& Pos) noexcept { m_Transform.SetPosition(Pos); }
  void SetRotation(const vec3& Rot) noexcept { m_Transform.SetRotation(Rot); }
  void SetScale(const vec3& Sca) noexcept { m_Transform.SetScale(Sca); }

  [[nodiscard]] const vec3& GetPosition() const noexcept { return m_Transform.GetPosition(); }
  [[nodiscard]] const vec3& GetRotation() const noexcept { return m_Transform.GetRotation(); }
  [[nodiscard]] const vec3& GetScale() const noexcept { return m_Transform.GetScale(); }

  const mat4& GetMatrix() noexcept { return m_Transform.GetMatrix(); }

#pragma endregion

  void SetMaterial(const Material& Mat) noexcept;


#pragma region Component

  /// <summary>
  /// Adds a component to a game object
  /// </summary>
  /// <param name="ComponentType">The type of component</param>
  shared_ptr<Component> AddComponent(Component::Type ComponentType) noexcept;
  shared_ptr<Component> AddComponent(const Component& Component) noexcept;

  optional<shared_ptr<Component>> GetFirstComponentByType(Component::Type ComponentType) noexcept;
  static optional<shared_ptr<Component>> GetLastComponentByType(Component::Type ComponentType) noexcept;
  optional<shared_ptr<Component>> GetAnyComponentByType(Component::Type ComponentType) noexcept;

#pragma endregion


  private:
  friend class Renderer;

  Transform m_Transform;

  bool m_bIsActive;
  bool m_bIsDirty;

  array<vector<shared_ptr<Component>>, Utility::ID(Component::Type::COUNT)> m_Components;
};