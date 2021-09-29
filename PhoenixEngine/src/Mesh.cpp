#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const vec3& origin, bool isStatic) noexcept :
  m_Origin(origin),
  m_bIsStatic(isStatic),
  m_PositionArray(),
  m_NormalArray(),
  //m_EdgeArray(),
  m_TriangleArray(),
  m_bIsDirty(true)
{
  Log::Trace("Mesh constructor called");
}

unsigned Mesh::GetVertexCount() const noexcept
{
  return static_cast<unsigned>(m_PositionArray.size());
}

//unsigned Mesh::GetEdgeCount() const noexcept
//{
//  return static_cast<unsigned>(m_EdgeArray.size());
//}

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

void Mesh::AddNormal(vec3 normal) noexcept
{
  m_NormalArray.push_back(normal);
  m_bIsDirty = true;
}

void Mesh::AddNormal(float x, float y, float z) noexcept
{
  m_NormalArray.push_back({ x, y, z });
  m_bIsDirty = true;
}

void Mesh::AddTriangle(unsigned index1, unsigned index2, unsigned index3) noexcept
{
  m_TriangleArray.push_back({ index1, index2, index3 });
}
