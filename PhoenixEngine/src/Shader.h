#pragma once
#include "GL/glew.h"
#include "IBindable.h"
#include "VertexAttribute.h"

class Shader : public IBindable
{
public:
  enum class Type
  {
    UNKNOWN = 0,
    VERTEX,
    FRAGMENT
  };
public:

  Shader(Shader::Type shaderType, const string& fileName) noexcept;
  ~Shader() noexcept;


  const string& GetShaderFileName() const noexcept;
  const string& GetShaderFilePath() const noexcept;
  Shader::Type GetShaderType() const noexcept;
  GLuint GetGLShaderType() const noexcept;
  GLuint GetShaderID() const noexcept;
  bool ShaderIsLoaded() const noexcept;

  void RetrieveShaderLog(string& log) const noexcept;

private:
  GLuint GetGLShaderType(Type type) const noexcept;
  void LoadShader() noexcept;

protected:
  const string m_fileName;
  const string m_filePath;

  const Shader::Type m_ShaderType;
  const GLuint m_GLShaderType;

  GLuint m_ShaderID;
  bool m_bShaderIsLoaded;

  vector<VertexAttribute<float>> m_Attributes;
};
