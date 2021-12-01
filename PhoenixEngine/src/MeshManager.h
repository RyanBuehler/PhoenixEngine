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
      GLuint positionBufferID = Error::INVALID_INDEX,
      GLuint normalBufferID = Error::INVALID_INDEX,
      GLuint triangleBufferID = Error::INVALID_INDEX,
      GLuint texcoordBufferID = Error::INVALID_INDEX,
      GLuint vertexArrayID = Error::INVALID_INDEX) :
      FileName(fileName),
      PositionBufferID(positionBufferID),
      NormalBufferID(normalBufferID),
      TriangleBufferID(triangleBufferID),
      TexcoordBufferID(texcoordBufferID),
      VertexArrayID(vertexArrayID)
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
    const string& fileName,
    bool scaleToUnitSize = false,
    bool resetOrigin = false,
    UV::Generation uvGeneration = UV::Generation::PLANAR) noexcept;

  void UnloadMeshes() noexcept;

  void RenderMesh(unsigned id) const noexcept;

  void RenderSurfaceNormals(unsigned id, float length) const noexcept;

  void RenderVertexNormals(unsigned id, float length) const noexcept;

  const Mesh& GetMeshByID(unsigned id) const noexcept;

private:
  vector<Mesh> m_MeshArray;
  vector<MeshData> m_MeshDataArray;
  OBJReader m_OBJReader;

  unsigned LoadMeshFromOBJ(const string& fileName) noexcept;
  unsigned LoadSphere(float radius = 1.f, int numDivisions = 16) noexcept;
};
