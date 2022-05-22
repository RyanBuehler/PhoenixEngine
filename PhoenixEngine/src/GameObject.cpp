#include "pch.h"
#include "GameObject.h"
#include "MeshComponent.h"

using Utility::ID;

GameObject::GameObject() noexcept :
  m_bIsActive(true),
  m_bIsDirty(true)
{}

GameObject::GameObject(const GameObject& Other) noexcept :
  m_Transform(Other.m_Transform),
  m_bIsActive(Other.m_bIsActive),
  m_bIsDirty(true),
  m_Components(Other.m_Components)
{
}

GameObject& GameObject::operator=(const GameObject& Other) noexcept
{
  m_Transform = Other.m_Transform;
  m_bIsActive = Other.m_bIsActive;
  m_bIsDirty = true;
  m_Components = Other.m_Components;
  return *this;
}

GameObject::GameObject(GameObject&& Other) noexcept :
  m_Transform(Other.m_Transform),
  m_bIsActive(Other.m_bIsActive),
  m_bIsDirty(Other.m_bIsDirty),
  m_Components(std::move(Other.m_Components))
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

void GameObject::SetTransform(const Transform& Transform)
{
  m_Transform = Transform;
}

const Transform& GameObject::GetTransform() const noexcept
{
  return m_Transform;
}

void GameObject::SetMaterial(const Material& Mat) noexcept
{
  const auto meshComp = GetFirstComponentByType(Component::Type::MESH);
  if (!meshComp.has_value())
  {
    Log::Warn("[GameObject.cpp] Tried to set material with no Mesh Component.");
    return;
  }

  const auto ptr = dynamic_pointer_cast<MeshComponent>(meshComp.value());
  ptr->SetMaterial(Mat);
}

shared_ptr<Component> GameObject::AddComponent(const Component::Type ComponentType) noexcept
{
  switch (ComponentType)
  {
    case Component::Type::MESH:
      return m_Components[ID(Component::Type::MESH)].emplace_back(make_shared<MeshComponent>());
    case Component::Type::COUNT:
      Log::Error("[GameObject.cpp] Attempting to add component of invalid type.");
      break;
  }
  return nullptr;
}

shared_ptr<Component> GameObject::AddComponent(const Component& Component) noexcept
{
  switch (Component.GetType())
  {
    case Component::Type::MESH:
      return m_Components[ID(Component::Type::MESH)].emplace_back(make_shared<MeshComponent>());
    case Component::Type::COUNT:
      Log::Error("[GameObject.cpp] Attempting to add component of invalid type.");
      break;
  }
  return nullptr;
}

optional<shared_ptr<Component>> GameObject::GetFirstComponentByType(const Component::Type ComponentType) noexcept
{
  const size_t i = ID(ComponentType);
  if (m_Components[i].empty())
  {
    return {};
  }
  return m_Components[i].at(0);
}

optional<shared_ptr<Component>> GameObject::GetLastComponentByType(Component::Type ComponentType) noexcept
{
  return {};
}

optional<shared_ptr<Component>> GameObject::GetAnyComponentByType(const Component::Type ComponentType) noexcept
{
  return GetFirstComponentByType(ComponentType);
}