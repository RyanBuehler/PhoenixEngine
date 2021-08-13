#pragma once

template <typename T>
class VertexAttribute
{
public:

  VertexAttribute(GLuint programID, const string& attributeName, unsigned size) noexcept;
  ~VertexAttribute() = default;
  VertexAttribute(const VertexAttribute&) = delete;
  VertexAttribute& operator=(const VertexAttribute&) = delete;
  VertexAttribute(VertexAttribute&&) = delete;
  VertexAttribute& operator=(VertexAttribute&&) = delete;

  inline unsigned GetCount() const noexcept;

private:
  unsigned m_AttributeID; 
  string m_AttributeName;
  unsigned m_Size;
};
