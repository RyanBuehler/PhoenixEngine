#include "pch.h"
#include "VertexArrayObject.h"
#include "GLEW/glew.h"

VertexArrayObject::VertexArrayObject() noexcept :
  m_BufferID(0u),
  m_Vertices(),
  m_IsDirty(true),
  m_DataUsage(Graphics::DataUsage::DYNAMIC),
  IBindable()
{
  // Generate the Buffer and ID
  glGenBuffers(1, &m_BufferID);
}

VertexArrayObject::VertexArrayObject(Graphics::DataUsage dataUsage) noexcept :
  m_BufferID(0u),
  m_Vertices(),
  m_IsDirty(true),
  m_DataUsage(dataUsage),
  IBindable()
{
  // Generate the Buffer and ID
  glGenBuffers(1, &m_BufferID);
}

VertexArrayObject::~VertexArrayObject() noexcept
{
  glDeleteBuffers(1, &m_BufferID);
  m_Vertices.clear();
}

void VertexArrayObject::Bind() noexcept
{
  glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

  if (m_IsDirty)
  {
    switch (m_DataUsage)
    {
    case Graphics::DataUsage::STREAM:
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size(), m_Vertices.data(), GL_STREAM_DRAW);
      break;
    case Graphics::DataUsage::DYNAMIC:
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size(), m_Vertices.data(), GL_DYNAMIC_DRAW);
      break;
    case Graphics::DataUsage::STATIC:
    default:
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);
      break;
    }
  }
}

//inline void VertexArrayObject::AddVertex(const Vertex& vertex) noexcept
//{
//  AddVertex(vertex.X, vertex.Y, vertex.Z);
//}

void VertexArrayObject::AddVertex(float x, float y, float z) noexcept
{
  m_IsDirty = true;

  m_Vertices.push_back(x);
  m_Vertices.push_back(y);
  m_Vertices.push_back(z);
}

inline const float* VertexArrayObject::GetVertexArray() const noexcept
{
  return m_Vertices.data();
}

inline void VertexArrayObject::SetDataUsage(Graphics::DataUsage dataUsage) noexcept
{
  m_IsDirty = true;

  m_DataUsage = dataUsage;
}

inline Graphics::DataUsage VertexArrayObject::GetDataUsage() const noexcept
{
  return m_DataUsage;
}

void VertexArrayObject::ClearArray() noexcept
{
  m_Vertices.clear();
}
