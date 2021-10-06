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

vec3 Mesh::CalculateBoundingBoxSize() noexcept
{
  float xMin = numeric_limits<float>::max();
  float yMin = numeric_limits<float>::max();
  float zMin = numeric_limits<float>::max();
  float xMax = numeric_limits<float>::min();
  float yMax = numeric_limits<float>::min();
  float zMax = numeric_limits<float>::min();

  for(const vec3& v : m_PositionArray)
  {
    xMin = std::min(v.x, xMin);
    xMax = std::max(v.x, xMax);
    yMin = std::min(v.y, yMin);
    yMax = std::max(v.y, yMax);
    zMin = std::min(v.z, zMin);
    zMax = std::max(v.z, zMax);
  }

  return vec3(xMax - xMin, yMax - yMin, zMax - zMin);
}

float Mesh::CalculateWidestPoint() noexcept
{
  vec3 size = CalculateBoundingBoxSize();
  return std::max(std::max(size.x, size.y), size.z);
}

namespace
{
  struct NormalCloseEnough
  {
    bool operator() (const vec3& lhs, const vec3& rhs) const
    {
      float dotP = dot(lhs, rhs);
      return glm::epsilonNotEqual(dotP, 1.0f, FLT_EPSILON);
    }
  };
}

void Mesh::CalculateNormals(bool flipNormals) noexcept
{
  // vertices and indices must be populated
  if (m_PositionArray.empty() || m_TriangleArray.empty())
  {
    Log::Error("Mesh::CalculateNormals: Can't calculate from empty mesh.");
    return;
  }

  // Initialize vertex normals
  m_NormalArray.clear();
  m_NormalDisplay.clear();
  m_NormalArray.resize(GetVertexCount(), vec3(0.0f));
  m_NormalDisplay.resize(GetVertexCount() * 2u, vec3(0.0f));

  vector<set<vec3, NormalCloseEnough>> normalSet;
  normalSet.resize(GetVertexCount());

  // For every triangle
  for (size_t index = 0; index < GetTriangleCount(); ++index)
  {
    GLuint a = m_TriangleArray.at(index).Index1;
    GLuint b = m_TriangleArray.at(index).Index2;
    GLuint c = m_TriangleArray.at(index).Index2;

    vec3 vA = m_PositionArray[a];
    vec3 vB = m_PositionArray[b];
    vec3 vC = m_PositionArray[c];

    // Edge Vectors
    vec3 vEdge1 = vB - vA;
    vec3 vEdge2 = vC - vA;

    vec3 N = normalize(cross(vEdge1, vEdge2));

    if (flipNormals)
      N = N * -1.0f;

    normalSet.at(a).insert(N);
    normalSet.at(b).insert(N);
    normalSet.at(c).insert(N);
  }

  for (int index = 0; index < normalSet.size(); ++index)
  {
    vec3 vNormal(0.0f);

    auto nIt = normalSet[index].begin();
    while (nIt != normalSet[index].end())
    {
      vNormal += (*nIt);
      ++nIt;
    }

    // Final Vertex Normal
    m_NormalArray[index] = normalize(vNormal);


    // Calculate display normals
    vec3 point1 = m_PositionArray[index];

    m_NormalDisplay[2 * static_cast<size_t>(index)] = point1;
    m_NormalDisplay[(2 * static_cast<size_t>(index)) + 1] = point1 + (m_NormalLength * m_NormalArray[index]);
  }
}

void Mesh::ScaleToUnitSize() noexcept
{
  float factor = 1.f / CalculateWidestPoint();

  for (vec3& v : m_PositionArray)
  {
    v *= factor;
  }
}
