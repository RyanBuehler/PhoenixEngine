#include "pch.h"
#include "GameObject.h"
#include "MeshComponent.h"

using Utility::ID;
using CType = Component::Type;

GameObject::GameObject(const string& meshFileName) noexcept :
  m_Transform(),
  m_MeshID(Error::INVALID_INDEX),
  m_MeshFileName(meshFileName),
  m_Material(Material::Type::BASIC),
  m_bIsActive(true),
  m_bIsDirty(true)
{}

GameObject::GameObject(const GameObject& other) noexcept :
  m_Transform(other.m_Transform),
  m_MeshID(other.m_MeshID),
  m_MeshFileName(other.m_MeshFileName),
  m_Material(Material::Type::BASIC),
  m_bIsActive(other.m_bIsActive),
  m_bIsDirty(other.m_bIsDirty)
{
}

GameObject& GameObject::operator=(const GameObject& other) noexcept
{
  m_Transform = other.m_Transform;
  m_MeshID = other.m_MeshID;
  m_MeshFileName = other.m_MeshFileName;
  m_Material = other.m_Material;
  m_bIsActive = other.m_bIsActive;
  m_bIsDirty = other.m_bIsDirty;
  return *this;
}

GameObject::GameObject(GameObject&& other) noexcept :
  m_Transform(other.m_Transform),
  m_MeshID(other.m_MeshID),
  m_MeshFileName(other.m_MeshFileName),
  m_Material(other.m_Material),
  m_bIsActive(other.m_bIsActive),
  m_bIsDirty(other.m_bIsDirty)
{
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
  m_Transform = other.m_Transform;
  m_MeshID = other.m_MeshID;
  m_MeshFileName = other.m_MeshFileName;
  m_Material = other.m_Material;
  m_bIsActive = other.m_bIsActive;
  m_bIsDirty = other.m_bIsDirty;
  return *this;
}

void GameObject::SetTransform(const Transform& transform)
{
  m_Transform = transform;
}

const Transform& GameObject::GetTransform() const noexcept
{
  return m_Transform;
}

shared_ptr<Component> GameObject::AddComponent(Component::Type type) noexcept
{
  switch (type)
  {
    case CType::MESH:
      m_Components[ID(CType::MESH)].emplace_back(make_shared<MeshComponent>(*this));
    case CType::COUNT:
    default:
      Log::Error("[GameObject.cpp] Attempting to add component of invalid type.");
      break;
  }
  return nullptr;
}

shared_ptr<Component> GameObject::AddComponent(Component& component) noexcept
{
  switch (component.GetType())
  {
    case CType::MESH:
      m_Components[ID(CType::MESH)].emplace_back(make_shared<MeshComponent>(*this));
    case CType::COUNT:
    default:
      Log::Error("[GameObject.cpp] Attempting to add component of invalid type.");
      break;
  }
  return nullptr;
}

optional<shared_ptr<Component>> GameObject::GetFirstComponentByType(Component::Type type) noexcept
{
  size_t i = ID(type);
  if (m_Components[i].empty())
  {
    return {};
  }
  return m_Components[i].at(0);
}

optional<shared_ptr<Component>> GameObject::GetLastComponentByType(Component::Type type) noexcept
{
  return {};
}

optional<shared_ptr<Component>> GameObject::GetAnyComponentByType(Component::Type type) noexcept
{
  return {};
}

void GameObject::SetMeshFileName(const string& fileName) noexcept
{
  m_MeshFileName = fileName;
  m_MeshID = Error::INVALID_INDEX;
}