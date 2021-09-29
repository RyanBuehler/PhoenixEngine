#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const vec3& origin, bool isStatic) noexcept :
  m_Origin(origin),
  m_bIsStatic(isStatic),
  m_PositionArray(),
  m_EdgeArray(),
  m_TriangleArray(),
  m_bIsDirty(true)
{
}

unsigned Mesh::GetVertexCount() const noexcept
{
  return static_cast<unsigned>(m_PositionArray.size());
}

unsigned Mesh::GetEdgeCount() const noexcept
{
  return static_cast<unsigned>(m_EdgeArray.size());
}

unsigned Mesh::GetTriangleCount() const noexcept
{
  return static_cast<unsigned>(m_TriangleArray.size());
}

void Mesh::AddVertex(vec3 vertex) noexcept
{
  m_PositionArray.push_back(vertex);
  m_bIsDirty = true;
}

void Mesh::AddVertex(float x, float y, float z) noexcept
{
  m_PositionArray.push_back({ x, y, z });
  m_bIsDirty = true;
}