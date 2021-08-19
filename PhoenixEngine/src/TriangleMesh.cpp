#include "pch.h"
#include "GLEW/glew.h"
#include "TriangleMesh.h"

const Mesh::Position TriangleMesh::m_VertexPositions[3] =
{
  Mesh::Position(-0.2f, -0.2f, 0.f),
  Mesh::Position(0.f, 0.2f, 0.f),
  Mesh::Position(0.2f, -0.2f, 0.f),
};

const Mesh::Edge TriangleMesh::m_EdgeIndices[3] =
{
  Mesh::Edge(0,1),
  Mesh::Edge(1,2),
  Mesh::Edge(2,0),
};

const Mesh::Face TriangleMesh::m_FaceIndices[1] =
{
  Mesh::Face(0,1,2)
};

TriangleMesh::TriangleMesh() noexcept :
  IMesh()
{
  glGenBuffers(1, &m_PositionBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_PositionBufferID);
  switch (m_Usage)
  {
  case Graphics::DataUsage::DYNAMIC:
    glBufferData(GL_ARRAY_BUFFER, GetVertexCount() * sizeof(Mesh::Position), GetPositionArray(), GL_DYNAMIC_DRAW);
    break;
  case Graphics::DataUsage::STATIC:
  default:
    glBufferData(GL_ARRAY_BUFFER, GetVertexCount() * sizeof(Mesh::Position), GetPositionArray(), GL_STATIC_DRAW);
    break;
  }
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Position), 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0u);
}

void TriangleMesh::Bind() noexcept
{
  //TODO:
}

unsigned TriangleMesh::GetVertexCount() const noexcept
{
  return 3;
}

unsigned TriangleMesh::GetEdgeCount() const noexcept
{
  return 3;
}

unsigned TriangleMesh::GetFaceCount() const noexcept
{
  return 1;
}

const Mesh::Position* TriangleMesh::GetPositionArray() const noexcept
{
  return &m_VertexPositions[0];
}

const Mesh::Edge* TriangleMesh::GetEdgeArray() const noexcept
{
  return &m_EdgeIndices[0];
}

const Mesh::Face* TriangleMesh::GetFaceArray() const noexcept
{
  return &m_FaceIndices[0];
}
