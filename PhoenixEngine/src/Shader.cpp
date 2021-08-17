#include "pch.h"
#include "Shader.h"

#include <filesystem>

Shader::Shader(Shader::Type shaderType, const string& fileName) noexcept :
  m_fileName(fileName),
  m_filePath(Paths::SHADER_PATH + fileName),
  m_ShaderType(shaderType),
  m_GLShaderType(GetGLShaderType(shaderType)),
  m_ShaderID(numeric_limits<unsigned>::max()),
  m_bShaderIsLoaded(false)
{
  LoadShader();
}

Shader::~Shader() noexcept
{
  glDeleteShader(m_ShaderID);
}

GLuint Shader::GetShaderID() const noexcept
{
  return m_ShaderID;
}

void Shader::LoadShader() noexcept
{
  GLint result;

  std::filesystem::path p = m_filePath;
  std::cout << "Current path is " << std::filesystem::current_path() << '\n';
  std::cout << "Absolute path for " << p << " is "
    << std::filesystem::absolute(p) << '\n';

  // load fragment shader
  ifstream file;
  file.open(m_filePath.c_str(), std::ios::in);
  if (!file.is_open())
  {
    Log::Error(string("Could not open shader file: ") + m_filePath);
    return;
  }
  string fstring((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  // compile fragment shader
  m_ShaderID = glCreateShader(m_GLShaderType);
  const char* const& fs = fstring.c_str();
  glShaderSource(m_ShaderID, 1, &fs, 0);
  glCompileShader(m_ShaderID);
  glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    Log::Error(m_fileName + " - error compiling shader:");
    string error;
    RetrieveShaderLog(error);
    Log::Error(error);
    return;
  }

  m_bShaderIsLoaded = true;
}

const string& Shader::GetShaderFileName() const noexcept
{
  return m_fileName;
}

const string& Shader::GetShaderFilePath() const noexcept
{
  return m_filePath;
}

Shader::Type Shader::GetShaderType() const noexcept
{
  return m_ShaderType;
}

GLuint Shader::GetGLShaderType() const noexcept
{
  return m_GLShaderType;
}

bool Shader::ShaderIsLoaded() const noexcept
{
  return m_bShaderIsLoaded;
}

void Shader::RetrieveShaderLog(string& log) const noexcept
{
  int logLength;
  glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &logLength);
  char* shaderLog = new char[logLength];
  glGetShaderInfoLog(m_ShaderID, logLength, &logLength, shaderLog);
  log.clear();
  log = shaderLog;
  delete[] shaderLog;
}

GLuint Shader::GetGLShaderType(Type type) const noexcept
{
  switch (type)
  {
  case Type::VERTEX:
    return GL_VERTEX_SHADER;
  case Type::FRAGMENT:
    return GL_FRAGMENT_SHADER;

  default:
  case Type::UNKNOWN:
    Log::Error("Shader type unknown.");
    return numeric_limits<unsigned>::max();
  }
}
