//------------------------------------------------------------------------------
// File:    MeshManager.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Class definitions for MeshManager class
//------------------------------------------------------------------------------
#pragma once
#include "Mesh.h"
#include "GLEW/glew.h"
#include "OBJReader.h"

class MeshManager
{
private:
  struct MeshData
  {
    MeshData(const string& fileName = "Unknown",
             const GLuint PositionBufferId = Error::INVALID_INDEX,
      const GLuint NormalBufferId = Error::INVALID_INDEX,
      const GLuint TriangleBufferId = Error::INVALID_INDEX,
      const GLuint TexcoordBufferId = Error::INVALID_INDEX,
      const GLuint VertexArrayId = Error::INVALID_INDEX) :
      FileName(fileName),
      PositionBufferId(PositionBufferId),
      TriangleBufferId(TriangleBufferId),
      NormalBufferId(NormalBufferId),
      TexcoordBufferId(TexcoordBufferId),
      VertexArrayId(VertexArrayId)
    {}

    string FileName;
    GLuint PositionBufferId;
    GLuint TriangleBufferId;
    GLuint NormalBufferId;
    GLuint TexcoordBufferId;
    GLuint VertexArrayId;
  };

public:
  MeshManager() noexcept;
  ~MeshManager();
  MeshManager(const MeshManager&) = delete;
  MeshManager& operator=(const MeshManager&) = delete;
  MeshManager(MeshManager&&) = delete;
  MeshManager& operator=(MeshManager&&) = delete;

  unsigned LoadMesh(
    const string& FileName,
    bool ScaleToUnitSize = false,
    bool ResetOrigin = false,
    UV::Generation UvGeneration = UV::Generation::PLANAR) noexcept;

  void UnloadMeshes() noexcept;

  void RenderMesh(unsigned Id) const noexcept;

  void RenderSurfaceNormals(unsigned Id, float Length) const noexcept;

  void RenderVertexNormals(unsigned Id, float Length) const noexcept;

  const Mesh& GetMeshByID(unsigned Id) const noexcept;

private:
  vector<Mesh> m_MeshArray;
  vector<MeshData> m_MeshDataArray;
  OBJReader m_ObjReader;

  unsigned LoadMeshFromOBJ(const string& FileName) noexcept;
  unsigned LoadSphere(float Radius = 1.f, int NumDivisions = 16) noexcept;
};
