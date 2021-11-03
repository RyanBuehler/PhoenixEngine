#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const string& meshFileName) noexcept :
  m_Transform(),
  m_MeshID(numeric_limits<unsigned>::max()),
  m_MeshFileName(meshFileName),
  m_bIsActive(true),
  m_bIsDirty(true)
{}

GameObject::GameObject(const GameObject& other) noexcept :
  m_Transform(other.m_Transform),
  m_MeshID(other.m_MeshID),
  m_MeshFileName(other.m_MeshFileName),
  m_bIsActive(other.m_bIsActive),
  m_bIsDirty(other.m_bIsDirty)
{
}

GameObject& GameObject::operator=(const GameObject& other) noexcept
{
  m_Transform = other.m_Transform;
  m_MeshID = other.m_MeshID;
  m_MeshFileName = other.m_MeshFileName;
  m_bIsActive = other.m_bIsActive;
  m_bIsDirty = other.m_bIsDirty;
  return *this;
}

GameObject::GameObject(GameObject&& other) noexcept :
  m_Transform(other.m_Transform),
  m_MeshID(other.m_MeshID),
  m_MeshFileName(other.m_MeshFileName),
  m_bIsActive(other.m_bIsActive),
  m_bIsDirty(other.m_bIsDirty)
{
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
  m_Transform = other.m_Transform;
  m_MeshID = other.m_MeshID;
  m_MeshFileName = other.m_MeshFileName;
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

void GameObject::SetMeshFileName(const string& fileName) noexcept
{
  m_MeshFileName = fileName;
  m_MeshID = numeric_limits<unsigned>::max();
}
