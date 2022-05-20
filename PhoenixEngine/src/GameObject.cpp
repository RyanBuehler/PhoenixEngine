#include "pch.h"
#include "GameObject.h"
#include "MeshComponent.h"

using Utility::ID;
using CType = Component::Type;

GameObject::GameObject() noexcept :
  m_Transform(),
  m_bIsActive(true),
  m_bIsDirty(true)
{}

GameObject::GameObject(const GameObject& other) noexcept :
  m_Transform(other.m_Transform),
  m_bIsActive(other.m_bIsActive),
  m_bIsDirty(true),
  m_Components(other.m_Components)
{
}

GameObject& GameObject::operator=(const GameObject& other) noexcept
{
  m_Transform = other.m_Transform;
  m_bIsActive = other.m_bIsActive;
  m_bIsDirty = true;
  m_Components = other.m_Components;
  return *this;
}

GameObject::GameObject(GameObject&& other) noexcept :
  m_Transform(other.m_Transform),
  m_bIsActive(other.m_bIsActive),
  m_bIsDirty(other.m_bIsDirty),
  m_Components(std::move(other.m_Components))
{
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
  m_Transform = other.m_Transform;
  m_bIsActive = other.m_bIsActive;
  m_bIsDirty = other.m_bIsDirty;
  m_Components = std::move(other.m_Components);
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

void GameObject::SetMaterial(const Material& material) noexcept
{
  auto meshcomp = GetFirstComponentByType(Component::Type::MESH);
  if (!meshcomp.has_value())
  {
    Log::warn("[GameObject.cpp] Tried to set material with no Mesh Component.");
    return;
  }

  auto ptr = dynamic_pointer_cast<MeshComponent>(meshcomp.value());
  ptr->SetMaterial(material);
}

shared_ptr<Component> GameObject::AddComponent(Component::Type type) noexcept
{
  switch (type)
  {
    case CType::MESH:
      return m_Components[ID(CType::MESH)].emplace_back(make_shared<MeshComponent>(*this));
    case CType::COUNT:
    default:
      Log::error("[GameObject.cpp] Attempting to add component of invalid type.");
      break;
  }
  return nullptr;
}

shared_ptr<Component> GameObject::AddComponent(Component& component) noexcept
{
  switch (component.GetType())
  {
    case CType::MESH:
      return m_Components[ID(CType::MESH)].emplace_back(make_shared<MeshComponent>(*this));
    case CType::COUNT:
    default:
      Log::error("[GameObject.cpp] Attempting to add component of invalid type.");
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
  return GetFirstComponentByType(type);
}