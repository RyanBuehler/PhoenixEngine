#pragma once
#include "IMesh.h"

class CubeMesh : public Mesh::IMesh
{
public:

  CubeMesh() noexcept;
  ~CubeMesh();
  CubeMesh(const CubeMesh&) = delete;
  CubeMesh& operator=(const CubeMesh&) = delete;
  CubeMesh(CubeMesh&&) = delete;
  CubeMesh& operator=(CubeMesh&&) = delete;

  void Bind() noexcept;

  unsigned GetVertexCount() const noexcept;
  unsigned GetEdgeCount() const noexcept;
  unsigned GetFaceCount() const noexcept;

  const Mesh::Position* GetPositionArray() const noexcept;
  const Mesh::Edge* GetEdgeArray() const noexcept;
  const Mesh::Face* GetFaceArray() const noexcept;

private:
  GLuint m_PositionBufferID;
  GLuint m_FaceBufferID;
  GLuint m_VertexArrayID;

  static const Mesh::Position m_VertexPositions[8];
  static const Mesh::Edge m_EdgeIndices[12];
  static const Mesh::Face m_FaceIndices[12];
};
