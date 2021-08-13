#pragma once
#include "IBindable.h"

class VertexAttribute;

class Shader : public IBindable
{
public:

  Shader(GLint shaderType, const string& fileName) noexcept;
  ~Shader() noexcept;

  bool LoadShader(const string& filePath) noexcept;
  GLuint GetShaderID() const noexcept;

  void LogShaderInfo() const noexcept;

protected:
  string m_fileName;

  const GLint m_ShaderType;
  GLuint m_ShaderID;

  vector<VertexAttribute> m_Attributes;
};
