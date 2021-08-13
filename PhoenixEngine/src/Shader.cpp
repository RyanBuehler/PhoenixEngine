#include "pch.h"
#include "GLEW/glew.h"
#include "VertexAttribute.h"
#include "Shader.h"

Shader::Shader(GLint shaderType, const string& fileName) noexcept :
  m_ShaderType(shaderType),
  m_fileName(fileName)
{
  if (!LoadShader(fileName))
  {
    Log::Error(string("Could not compile shader - ") + m_fileName);
    LogShaderInfo();
  };
}


Shader::~Shader() noexcept
{
  glDeleteShader(m_ShaderID);
}

GLuint Shader::GetShaderID() const noexcept
{
  return m_ShaderID;
}

bool Shader::LoadShader(const string& fileName) noexcept
{
  GLint result;

  // load fragment shader
  ifstream file;
  string path(Paths::SHADER_PATH);
  path += fileName.c_str();
  file.open(path.c_str(), std::ios::in);
  if (!file.is_open())
  {
    Log::Error(string("Could not open shader file: ") + path);
    return false;
  }
  string fstring((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  // compile fragment shader
  m_ShaderID = glCreateShader(m_ShaderType);
  const char* const& fs = fstring.c_str();
  glShaderSource(m_ShaderID, 1, &fs, 0);
  glCompileShader(m_ShaderID);
  glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    LogShaderInfo();
    return false;
  }

  return true;
}

void Shader::LogShaderInfo() const noexcept
{
  char buffer[1024];
  glGetShaderInfoLog(m_ShaderID, 1024, 0, buffer);
  Log::Error(buffer);
}