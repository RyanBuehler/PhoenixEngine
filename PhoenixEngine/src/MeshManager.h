//------------------------------------------------------------------------------
// File:    MeshManager.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Class definitions for MeshManager class
//------------------------------------------------------------------------------
#pragma once
#include "Mesh.h"
#include "GLEW/glew.h"

class MeshManager
{
public:
  static constexpr unsigned MESH_INDEX_ERROR = numeric_limits<unsigned>::max();

private:
  struct MeshData
  {
    Mesh Mesh;
    string FileName;
    GLuint VertexArrayID;
    GLuint TriangleArrayID;
  };

public:
  MeshManager() noexcept;
  ~MeshManager() = default;
  MeshManager(const MeshManager&) = delete;
  MeshManager& operator=(const MeshManager&) = delete;
  MeshManager(MeshManager&&) = delete;
  MeshManager& operator=(MeshManager&&) = delete;

  unsigned LoadMeshFromOBJ(const string& fileName) noexcept;

  void UnloadMeshes() noexcept;

  void RenderMesh(unsigned id) const noexcept;

private:
  vector<MeshData> m_MeshArray;
};
