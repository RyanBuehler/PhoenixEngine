#include "pch.h"
#include "GLEW/glew.h"
#include "CubeMesh.h"

const Mesh::Position CubeMesh::m_VertexPositions[8] =
{
  // Bottom Half
  Mesh::Position(-1,-1,1),  // 0
  Mesh::Position(1,-1,1),   // 1
  Mesh::Position(1,-1,-1),  // 2
  Mesh::Position(-1,-1,-1), // 3

  // Top Half
  Mesh::Position(-1,1,1),   // 4
  Mesh::Position(1,1,1),    // 5
  Mesh::Position(1,1,-1),   // 6
  Mesh::Position(-1,1,-1),  // 7
};

const Mesh::Edge CubeMesh::m_EdgeIndices[12] =
{
  Mesh::Edge(0,1),
  Mesh::Edge(1,2),
  Mesh::Edge(2,3),
  Mesh::Edge(3,0),
  Mesh::Edge(0,4),
  Mesh::Edge(1,5),
  Mesh::Edge(2,6),
  Mesh::Edge(3,7),
  Mesh::Edge(4,5),
  Mesh::Edge(5,6),
  Mesh::Edge(6,7),
  Mesh::Edge(7,4),
};

const Mesh::Triangle CubeMesh::m_FaceIndices[12] =
{
  // Bottom Face
Mesh::Triangle(0,3,1),
Mesh::Triangle(1,3,2),
// Left Face
Mesh::Triangle(0,4,3),
Mesh::Triangle(3,4,7),
// Right Face
Mesh::Triangle(1,2,5),
Mesh::Triangle(2,6,5),
// Front Face
Mesh::Triangle(0,1,4),
Mesh::Triangle(1,5,4),
// Back Face
Mesh::Triangle(3,7,6),
Mesh::Triangle(6,2,3),
// Top Face
Mesh::Triangle(4,6,7),
Mesh::Triangle(4,5,6),
};

CubeMesh::CubeMesh() noexcept :
  m_PositionBufferID(numeric_limits<unsigned>::max()),
  m_FaceBufferID(numeric_limits<unsigned>::max()),
  m_VertexArrayID(numeric_limits<unsigned>::max()),
  IMesh(Graphics::DataUsage::STATIC)
{
  GLenum dataUsage;
  switch (m_Usage)
  {
  case Graphics::DataUsage::DYNAMIC:
    dataUsage = GL_DYNAMIC_DRAW;
    break;
  case Graphics::DataUsage::STATIC:
  default:
    dataUsage = GL_STATIC_DRAW;
    break;
  }

  // The Position buffer
  glGenBuffers(1, &m_PositionBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_PositionBufferID);
  glBufferData(GL_ARRAY_BUFFER, GetVertexCount() * sizeof(Mesh::Position), GetPositionArray(), dataUsage);

  // The Face buffer
  glGenBuffers(1, &m_FaceBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetTriangleCount() * sizeof(Mesh::Triangle), GetTriangleArray(), dataUsage);
  
  glGenVertexArrays(1, &m_VertexArrayID);
  glBindVertexArray(m_VertexArrayID);

  // The '0' here is the attribute location
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Position), 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0u);

  Log::Trace("Cube Mesh loaded.");
}

CubeMesh::~CubeMesh()
{
  glDeleteVertexArrays(1, &m_VertexArrayID);

  glDeleteBuffers(1, &m_PositionBufferID);
  glDeleteBuffers(1, &m_FaceBufferID);

  Log::Trace("Cube Mesh destroyed.");
}

void CubeMesh::Bind() noexcept
{
  glBindVertexArray(m_VertexArrayID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceBufferID);
}

unsigned CubeMesh::GetVertexCount() const noexcept
{
  return 8;
}

unsigned CubeMesh::GetEdgeCount() const noexcept
{
  return 12;
}

unsigned CubeMesh::GetTriangleCount() const noexcept
{
  return 12;
}

const Mesh::Position* CubeMesh::GetPositionArray() const noexcept
{
  return &m_VertexPositions[0];
}

const Mesh::Edge* CubeMesh::GetEdgeArray() const noexcept
{
  return &m_EdgeIndices[0];
}

const Mesh::Triangle* CubeMesh::GetTriangleArray() const noexcept
{
  return &m_FaceIndices[0];
}
