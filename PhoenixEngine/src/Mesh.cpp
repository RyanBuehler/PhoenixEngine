//------------------------------------------------------------------------------
// File:    Mesh.cpp
// Author:  Ryan Buehler
// Created: 09/24/21
// Desc:    The class to handle the mesh data for rendering
//------------------------------------------------------------------------------
#include "pch.h"
#include "Mesh.h"
#include "GLEW/glew.h"
#include <glm/gtc/epsilon.hpp>

Mesh::Mesh(const vec3& origin, bool isStatic) noexcept :
  m_Origin(origin),
  m_bIsStatic(isStatic),
  m_NormalLength(0.05f),
  m_PositionArray(),
  m_NormalArray(),
  m_TriangleArray(),
  m_bIsDirty(true)
{}

unsigned Mesh::GetVertexCount() const noexcept
{
  return static_cast<unsigned>(m_PositionArray.size());
}

unsigned Mesh::GetTriangleCount() const noexcept
{
  return static_cast<unsigned>(m_TriangleArray.size());
}

unsigned Mesh::GetNormalCount() const noexcept
{
  return static_cast<unsigned>(m_NormalArray.size());
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

struct compareVec
{
  bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const
  {
    float V = glm::dot(lhs, rhs);
    bool bRetCode = glm::epsilonNotEqual(V, 1.0f, FLT_EPSILON);

    return bRetCode;
  }
};

void Mesh::CalculateNormals(bool flipNormals) noexcept
{
  // vertices and indices must be populated
  if (m_PositionArray.empty() || m_TriangleArray.empty())
  {
    Log::Error("Mesh::CalculateNormals: Can't calculate from empty mesh.");
    return;
  }

  // Initialize vertex normals
  m_NormalArray.resize(GetVertexCount(), glm::vec3(0.0f));
  m_NormalDisplay.resize(GetVertexCount() * 2u, glm::vec3(0.0f));

  vector< std::set<glm::vec3, compareVec>> vNormalSet;
  vNormalSet.resize(GetVertexCount());

  // For every face
  for (size_t index = 0; index < GetTriangleCount(); ++index)
  {
    GLuint a = m_TriangleArray.at(index).Index1;
    GLuint b = m_TriangleArray.at(index).Index2;
    GLuint c = m_TriangleArray.at(index).Index2;

    vec3 vA = m_PositionArray[a];
    vec3 vB = m_PositionArray[b];
    vec3 vC = m_PositionArray[c];

    // Edge vectors
    vec3  E1 = vB - vA;
    vec3  E2 = vC - vA;

    vec3  N = glm::normalize(glm::cross(E1, E2));

    if (flipNormals)
      N = N * -1.0f;

    // For vertex a
    vNormalSet.at(a).insert(N);
    vNormalSet.at(b).insert(N);
    vNormalSet.at(c).insert(N);
  }

  // Now sum up the values per vertex
  for (int index = 0; index < vNormalSet.size(); ++index)
  {
    vec3 vNormal(0.0f);

    auto nIt = vNormalSet[index].begin();
    while (nIt != vNormalSet[index].end())
    {
      vNormal += (*nIt);
      ++nIt;
    }

    // save vertex normal
    m_NormalArray[index] = normalize(vNormal);

    // save normal to display
    vec3  vA = m_PositionArray[index];

    m_NormalDisplay[2 * static_cast<size_t>(index)] = vA;
    m_NormalDisplay[(2 * static_cast<size_t>(index)) + 1] = vA + (m_NormalLength * m_NormalArray[index]);
  }
}