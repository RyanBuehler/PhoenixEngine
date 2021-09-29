#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "IMesh.h"

GameObject::GameObject() noexcept :
  m_Transform(make_unique<Transform>()),
  m_Mesh()
{
}

const Transform& GameObject::GetTransform() const noexcept
{
  assert(m_Transform.get());
  return *m_Transform.get();
}

const IMesh& GameObject::GetMesh() const noexcept
{
  assert(m_Mesh.get());
  return *m_Mesh.get();
}