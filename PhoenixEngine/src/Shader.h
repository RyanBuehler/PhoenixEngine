#pragma once
#include "IBindable.h"
#include "VertexAttribute.h"

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

  vector<VertexAttribute<float>> m_Attributes;
};
