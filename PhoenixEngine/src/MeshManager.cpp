//------------------------------------------------------------------------------
// File:    MeshManager.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Contains static primitives and functionality to load custom meshes.
//------------------------------------------------------------------------------
#include "pch.h"
#include "GLEW/glew.h"
#include "MeshManager.h"
#include "TriangleMesh.h"
#include "CubeMesh.h"

MeshManager::MeshManager() noexcept :
  m_MeshArray(),
  m_PrimitiveIDs({
  INDEX_ERROR,
  INDEX_ERROR
    })
{}

unsigned MeshManager::LoadPrimitive(Mesh::Primitive primitive) noexcept
{
  switch (primitive)
  {
  case Mesh::Primitive::TRIANGLE:
    if (m_PrimitiveIDs[TRIANGLE] != INDEX_ERROR)
    {
      Log::Warn("Primitive: Triangle Mesh already loaded.");
      return m_PrimitiveIDs[TRIANGLE];
    }
    Log::Trace("Loading primitive: Triangle Mesh.");
    m_MeshArray.emplace_back(make_unique<TriangleMesh>());
    break;
  case Mesh::Primitive::CUBE:
    if (m_PrimitiveIDs[CUBE] != INDEX_ERROR)
    {
      Log::Warn("Primitive: Cube Mesh already loaded.");
      return m_PrimitiveIDs[CUBE];
    }
    Log::Trace("Loading primitive: Cube Mesh.");
    m_MeshArray.emplace_back(make_unique<CubeMesh>());
    break;
  default:
    Log::Error("Attempted to load unrecognized primitive.");
    return INDEX_ERROR;
  }

  // Return the new index
  return static_cast<unsigned>(m_MeshArray.size() - 1);
}

unsigned MeshManager::LoadCustomMesh() noexcept
{
  Log::Error("LoadCustomMesh not implemented yet!");
  return INDEX_ERROR;
}

void MeshManager::UnloadMeshes() noexcept
{
  Log::Error("UnloadMeshes not implemented yet!");
}

void MeshManager::RenderMesh(unsigned id) const noexcept
{
  if (id == INDEX_ERROR)
  {
    Log::Error("RenderMesh: Mesh not loaded!");
    return;
  }
  m_MeshArray[id]->Bind();
  //TODO: GetElementCount? Instead to save the 3 * every frame?
  glDrawElements(GL_TRIANGLES, 3 * m_MeshArray[id]->GetFaceCount(), GL_UNSIGNED_INT, 0);
}
