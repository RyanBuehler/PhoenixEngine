#include "pch.h"
#include "GL/glew.h"
#include "VertexAttribute.h"

template <typename T>
VertexAttribute<T>::VertexAttribute(GLuint programID, const string& attributeName, unsigned size) noexcept :
  m_AttributeName(attributeName),
  m_Size(size)
{
  m_AttributeID = glGetUniformLocation(programID, m_AttributeName.c_str());
  if (m_Size < 1 || m_Size > 4)
  {
    Log::Error(attributeName + ": Vertex Attribute must be between 1-4");
    m_Size = 0;
  }
}

template<typename T>
inline unsigned VertexAttribute<T>::GetCount() const noexcept
{
  return m_Size;
}
