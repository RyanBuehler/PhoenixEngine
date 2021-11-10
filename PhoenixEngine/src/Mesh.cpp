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
#include <glm/gtc/constants.hpp>
//TODO:
#include "DebugRenderer.h"
#include "Colors.h"

Mesh::Mesh(const vec3& origin, bool isStatic) noexcept :
  m_Origin(origin),
  m_bIsStatic(isStatic),
  m_NormalLength(0.05f),
  m_PositionArray(),
  m_VertexNormalArray(),
  m_SurfaceNormalArray(),
  m_SurfaceNormalPositionArray(),
  m_TriangleArray(),
  m_TexcoordArray(),
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
  return static_cast<unsigned>(m_VertexNormalArray.size());
}

unsigned Mesh::GetTexcoordCount() const noexcept
{
  return static_cast<unsigned>(m_TexcoordArray.size());
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
  m_VertexNormalArray.push_back(normal);
  m_bIsDirty = true;
}

void Mesh::AddNormal(float x, float y, float z) noexcept
{
  m_VertexNormalArray.push_back({ x, y, z });
  m_bIsDirty = true;
}

void Mesh::AddTriangle(unsigned index1, unsigned index2, unsigned index3) noexcept
{
  m_TriangleArray.push_back({ index1, index2, index3 });
}

const vector<vec3>& Mesh::GetVertexNormalArray() const noexcept
{
  return m_VertexNormalArray;
}

const vector<vec3>& Mesh::GetSurfaceNormalArray() const noexcept
{
  return m_SurfaceNormalArray;
}

const vector<vec3>& Mesh::GetSurfaceNormalPositionArray() const noexcept
{
  return m_SurfaceNormalPositionArray;
}

const vector<vec2>& Mesh::GetTexcoordArray() const noexcept
{
  return m_TexcoordArray;
}

vec3 Mesh::CalculateBoundingBoxSize() noexcept
{
  float xMin = numeric_limits<float>::max();
  float yMin = numeric_limits<float>::max();
  float zMin = numeric_limits<float>::max();
  float xMax = numeric_limits<float>::min();
  float yMax = numeric_limits<float>::min();
  float zMax = numeric_limits<float>::min();

  for (const vec3& v : m_PositionArray)
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

  calculateSurfaceNormals(flipNormals);
  calculateVertexNormals();
}

void Mesh::ScaleToUnitSize() noexcept
{
  float factor = 1.f / CalculateWidestPoint();

  for (vec3& v : m_PositionArray)
  {
    v *= factor;
  }

  for (vec3& np : m_SurfaceNormalPositionArray)
  {
    np *= factor;
  }

}

vec3 Mesh::FindCenterOfMass() const noexcept
{
  float xMin = numeric_limits<float>::max();
  float yMin = numeric_limits<float>::max();
  float zMin = numeric_limits<float>::max();
  float xMax = numeric_limits<float>::min();
  float yMax = numeric_limits<float>::min();
  float zMax = numeric_limits<float>::min();

  for (const vec3& v : m_PositionArray)
  {
    xMin = std::min(v.x, xMin);
    xMax = std::max(v.x, xMax);
    yMin = std::min(v.y, yMin);
    yMax = std::max(v.y, yMax);
    zMin = std::min(v.z, zMin);
    zMax = std::max(v.z, zMax);
  }

  vec3 center(
    xMin + ((xMax - xMin) / 2.f),
    yMin + ((yMax - yMin) / 2.f),
    zMin + ((zMax - zMin) / 2.f)
  );

  return center;
}

void Mesh::ResetOriginToCenterOfMass() noexcept
{
  vec3 oldOrigin = m_Origin;
  m_Origin = FindCenterOfMass();

  vec3 move = m_Origin - oldOrigin;

  for (vec3& v : m_PositionArray)
  {
    v -= move;
  }

  for (vec3& sn : m_SurfaceNormalPositionArray)
  {
    sn -= move;
  }
}

void Mesh::GenerateTexcoords(UV::Generation generation) noexcept
{
  switch (generation)
  {
  case UV::Generation::SPHERICAL:
    calculateSphereUVs();
    break;
  case UV::Generation::CYLINDRICAL:
    calculateCylinderUVs();
    break;
  case UV::Generation::PLANAR:
    break;
  case UV::Generation::CUSTOM:
  default:
    break;
  }
}

//TODO: Fix this
void Mesh::AssembleVertexData() noexcept
{
  for (size_t i = 0; i < m_PositionArray.size(); ++i)
  {
    m_VertexData.emplace_back(m_PositionArray[i], m_VertexNormalArray[i], m_TexcoordArray[i]);
  }
}

void Mesh::calculateSurfaceNormals(bool flipNormals) noexcept
{
  m_SurfaceNormalArray.clear();
  m_SurfaceNormalPositionArray.clear();
  m_SurfaceNormalArray.resize(GetTriangleCount(), vec3(0.0f));
  m_SurfaceNormalPositionArray.resize(GetTriangleCount(), vec3(0.0f));

  for (size_t i = 0; i < m_TriangleArray.size(); ++i)
  {
    Mesh::Triangle& tri = m_TriangleArray[i];
    vec3 v1 = m_PositionArray[tri.Index1];
    vec3 v2 = m_PositionArray[tri.Index2];
    vec3 v3 = m_PositionArray[tri.Index3];

    vec3 e1 = v2 - v1;
    vec3 e2 = v3 - v1;

    vec3 n = normalize(cross(e1, e2));

    m_SurfaceNormalArray[i] = flipNormals ? -1.f * n : n;

    m_SurfaceNormalPositionArray[i] = 1.f / 3.f * (v1 + v2 + v3);
  }
}

void Mesh::calculateVertexNormals() noexcept
{
  m_VertexNormalArray.clear();
  m_VertexNormalArray.resize(GetVertexCount(), vec3(0.0f));

  vector<pair<vec3, float>> normArray(m_VertexNormalArray.size(), make_pair(vec3(0.f), 0.f));
  for (size_t i = 0; i < m_SurfaceNormalArray.size(); ++i)
  {
    Mesh::Triangle& tri = m_TriangleArray[i];
    vec3 sN = m_SurfaceNormalArray[i];

    // first - Surface Normal
    normArray[tri.Index1].first += sN;
    normArray[tri.Index2].first += sN;
    normArray[tri.Index3].first += sN;
    // second - triangles shared
    ++normArray[tri.Index1].second;
    ++normArray[tri.Index2].second;
    ++normArray[tri.Index3].second;
  }

  for (size_t i = 0; i < normArray.size(); ++i)
  {
    if (normArray[i].second > 0.f)
    {
      m_VertexNormalArray[i] = 1.f / normArray[i].second * normArray[i].first;
    }
  }
}

void Mesh::calculateSphereUVs() noexcept
{
  m_TexcoordArray.clear();
  vec3 bounds = CalculateBoundingBoxSize();
  vec3 centroid = FindCenterOfMass();
  for (const vec3& vertex : m_PositionArray)
  {
    vec3 pos = vertex - centroid;
    float theta = glm::atan(pos.z / pos.x);

    if (pos.z < 0)
    {
      if (pos.x < 0)
      {
        theta += glm::pi<float>();
      }
      else
      {
        theta += glm::pi<float>() * 3.f / 2.f;
      }
    }
    else
    {
      if (pos.x < 0)
      {
        theta += glm::pi<float>() / 2.f;
      }
    }

    float phi = acos(pos.y / glm::length(pos));

    float u = theta / glm::two_pi<float>();
    float v = (glm::pi<float>() - phi) / glm::pi<float>();

    m_TexcoordArray.emplace_back(u, v);
  }
}

void Mesh::calculateCylinderUVs() noexcept
{
  m_TexcoordArray.clear();
  vec3 bounds = CalculateBoundingBoxSize();
  vec3 centroid = FindCenterOfMass();
  for (const vec3& vertex : m_PositionArray)
  {
    vec3 pos = vertex - centroid;
    float theta = glm::atan(pos.z / pos.x);

    if (pos.z < 0)
    {
      if (pos.x < 0)
      {
        theta += glm::pi<float>();
      }
      else
      {
        theta += glm::pi<float>() * 3.f / 2.f;
      }
    }
    else
    {
      if (pos.x < 0)
      {
        theta += glm::pi<float>() / 2.f;
      }
    }

    float u = theta / glm::two_pi<float>();
    float v = (pos.y - bounds.y / -2.f) / bounds.y;

    m_TexcoordArray.emplace_back(u, v);
  }
}

void Mesh::calculateCubeMapUVs() noexcept
{
  m_TexcoordArray.clear();
  
}
