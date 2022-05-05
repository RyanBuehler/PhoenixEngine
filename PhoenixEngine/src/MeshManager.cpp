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

#pragma region ImGui

#ifdef _IMGUI

#include "ImGUIManager.h"

#endif

#pragma endregion

MeshManager::MeshManager() noexcept :
  m_MeshArray(),
  m_OBJReader()
{
  //TODO: This can probably be circumvented
  // Probably remove this after AssImp
  m_OBJReader.initData();
}

MeshManager::~MeshManager()
{
  UnloadMeshes();
}

unsigned MeshManager::LoadMesh(
  const string& fileName,
  bool scaleToUnitSize,
  bool resetOrigin,
  UV::Generation uvGeneration) noexcept
{
  // Check if this mesh has already been loaded
  for (unsigned i = 0u; i < m_MeshArray.size(); ++i)
  {
    if (m_MeshDataArray[i].FileName == fileName)
    {
      return i;
    }
  }
  scaleToUnitSize ? Log::Trace("Loading mesh: " + fileName) : Log::Trace("Loading [Unit] mesh: " + fileName);

  unsigned index = numeric_limits<unsigned>::max();
  if (fileName == "sphere")
  {
    // Load procedural sphere
    index = LoadSphere();
  }
  else
  {
    // Hasn't been loaded. Load from OBJ
    index = LoadMeshFromOBJ(fileName);
    if (index == Error::INVALID_INDEX)
    {
      Log::Error("Could not load from OBJ file: " + fileName);
      return Error::INVALID_INDEX;
    }
  }

  // Set up reference for legibility
  Mesh& mesh = m_MeshArray[index];

  // Scale to unit size (1x1x1 cube)
  if (scaleToUnitSize)
  {
    mesh.ScaleToUnitSize();
  }

  // Reset the origin to the centroid
  if (resetOrigin)
  {
    mesh.ResetOriginToCentroid();
  }

  // If the normals weren't calculated, calculate them now
  if (!mesh.NormalsAreCalculated())
    mesh.CalculateNormals();

  // Generate the UVs
  mesh.GenerateTexcoords(uvGeneration);

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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void*)(sizeof(vec3)));
  glEnableVertexAttribArray(1);
  // Texcoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void*)(sizeof(vec3) * 2));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0u);

  Log::Trace("Mesh: " + fileName + " loaded.");

  return index;
}

void MeshManager::UnloadMeshes() noexcept
{
  for (unsigned i = 0u; i < m_MeshDataArray.size(); ++i)
  {
    glDeleteVertexArrays(1, &m_MeshDataArray[i].VertexArrayID);

    glDeleteBuffers(1, &m_MeshDataArray[i].PositionBufferID);
    //glDeleteBuffers(1, &m_MeshDataArray[i].NormalBufferID);
    //glDeleteBuffers(1, &m_MeshDataArray[i].TexcoordBufferID);
    glDeleteBuffers(1, &m_MeshDataArray[i].TriangleBufferID);

    Log::Trace("Mesh '" + m_MeshDataArray[i].FileName + "' destroyed.");
  }
  m_MeshArray.clear();
  m_MeshDataArray.clear();
}

void MeshManager::RenderMesh(unsigned id) const noexcept
{
  if (id == Error::INVALID_INDEX)
  {
    Log::Error("[RenderMesh] Mesh not loaded!");
    return;
  }

  glBindVertexArray(m_MeshDataArray[id].VertexArrayID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshDataArray[id].TriangleBufferID);

  glDrawElements(GL_TRIANGLES, 3 * m_MeshArray[id].GetTriangleCount(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0u);
}

void MeshManager::RenderSurfaceNormals(unsigned id, float length) const noexcept
{
  if (id == Error::INVALID_INDEX)
  {
    Log::Error("[RenderSurfaceNormals] Mesh not loaded!");
    return;
  }

  for (size_t i = 0; i < m_MeshArray[id].m_SurfaceNormalArray.size(); ++i)
  {
    DebugRenderer::I().AddLine(
      m_MeshArray[id].m_SurfaceNormalPositionArray[i],
      m_MeshArray[id].m_SurfaceNormalPositionArray[i] + m_MeshArray[id].m_SurfaceNormalArray[i] * length);
  }

  DebugRenderer::I().RenderLines();
}

void MeshManager::RenderVertexNormals(unsigned id, float length) const noexcept
{
  if (id == Error::INVALID_INDEX)
  {
    Log::Error("[RenderVertexNormals] Mesh not loaded!");
    return;
  }

  for (size_t i = 0; i < m_MeshArray[id].m_VertexNormalArray.size(); ++i)
  {
    DebugRenderer::I().AddLine(
      m_MeshArray[id].m_PositionArray[i],
      m_MeshArray[id].m_PositionArray[i] + m_MeshArray[id].m_VertexNormalArray[i] * length);
  }

  DebugRenderer::I().RenderLines();
}

unsigned MeshManager::LoadMeshFromOBJ(const string& fileName) noexcept
{
  unsigned i = static_cast<unsigned>(m_MeshArray.size());
  m_MeshArray.emplace_back();
  m_MeshDataArray.emplace_back();
  m_MeshDataArray[i].FileName = fileName;

  auto result = m_OBJReader.ReadOBJFile(fileName, &m_MeshArray[i], OBJReader::ReadMethod::LINE_BY_LINE, false);
  return i;
}

const Mesh& MeshManager::GetMeshByID(unsigned id) const noexcept
{
  return m_MeshArray[id];
}

unsigned MeshManager::LoadSphere(float radius, int numDivisions) noexcept
{
  unsigned index = static_cast<unsigned>(m_MeshArray.size());
  m_MeshArray.emplace_back();
  m_MeshDataArray.emplace_back();
  m_MeshDataArray[index].FileName = "Procedural Sphere";

  const unsigned SLICES = numDivisions;
  const unsigned STACKS = 2 * numDivisions;
  const unsigned TOPVERT = (STACKS * (SLICES - 1));
  const unsigned BOTVERT = (STACKS * (SLICES - 1) + 1);

  // Reference for legibility
  Mesh& mesh = m_MeshArray[index];

  mesh.m_PositionArray.resize(static_cast<size_t>(STACKS * (SLICES - 1) + 2));
  mesh.m_VertexNormalArray.resize(static_cast<size_t>(STACKS * (SLICES - 1) + 2));

  for (unsigned i = 1; i < SLICES; ++i)
  {
    float theta = glm::pi<float>() * i / SLICES;
    for (unsigned j = 0; j < STACKS; ++j)
    {
      unsigned y = STACKS * (i - 1) + j;
      float phi = 2 * glm::pi<float>() * j / STACKS;
      vec3 normal(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
      mesh.m_VertexNormalArray[y] = normal;
    }
  }

  mesh.m_VertexNormalArray[TOPVERT] = vec3(0.0f, 0.0f, 1.0f);
  mesh.m_VertexNormalArray[BOTVERT] = vec3(0.0f, 0.0f, -1.0f);

  for (unsigned n = 0; n < mesh.m_VertexNormalArray.size(); ++n)
  {
    mesh.m_PositionArray[n] = radius * mesh.m_VertexNormalArray[n];
  }

  for (unsigned i = 2; i < SLICES; ++i)
  {
    for (unsigned j = 0; j < STACKS; ++j)
    {
      Mesh::Triangle triangle;
      unsigned nextIndex = (j + 1) % STACKS;
      triangle.Index1 = STACKS * (i - 2) + j;
      triangle.Index2 = STACKS * (i - 1) + nextIndex;
      triangle.Index3 = STACKS * (i - 2) + nextIndex;
      mesh.m_TriangleArray.push_back(triangle);
      triangle.Index2 = STACKS * (i - 1) + j;
      triangle.Index3 = STACKS * (i - 1) + nextIndex;
      mesh.m_TriangleArray.push_back(triangle);
    }
  }

  for (unsigned j = 0; j < STACKS; ++j)
  {
    Mesh::Triangle triangle;
    unsigned nextIndex = (j + 1) % STACKS;
    triangle.Index1 = j;
    triangle.Index2 = nextIndex;
    triangle.Index3 = TOPVERT;
    mesh.m_TriangleArray.push_back(triangle);
    triangle.Index1 = STACKS * (SLICES - 2) + j;
    triangle.Index2 = BOTVERT;
    triangle.Index3 = STACKS * (SLICES - 2) + nextIndex;
    mesh.m_TriangleArray.push_back(triangle);
  }

  return index;
}