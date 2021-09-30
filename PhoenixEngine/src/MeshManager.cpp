//------------------------------------------------------------------------------
// File:    MeshManager.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Contains static primitives and functionality to load custom meshes.
//------------------------------------------------------------------------------
#include "pch.h"
#include "MeshManager.h"

MeshManager::MeshManager() noexcept :
  m_MeshArray(),
  m_OBJReader()
{
  //TODO: LULWUT
  m_OBJReader.initData();
}

MeshManager::~MeshManager()
{
  UnloadMeshes();
}

unsigned MeshManager::LoadMesh(const string& fileName) noexcept
{
  // Check if this mesh has already been loaded
  for (unsigned i = 0u; i < m_MeshArray.size(); ++i)
  {
    if (m_MeshDataArray[i].FileName == fileName)
    {
      return i;
    }
  }
  //TODO: check for file extension

  // Hasn't been loaded. Load from OBJ
  unsigned index = LoadMeshFromOBJ(fileName);
  if (index == MESH_INDEX_ERROR)
  {
    Log::Error("Could not load from OBJ file: " + fileName);
    return MESH_INDEX_ERROR;
  }

  // The Position buffer
  glGenBuffers(1, &m_MeshDataArray[index].PositionBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_MeshDataArray[index].PositionBufferID);
  glBufferData(GL_ARRAY_BUFFER, m_MeshArray[index].GetVertexCount() * sizeof(vec3),
    m_MeshArray[index].m_PositionArray.data(), GL_STATIC_DRAW);

  // The Face buffer
  glGenBuffers(1, &m_MeshDataArray[index].TriangleBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshDataArray[index].TriangleBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MeshArray[index].GetTriangleCount() * sizeof(Mesh::Triangle),
    m_MeshArray[index].m_TriangleArray.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &m_MeshDataArray[index].VertexArrayID);
  glBindVertexArray(m_MeshDataArray[index].VertexArrayID);

  // The '0' here is the attribute location
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0u);

  return index;
}

void MeshManager::UnloadMeshes() noexcept
{
  for (unsigned i = 0u; i < m_MeshDataArray.size(); ++i)
  {
    glDeleteVertexArrays(1, &m_MeshDataArray[i].VertexArrayID);

    glDeleteBuffers(1, &m_MeshDataArray[i].PositionBufferID);
    glDeleteBuffers(1, &m_MeshDataArray[i].TriangleBufferID);

    Log::Trace("Mesh '" + m_MeshArray[i].m_FileName + "' destroyed.");
  }
  m_MeshArray.clear();
  m_MeshDataArray.clear();
}

void MeshManager::RenderMesh(unsigned id) const noexcept
{
  if (id == MESH_INDEX_ERROR)
  {
    Log::Error("RenderMesh: Mesh not loaded!");
    return;
  }

  glBindVertexArray(m_MeshDataArray[id].VertexArrayID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshDataArray[id].TriangleBufferID);

  //TODO: GetElementCount? Instead to save the 3 * every frame?
  glDrawElements(GL_TRIANGLES, 3 * m_MeshArray[id].GetTriangleCount(), GL_UNSIGNED_INT, 0);
}

unsigned MeshManager::LoadMeshFromOBJ(const string& fileName) noexcept
{
  unsigned i = static_cast<unsigned>(m_MeshArray.size());
  m_MeshArray.emplace_back();
  m_MeshDataArray.emplace_back();
  m_MeshDataArray[i].FileName = fileName;

  auto result = m_OBJReader.ReadOBJFile(fileName, &m_MeshArray[i]);
  return i;
}