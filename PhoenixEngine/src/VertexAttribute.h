#pragma once

class VertexAttribute
{
public:

  VertexAttribute(GLuint programID, const string& attributeName) noexcept;
  ~VertexAttribute() = default;
  VertexAttribute(const VertexAttribute&) = delete;
  VertexAttribute& operator=(const VertexAttribute&) = delete;
  VertexAttribute(VertexAttribute&&) = delete;
  VertexAttribute& operator=(VertexAttribute&&) = delete;

private:
  unsigned m_AttributeID; 
  string m_AttributeName;
};
