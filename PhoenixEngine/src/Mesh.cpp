//------------------------------------------------------------------------------
// File:    Mesh.cpp
// Author:  Ryan Buehler
// Created: 09/24/21
// Desc:    The class to handle the mesh data for rendering
//------------------------------------------------------------------------------
#include "pch.h"
#include "Mesh.h"
#include <glm/gtc/epsilon.hpp>    // For epsilon use in checking normal congruency
#include <glm/gtc/constants.hpp>  // PI and related constants

Mesh::Mesh(const vec3& Origin, const bool bIsStatic) noexcept :
  m_Origin(Origin),
  m_MeshIsStatic(bIsStatic),
  m_NormalLength(0.05f),
  m_MeshIsDirty(true),
  m_NormalsAreCalculated(false)
{}

unsigned Mesh::GetVertexCount() const noexcept
{
  return static_cast<unsigned>(m_PositionArray.size());
}

unsigned Mesh::GetTriangleCount() const noexcept
{
  return static_cast<unsigned>(m_TriangleArray.size());
}

unsigned Mesh::GetVertexNormalCount() const noexcept
{
  return static_cast<unsigned>(m_VertexNormalArray.size());
}

unsigned Mesh::GetTexcoordCount() const noexcept
{
  return static_cast<unsigned>(m_TexcoordArray.size());
}

void Mesh::AddVertex(const vec3& Vertex) noexcept
{
  m_PositionArray.push_back(Vertex);
  m_MeshIsDirty = true;
}

void Mesh::AddVertex(float X, float Y, float Z) noexcept
{
  m_PositionArray.emplace_back(X, Y, Z);
  m_MeshIsDirty = true;
}

void Mesh::AddVertexNormal(const vec3& Normal) noexcept
{
  m_VertexNormalArray.push_back(Normal);
  m_MeshIsDirty = true;
}

void Mesh::AddVertexNormal(float X, float Y, float Z) noexcept
{
  m_VertexNormalArray.emplace_back(X, Y, Z);
  m_MeshIsDirty = true;
}

void Mesh::AddTriangle(unsigned Index1, unsigned Index2, unsigned Index3) noexcept
{
  m_TriangleArray.emplace_back(Index1, Index2, Index3);
  m_MeshIsDirty = true;
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

Mesh::BoundingBox Mesh::CalculateBoundingBox() const noexcept
{
  BoundingBox bounds{
    bounds.XMin = numeric_limits<float>::max(),
    bounds.YMin = numeric_limits<float>::max(),
    bounds.ZMin = numeric_limits<float>::max(),
    bounds.XMax = numeric_limits<float>::min(),
    bounds.YMax = numeric_limits<float>::min(),
    bounds.ZMax = numeric_limits<float>::min()
  };


  // Find the minimum x, y, z values
  for (const vec3& v : m_PositionArray)

  {
    bounds.XMin = std::min(v.x, bounds.XMin);
    bounds.XMax = std::max(v.x, bounds.XMax);
    bounds.YMin = std::min(v.y, bounds.YMin);
    bounds.YMax = std::max(v.y, bounds.YMax);
    bounds.ZMin = std::min(v.z, bounds.ZMin);
    bounds.ZMax = std::max(v.z, bounds.ZMax);
  }

  return bounds;
}

vec3 Mesh::CalculateBoundingBoxSize() const noexcept
{
  const BoundingBox bounds = CalculateBoundingBox();

  return {bounds.XMax - bounds.XMin, bounds.YMax - bounds.YMin, bounds.ZMax - bounds.ZMin};
}

float Mesh::CalculateWidestPoint() const noexcept
{
  // First get the bounding box size
  const vec3 size = CalculateBoundingBoxSize();

  // Return the widest dimension
  return std::max(std::max(size.x, size.y), size.z);
}

// Local helper function for use in merging triangle normals to flat surface
//namespace
//{
//  // Compare normals by epsilon
//  struct NormalCloseEnough
//  {
//    bool operator() (const vec3& lhs, const vec3& rhs) const
//    {
//      float dotP = dot(lhs, rhs);
//      return glm::epsilonNotEqual(dotP, 1.0f, FLT_EPSILON);
//    }
//  };
//}

void Mesh::CalculateNormals(bool bShouldFlipNormals) noexcept
{
  // Vertices and indices must be populated
  if (m_PositionArray.empty() || m_TriangleArray.empty())
  {
    Log::error("Mesh::CalculateNormals - Can't calculate from empty mesh.");
    return;
  }

  // First get the surface normals
  calculateSurfaceNormals(bShouldFlipNormals);
  // Using surface normals, average them out per vertex
  calculateVertexNormals();
}

void Mesh::ScaleToUnitSize() noexcept
{
  // First find the ratio of v to the widest axis
  const float factor = 1.f / CalculateWidestPoint();

  // For each location, rescale
  for (vec3& v : m_PositionArray)
  {
    v *= factor;
  }

  // For each surface normal location, rescale
  for (vec3& np : m_SurfaceNormalPositionArray)
  {
    np *= factor;
  }
}

vec3 Mesh::FindCentroid() const noexcept
{
  const BoundingBox bounds = CalculateBoundingBox();

  // Average the center of 
  const vec3 center(
    bounds.XMin + ((bounds.XMax - bounds.XMin) / 2.f),
    bounds.YMin + ((bounds.YMax - bounds.YMin) / 2.f),
    bounds.ZMin + ((bounds.ZMax - bounds.ZMin) / 2.f)
  );

  return center;
}

void Mesh::ResetOriginToCentroid() noexcept
{
  const vec3 oldOrigin = m_Origin;
  m_Origin = FindCentroid();

  const vec3 move = m_Origin - oldOrigin;

  for (vec3& v : m_PositionArray)
  {
    v -= move;
  }

  for (vec3& sn : m_SurfaceNormalPositionArray)
  {
    sn -= move;
  }
  m_MeshIsDirty = true;
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
    calculatePlanarUVs();
    break;
  case UV::Generation::CUSTOM:
    break;
  }
  m_MeshIsDirty = true;
}

void Mesh::AssembleVertexData() noexcept
{
  for (size_t i = 0; i < m_PositionArray.size(); ++i)
  {
    m_VertexData.emplace_back(m_PositionArray[i], m_VertexNormalArray[i], m_TexcoordArray[i]);
  }
  m_MeshIsDirty = true;
}

void Mesh::calculateSurfaceNormals(bool bShouldFlipNormals) noexcept
{
  m_SurfaceNormalArray.clear();
  m_SurfaceNormalPositionArray.clear();
  m_SurfaceNormalArray.resize(GetTriangleCount(), vec3(0.0f));
  m_SurfaceNormalPositionArray.resize(GetTriangleCount(), vec3(0.0f));

  for (size_t i = 0; i < m_TriangleArray.size(); ++i)
  {
    const Triangle& tri = m_TriangleArray[i];
    vec3 v1 = m_PositionArray[tri.Index1];
    vec3 v2 = m_PositionArray[tri.Index2];
    vec3 v3 = m_PositionArray[tri.Index3];

    vec3 e1 = v2 - v1;
    vec3 e2 = v3 - v1;

    vec3 n = normalize(cross(e1, e2));

    m_SurfaceNormalArray[i] = bShouldFlipNormals ? -1.f * n : n;

    m_SurfaceNormalPositionArray[i] = 1.f / 3.f * (v1 + v2 + v3);
  }
  m_MeshIsDirty = true;
}

void Mesh::calculateVertexNormals() noexcept
{
  m_VertexNormalArray.clear();
  m_VertexNormalArray.resize(GetVertexCount(), vec3(0.0f));

  vector normArray(m_VertexNormalArray.size(), make_pair(vec3(0.f), 0.f));
  for (size_t i = 0; i < m_SurfaceNormalArray.size(); ++i)
  {
    const Triangle& tri = m_TriangleArray[i];
    const vec3 sN = m_SurfaceNormalArray[i];

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
  m_MeshIsDirty = true;
}

void Mesh::calculatePlanarUVs() noexcept
{
  m_TexcoordArray.clear();

  // First get the bounding box size
  const BoundingBox bounds = CalculateBoundingBox();

  for (const vec3& vertex : m_PositionArray)
  {
    float u = (vertex.x - bounds.XMin) / (bounds.XMax - bounds.XMin);
    float v = (vertex.y - bounds.YMin) / (bounds.YMax - bounds.YMin);
    m_TexcoordArray.emplace_back(u, v);
  }

  m_MeshIsDirty = true;
}

void Mesh::calculateSphereUVs() noexcept
{
  m_TexcoordArray.clear();
  
  // Find the centroid
  const vec3 centroid{ FindCentroid() };

  // For each vertex
  for (const vec3& vertex : m_PositionArray)
  {
    // Adjust vertex position by centroid
    vec3 pos = vertex - centroid;
    // Get first angle
    float theta = glm::atan(pos.z / pos.x);

    // Adjust angle by quadrant
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

    // Get second angle
    const float phi = acos(pos.y / glm::length(pos));

    // Set the UVs
    float u = theta / glm::two_pi<float>();
    float v = (glm::pi<float>() - phi) / glm::pi<float>();

    m_TexcoordArray.emplace_back(u, v);
  }
  m_MeshIsDirty = true;
}

void Mesh::calculateCylinderUVs() noexcept
{
  m_TexcoordArray.clear();
  // First get the bounding box size
  const vec3 bounds{ CalculateBoundingBoxSize() };
  // Find the centroid
  const vec3 centroid{ FindCentroid() };

  // For each vertex
  for (const vec3& vertex : m_PositionArray)
  {
    // Adjust vertex position by centroid
    const vec3 pos = vertex - centroid;
    // Get the radial angle
    float theta = glm::atan(pos.z / pos.x);

    // Adjust angle by quadrant
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

    // U is based on radial angle
    float u = theta / glm::two_pi<float>();
    // V is based on cylinder height
    float v = (pos.y - bounds.y / -2.f) / bounds.y;

    m_TexcoordArray.emplace_back(u, v);
  }
  m_MeshIsDirty = true;
}

void Mesh::calculateCubeMapUVs() const noexcept
{
  Log::error("Mesh::calculateCubeMapUVs not implemented.");
}
