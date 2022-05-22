//------------------------------------------------------------------------------
// File:    MeshManager.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Contains static primitives and functionality to load custom meshes.
//------------------------------------------------------------------------------
#include "pch.h"
#include "MeshManager.h"
#include "Transform.h"
#include "glm/ext/scalar_constants.inl"
#include "DebugRenderer.h"
#include "AssetLoader.h"

#pragma region ImGui

#ifdef _IMGUI

#include "ImGUIManager.h"

#endif

#pragma endregion

MeshManager::~MeshManager()
{
  UnloadMeshes();
}

unsigned MeshManager::LoadMesh(
  const string& FileName,
  const bool ScaleToUnitSize,
  const bool ResetOrigin,
  const UV::Generation UvGeneration) noexcept
{
  // Check if this mesh has already been loaded
  for (unsigned i = 0u; i < m_MeshArray.size(); ++i)
  {
    if (m_MeshDataArray[i].FileName == FileName)
    {
      return i;
    }
  }

  ScaleToUnitSize ? Log::trace("Loading mesh: " + FileName) : Log::trace("Loading [Unit] mesh: " + FileName);

  unsigned index = numeric_limits<unsigned>::max();
  if (FileName == "sphere")
  {
    // Load procedural sphere
    index = LoadSphere();
  }
  else
  {
    // Hasn't been loaded. Load from OBJ
    index = LoadMeshFromFile(FileName);
    if (index == Error::INVALID_INDEX)
    {
      Log::error("Could not load from OBJ file: " + FileName);
      return Error::INVALID_INDEX;
    }
  }

  // Set up reference for legibility
  Mesh& mesh = m_MeshArray[index];

  // Scale to unit size (1x1x1 cube)
  if (ScaleToUnitSize)
  {
    mesh.ScaleToUnitSize();
  }

  // Reset the origin to the centroid
  if (ResetOrigin)
  {
    mesh.ResetOriginToCentroid();
  }

  // If the normals weren't calculated, calculate them now
  if (!mesh.NormalsAreCalculated())
    mesh.CalculateNormals();

  // Generate the UVs
  if (mesh.GetTexcoordCount() == 0)
  {
    mesh.GenerateTexCoords(UvGeneration);
  }

  // Assemble the Vertex Data for the GPU
  mesh.AssembleVertexData();

  // The Vertex buffer
  glGenBuffers(1, &m_MeshDataArray[index].PositionBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_MeshDataArray[index].PositionBufferID);
  glBufferData(GL_ARRAY_BUFFER, mesh.GetVertexCount() * sizeof(Mesh::VertexData),
    mesh.m_VertexData.data(), GL_STATIC_DRAW);

  // The Triangle buffer
  glGenBuffers(1, &m_MeshDataArray[index].TriangleBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshDataArray[index].TriangleBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetTriangleCount() * sizeof(Mesh::Triangle),
    mesh.m_TriangleArray.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &m_MeshDataArray[index].VertexArrayID);
  glBindVertexArray(m_MeshDataArray[index].VertexArrayID);

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  glEnableVertexAttribArray(0);
  // Normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), reinterpret_cast<void*>(sizeof(vec3)));
  glEnableVertexAttribArray(1);
  // Texcoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), reinterpret_cast<void*>(sizeof(vec3) * 2));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0u);

  Log::trace("Mesh: " + FileName + " loaded.");

  return index;
}

void MeshManager::UnloadMeshes() noexcept
{
  for (auto& i : m_MeshDataArray)
  {
    glDeleteVertexArrays(1, &i.VertexArrayID);

    glDeleteBuffers(1, &i.PositionBufferID);
    //glDeleteBuffers(1, &m_MeshDataArray[i].NormalBufferID);
    //glDeleteBuffers(1, &m_MeshDataArray[i].TexcoordBufferID);
    glDeleteBuffers(1, &i.TriangleBufferID);

    Log::trace("Mesh '" + i.FileName + "' destroyed.");
  }
  m_MeshArray.clear();
  m_MeshDataArray.clear();
}

void MeshManager::RenderMesh(const unsigned ID) const noexcept
{
  if (ID == Error::INVALID_INDEX)
  {
    Log::error("[RenderMesh] Mesh not loaded!");
    return;
  }

  glBindVertexArray(m_MeshDataArray[ID].VertexArrayID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshDataArray[ID].TriangleBufferID);

  glDrawElements(GL_TRIANGLES, 3 * static_cast<int>(m_MeshArray[ID].GetTriangleCount()), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0u);
}

void MeshManager::RenderSurfaceNormals(const unsigned ID, const float Length) const noexcept
{
  if (ID == Error::INVALID_INDEX)
  {
    Log::error("[RenderSurfaceNormals] Mesh not loaded!");
    return;
  }

  for (size_t i = 0; i < m_MeshArray[ID].m_SurfaceNormalArray.size(); ++i)
  {
    DebugRenderer::I().AddLine(
      m_MeshArray[ID].m_SurfaceNormalPositionArray[i],
      m_MeshArray[ID].m_SurfaceNormalPositionArray[i] + m_MeshArray[ID].m_SurfaceNormalArray[i] * Length);
  }

  DebugRenderer::I().RenderLines();
}

void MeshManager::RenderVertexNormals(const unsigned ID, const float Length) const noexcept
{
  if (ID == Error::INVALID_INDEX)
  {
    Log::error("[RenderVertexNormals] Mesh not loaded!");
    return;
  }

  for (size_t i = 0; i < m_MeshArray[ID].m_VertexNormalArray.size(); ++i)
  {
    DebugRenderer::I().AddLine(
      m_MeshArray[ID].m_PositionArray[i],
      m_MeshArray[ID].m_PositionArray[i] + m_MeshArray[ID].m_VertexNormalArray[i] * Length);
  }

  DebugRenderer::I().RenderLines();
}

unsigned MeshManager::LoadMeshFromFile(const string& FileName) noexcept
{
  const auto i = static_cast<unsigned>(m_MeshArray.size());
  m_MeshArray.emplace_back();
  m_MeshDataArray.emplace_back();
  m_MeshDataArray[i].FileName = FileName;

  AssetLoader::LoadModel(FileName, m_MeshArray[i]);
  //m_ObjReader.ReadOBJFile(FileName, &m_MeshArray[i], OBJReader::ReadMethod::LINE_BY_LINE, false);
  return i;
}

const Mesh& MeshManager::GetMeshByID(const unsigned ID) const noexcept
{
  return m_MeshArray[ID];
}

unsigned MeshManager::LoadSphere(const float Radius, const unsigned NumDivisions) noexcept
{
  const auto index = static_cast<unsigned>(m_MeshArray.size());
  m_MeshArray.emplace_back();
  m_MeshDataArray.emplace_back();
  m_MeshDataArray[index].FileName = "Procedural Sphere";

  const unsigned slices = NumDivisions;
  const unsigned stacks = 2 * NumDivisions;
  const unsigned topVertex = (stacks * (slices - 1));
  const unsigned bottomVertex = (stacks * (slices - 1) + 1);

  // Reference for legibility
  Mesh& mesh = m_MeshArray[index];

  mesh.m_PositionArray.resize(stacks * (slices - 1u) + 2u);
  mesh.m_VertexNormalArray.resize(stacks * (slices - 1u) + 2u);

  for (unsigned i = 1; i < slices; ++i)
  {
    const float theta = glm::pi<float>() * i / slices;
    for (unsigned j = 0; j < stacks; ++j)
    {
      const unsigned y = stacks * (i - 1) + j;
      const float phi = 2 * glm::pi<float>() * j / stacks;
      const vec3 normal(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
      mesh.m_VertexNormalArray[y] = normal;
    }
  }

  mesh.m_VertexNormalArray[topVertex] = vec3(0.0f, 0.0f, 1.0f);
  mesh.m_VertexNormalArray[bottomVertex] = vec3(0.0f, 0.0f, -1.0f);

  for (unsigned n = 0; n < mesh.m_VertexNormalArray.size(); ++n)
  {
    mesh.m_PositionArray[n] = Radius * mesh.m_VertexNormalArray[n];
  }

  for (unsigned i = 2; i < slices; ++i)
  {
    for (unsigned j = 0; j < stacks; ++j)
    {
      Mesh::Triangle triangle;
      const unsigned nextIndex = (j + 1) % stacks;
      triangle.Index1 = stacks * (i - 2) + j;
      triangle.Index2 = stacks * (i - 1) + nextIndex;
      triangle.Index3 = stacks * (i - 2) + nextIndex;
      mesh.m_TriangleArray.push_back(triangle);
      triangle.Index2 = stacks * (i - 1) + j;
      triangle.Index3 = stacks * (i - 1) + nextIndex;
      mesh.m_TriangleArray.push_back(triangle);
    }
  }

  for (unsigned j = 0; j < stacks; ++j)
  {
    Mesh::Triangle triangle;
    const unsigned nextIndex = (j + 1) % stacks;
    triangle.Index1 = j;
    triangle.Index2 = nextIndex;
    triangle.Index3 = topVertex;
    mesh.m_TriangleArray.push_back(triangle);
    triangle.Index1 = stacks * (slices - 2) + j;
    triangle.Index2 = bottomVertex;
    triangle.Index3 = stacks * (slices - 2) + nextIndex;
    mesh.m_TriangleArray.push_back(triangle);
  }

  return index;
}