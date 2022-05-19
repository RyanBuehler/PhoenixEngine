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
    MeshData(const string& FileName = "Unknown",
             const GLuint PositionBufferID = Error::INVALID_INDEX,
      const GLuint NormalBufferID = Error::INVALID_INDEX,
      const GLuint TriangleBufferID = Error::INVALID_INDEX,
      const GLuint TexcoordBufferID = Error::INVALID_INDEX,
      const GLuint VertexArrayID = Error::INVALID_INDEX) :
      FileName(FileName),
      PositionBufferID(PositionBufferID),
      TriangleBufferID(TriangleBufferID),
      NormalBufferID(NormalBufferID),
      TexcoordBufferID(TexcoordBufferID),
      VertexArrayID(VertexArrayID)
    {}

    string FileName;
    GLuint PositionBufferID;
    GLuint TriangleBufferID;
    GLuint NormalBufferID;
    GLuint TexcoordBufferID;
    GLuint VertexArrayID;
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

  void RenderMesh(unsigned ID) const noexcept;

  void RenderSurfaceNormals(unsigned ID, float Length) const noexcept;

  void RenderVertexNormals(unsigned ID, float Length) const noexcept;

  [[nodiscard]] const Mesh& GetMeshByID(unsigned ID) const noexcept;

private:
  vector<Mesh> m_MeshArray;
  vector<MeshData> m_MeshDataArray;
  OBJReader m_ObjReader;

  unsigned LoadMeshFromOBJ(const string& FileName) noexcept;
  unsigned LoadSphere(float Radius = 1.f, int NumDivisions = 16) noexcept;
};
