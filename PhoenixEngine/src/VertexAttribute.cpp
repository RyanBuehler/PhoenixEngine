#include "pch.h"
#include "GLEW/glew.h"
#include "VertexAttribute.h"

VertexAttribute::VertexAttribute(GLuint programID, const string& attributeName) noexcept :
  m_AttributeName(attributeName)
{
  m_AttributeID = glGetUniformLocation(programID, m_AttributeName.c_str());
}
