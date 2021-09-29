//------------------------------------------------------------------------------
// File:    MeshManager.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Contains static primitives and functionality to load custom meshes.
//------------------------------------------------------------------------------
#include "pch.h"
#include "MeshManager.h"

MeshManager::MeshManager() noexcept :
  m_MeshArray()
{}

unsigned MeshManager::LoadMeshFromOBJ(const string& fileName) noexcept
{
  Log::Error("LoadMeshFromOBJ not implemented yet!");
  return MESH_INDEX_ERROR;
}

void MeshManager::UnloadMeshes() noexcept
{
  Log::Error("UnloadMeshes not implemented yet!");
}

void MeshManager::RenderMesh(unsigned id) const noexcept
{
  if (id == MESH_INDEX_ERROR)
  {
    Log::Error("RenderMesh: Mesh not loaded!");
    return;
  }
  
  glBindVertexArray(m_MeshArray[id].VertexArrayID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshArray[id].TriangleArrayID);

  //TODO: GetElementCount? Instead to save the 3 * every frame?
  glDrawElements(GL_TRIANGLES, 3 * m_MeshArray[id].Mesh.GetTriangleCount(), GL_UNSIGNED_INT, 0);
}
