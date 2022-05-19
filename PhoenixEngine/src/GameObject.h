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

  GameObject(/*const string& meshFileName = "cube.obj"*/) noexcept;
  ~GameObject() = default;
  GameObject(const GameObject&) noexcept;
  GameObject& operator=(const GameObject&) noexcept;
  GameObject(GameObject&&) noexcept;
  GameObject& operator=(GameObject&&) noexcept;

  void SetTransform(const Transform& transform);
  const Transform& GetTransform() const noexcept;

  //void SetMeshFileName(const string& fileName) noexcept;
  //inline const string& GetMeshFileName() const noexcept { return m_MeshFileName; }

  void SetIsActive(const bool IsActive) noexcept { m_bIsActive = IsActive; }
  bool IsActive() const noexcept { return m_bIsActive; }

#pragma region Transform Interface

  void Translate(const vec3& translation) { m_Transform.Translate(translation); }
  void RotateX(float degrees) { m_Transform.RotateX(degrees); }
  void RotateY(float degrees) { m_Transform.RotateY(degrees); }
  void RotateZ(float degrees) { m_Transform.RotateZ(degrees); }

  void RotateAround(float degrees, const vec3& axis)
  {
    m_Transform.RotateAround(degrees, axis);
  }

  void ScaleBy(float factor) { m_Transform.ScaleBy(factor); }

  void SetPosition(const vec3& pos) noexcept { m_Transform.SetPosition(pos); }
  void SetRotation(const vec3& rot) noexcept { m_Transform.SetRotation(rot); }
  void SetScale(const vec3& sca) noexcept { m_Transform.SetScale(sca); }

  const vec3& GetPosition() const noexcept { return m_Transform.GetPosition(); }
  const vec3& GetRotation() const noexcept { return m_Transform.GetRotation(); }
  const vec3& GetScale() const noexcept { return m_Transform.GetScale(); }

  const mat4& GetMatrix() noexcept { return m_Transform.GetMatrix(); }

#pragma endregion

  //inline void SetMaterial(const Material& material) noexcept { m_Material = material; }
  //inline const Material& GetMaterial() noexcept { return m_Material; }
  
  void SetMaterial(const Material& material) noexcept;


#pragma region Component

  /// <summary>
  /// Adds a component to a game object
  /// </summary>
  /// <param name="type">The type of component</param>
  shared_ptr<Component> AddComponent(Component::Type type) noexcept;
  shared_ptr<Component> AddComponent(Component& component) noexcept;

  optional<shared_ptr<Component>> GetFirstComponentByType(Component::Type type) noexcept;
  optional<shared_ptr<Component>> GetLastComponentByType(Component::Type type) noexcept;
  optional<shared_ptr<Component>> GetAnyComponentByType(Component::Type type) noexcept;

#pragma endregion


  private:
  friend class Renderer;

  Transform m_Transform;

  //TODO: Move this into a "RenderableComponent"
  //unsigned m_MeshID;
  //string m_MeshFileName;

  bool m_bIsActive;
  bool m_bIsDirty;

  array<vector<shared_ptr<Component>>, Utility::ID(Component::Type::COUNT)> m_Components;
  //vector<unique_ptr<Component>> m_Components;
};