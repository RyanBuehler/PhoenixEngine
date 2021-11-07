//------------------------------------------------------------------------------
// File:    ShaderManager.cpp
// Author:  Ryan Buehler
// Created: Nov 4, 2021
// Desc:    Managers loading/unloading shaders
//------------------------------------------------------------------------------
#include "pch.h"
#include "Paths.h"
#include "ShaderManager.h"

#define GL_SHADER_ERROR -1

ShaderManager::ShaderManager() noexcept :
  m_VertexShaders(),
  m_FragmentShaders()
{
  LoadShaders();
}

ShaderManager::~ShaderManager()
{
  UnloadShaders();
}

unsigned ShaderManager::GetVertexShaderID(Shader::Vertex shader) const noexcept
{
  return m_VertexShaders[static_cast<unsigned>(shader)];
}

unsigned ShaderManager::GetFragmentShaderID(Shader::Fragment shader) const noexcept
{
  return m_FragmentShaders[static_cast<unsigned>(shader)];
}

void ShaderManager::ReloadShaders() noexcept
{
  Log::Trace("Reloading Shaders");
  LoadShaders();
}

GLint ShaderManager::LoadShader(const string& fileName, GLenum shaderType) noexcept
{
  string path = Paths::SHADER_PATH + fileName;
  // load fragment shader
  ifstream file;
  file.open(path.c_str(), std::ios::in);
  if (!file.is_open())
  {
    Log::Error(string("Could not open shader file: ") + path);
    return GL_SHADER_ERROR;
  }
  string fstring((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  // compile the shader
  GLint id = glCreateShader(shaderType);
  const char* const& fs = fstring.c_str();
  glShaderSource(id, 1, &fs, 0);
  glCompileShader(id);

  GLint result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    Log::Error(string("Error compiling shader : ") + fileName);
    string error;
    RetrieveShaderLog(id, error);
    Log::Error(error);
    return GL_SHADER_ERROR;
  }

  return id;
}

void ShaderManager::RetrieveShaderLog(GLint shaderID, string& log) const noexcept
{
  int logLength;
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
  char* shaderLog = new char[logLength];
  glGetShaderInfoLog(shaderID, logLength, &logLength, shaderLog);
  log.clear();
  log = shaderLog;
  delete[] shaderLog;
}

void ShaderManager::LoadShaders() noexcept
{
  UnloadShaders();

  // TODO: This should happen automatically or at least in 1 place

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::PHONGLIGHT)] =
    LoadShader("PhongLighting.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::PHONGLIGHT)] =
    LoadShader("PhongLighting.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::DIFFUSE)] =
    LoadShader("Diffuse.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::DIFFUSE)] =
    LoadShader("Diffuse.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::DEBUG)] =
    LoadShader("Debug.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::DEBUG)] =
    LoadShader("Debug.frag", GL_FRAGMENT_SHADER);
}

void ShaderManager::UnloadShaders() noexcept
{
  for (GLint id : m_VertexShaders)
  {
    glDeleteShader(id);
  }
  m_VertexShaders.fill(-1);

  for (GLint id : m_FragmentShaders)
  {
    glDeleteShader(id);
  }
  m_FragmentShaders.fill(-1);
}
