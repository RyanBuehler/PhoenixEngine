#pragma once
#include "IMesh.h"

class TriangleMesh : public Mesh::IMesh
{
public:

  TriangleMesh() noexcept;
  ~TriangleMesh();
  TriangleMesh(const TriangleMesh&) = delete;
  TriangleMesh& operator=(const TriangleMesh&) = delete;
  TriangleMesh(TriangleMesh&&) = delete;
  TriangleMesh& operator=(TriangleMesh&&) = delete;

  void Bind() noexcept;

  unsigned GetVertexCount() const noexcept;
  unsigned GetEdgeCount() const noexcept;
  unsigned GetTriangleCount() const noexcept;

  const Mesh::Position* GetPositionArray() const noexcept;
  const Mesh::Edge* GetEdgeArray() const noexcept;
  const Mesh::Triangle* GetTriangleArray() const noexcept;

private:
  GLuint m_PositionBufferID;
  static const Mesh::Position m_VertexPositions[3];
  static const Mesh::Edge m_EdgeIndices[3];
  static const Mesh::Triangle m_FaceIndices[1];
};
