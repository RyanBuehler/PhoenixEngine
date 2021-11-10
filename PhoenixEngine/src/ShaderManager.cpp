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

bool ShaderManager::RelinkShader(GLuint& programID, GLint vertexShaderID, GLint fragmentShaderID, const char* vertexFile, const char* fragmentFile) noexcept
{
  glDeleteProgram(programID);
  
  //TODO: hard coded for testing purposes only
  LoadShader(vertexShaderID, vertexFile);
  LoadShader(fragmentShaderID, fragmentFile);

  programID = glCreateProgram();

  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);

  glLinkProgram(programID);

  GLint result;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (!result)
  {
    Log::Error("Error linking OpenGL program.");
    string error;
    Graphics::RetrieveProgramLog(programID, error);
    Log::Error(error);
    return false;
  }

  return true;
}

GLint ShaderManager::CreateShader(GLenum shaderType) noexcept
{
  return glCreateShader(shaderType);
}

bool ShaderManager::LoadShader(GLint shaderID, const string& fileName) noexcept
{
  string path = Paths::SHADER_PATH + fileName;
  // load fragment shader
  ifstream file;
  file.open(path.c_str(), std::ios::in);
  if (!file.is_open())
  {
    Log::Error(string("Could not open shader file: ") + path);
    return false;
  }
  string fstring((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  // compile the shader
  const char* const& fs = fstring.c_str();
  glShaderSource(shaderID, 1, &fs, 0);
  glCompileShader(shaderID);

  GLint result;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    Log::Error(string("Error compiling shader : ") + fileName);
    string error;
    Graphics::RetrieveShaderLog(shaderID, error);
    Log::Error(error);
    return false;
  }

  return true;
}

GLint ShaderManager::CreateAndLoadShader(const string& fileName, GLenum shaderType)
{
  GLint id = CreateShader(shaderType);
  if (!LoadShader(id, fileName))
  {
    Log::Error("Failed to Create and Load shader: " + fileName);
    return GL_SHADER_ERROR;
  }
  return id;
}

void ShaderManager::LoadShaders() noexcept
{
  UnloadShaders();

  // TODO: This should happen automatically or at least in 1 place

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::PHONGLIGHT)] =
    CreateAndLoadShader("PhongLighting.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::PHONGLIGHT)] =
    CreateAndLoadShader("PhongLighting.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::PHONGSHADE)] =
    CreateAndLoadShader("PhongShading.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::PHONGSHADE)] =
    CreateAndLoadShader("PhongShading.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::PHONGTEXTURE)] =
    CreateAndLoadShader("PhongTexture.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::PHONGTEXTURE)] =
    CreateAndLoadShader("PhongTexture.frag", GL_FRAGMENT_SHADER);

  //m_VertexShaders[static_cast<unsigned>(Shader::Vertex::DIFFUSE)] =
  //  CreateAndLoadShader("Diffuse.vert", GL_VERTEX_SHADER);
  //m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::DIFFUSE)] =
  //  CreateAndLoadShader("Diffuse.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::DEBUG)] =
    CreateAndLoadShader("Debug.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::DEBUG)] =
    CreateAndLoadShader("Debug.frag", GL_FRAGMENT_SHADER);
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
