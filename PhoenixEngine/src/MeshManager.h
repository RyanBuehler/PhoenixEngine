//------------------------------------------------------------------------------
// File:    MeshManager.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Class definitions for MeshManager class
//------------------------------------------------------------------------------
#pragma once
#include "IMesh.h"

class MeshManager
{
private:
  static constexpr unsigned TRIANGLE = 0;
  static constexpr unsigned CUBE = 1;

  static constexpr unsigned INDEX_ERROR = numeric_limits<unsigned>::max();
public:
  MeshManager() noexcept;
  ~MeshManager() = default;
  MeshManager(const MeshManager&) = delete;
  MeshManager& operator=(const MeshManager&) = delete;
  MeshManager(MeshManager&&) = delete;
  MeshManager& operator=(MeshManager&&) = delete;

  unsigned LoadPrimitive(Mesh::Primitive primitive) noexcept;
  unsigned LoadCustomMesh() noexcept;

  void UnloadMeshes() noexcept;

  void RenderMesh(unsigned id) const noexcept;

private:
  vector<unique_ptr<Mesh::IMesh>> m_MeshArray;

  array<unsigned, 2> m_PrimitiveIDs;
};
