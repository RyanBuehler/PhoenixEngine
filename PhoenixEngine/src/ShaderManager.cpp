//------------------------------------------------------------------------------
// File:    ShaderManager.cpp
// Author:  Ryan Buehler
// Created: Nov 4, 2021
// Desc:    Managers loading/unloading shaders
//------------------------------------------------------------------------------
#include "pch.h"
#include "Paths.h"
#include "ShaderManager.h"

#define GL_SHADER_ERROR (-1)

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

unsigned ShaderManager::GetVertexShaderID(Shader::Vertex Shader) const noexcept
{
  return m_VertexShaders[static_cast<unsigned>(Shader)];
}

unsigned ShaderManager::GetFragmentShaderID(Shader::Fragment Shader) const noexcept
{
  return m_FragmentShaders[static_cast<unsigned>(Shader)];
}

bool ShaderManager::RelinkShader(GLuint& ProgramID, const GLint VertexShaderID, const GLint FragmentShaderID,
  const char* VertexFile, const char* FragmentFile) const noexcept
{
  glDeleteProgram(ProgramID);

  //TODO: hard coded for testing purposes only
  if(!LoadShader(VertexShaderID, VertexFile))
  {
    return false;
  }

  if(!LoadShader(FragmentShaderID, FragmentFile))
  {
    return false;
  }

  ProgramID = glCreateProgram();

  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);

  glLinkProgram(ProgramID);

  GLint result;
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
  if (!result)
  {
    Log::Error("Error linking OpenGL program.");
    string error;
    Graphics::retrieve_program_log(ProgramID, error);
    Log::Error(error);
    return false;
  }

  return true;
}

GLint ShaderManager::CreateShader(const GLenum ShaderType) noexcept
{
  return glCreateShader(ShaderType);
}

bool ShaderManager::LoadShader(const GLint ShaderID, const string& FileName) const noexcept
{
  const string path = Paths::SHADER_PATH + FileName;
  // load fragment shader
  ifstream file;
  file.open(path.c_str(), std::ios::in);
  if (!file.is_open())
  {
    Log::Error(string("Could not open shader file: ") + path);
    return false;
  }
  const string fileString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  // compile the shader
  const char* const& fs = fileString.c_str();
  glShaderSource(ShaderID, 1, &fs, nullptr);
  glCompileShader(ShaderID);

  GLint result;
  glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    Log::Error(string("Error compiling shader : ") + FileName);
    string error;
    Graphics::retrieve_shader_log(ShaderID, error);
    Log::Error(error);
    return false;
  }

  return true;
}

GLint ShaderManager::CreateAndLoadShader(const string& FileName, const GLenum ShaderType) const
{
  const GLint id = CreateShader(ShaderType);
  if (!LoadShader(id, FileName))
  {
    Log::Error("Failed to Create and Load shader: " + FileName);
    return GL_SHADER_ERROR;
  }
  return id;
}

void ShaderManager::LoadShaders() noexcept
{
  UnloadShaders();

  // TODO: This should happen automatically or at least in 1 place

  //m_VertexShaders[static_cast<unsigned>(Shader::Vertex::DIFFUSE)] =
  //  CreateAndLoadShader("Diffuse.vert", GL_VERTEX_SHADER);
  //m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::DIFFUSE)] =
  //  CreateAndLoadShader("Diffuse.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::SKYBOX)] =
    CreateAndLoadShader("Skybox.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::SKYBOX)] =
    CreateAndLoadShader("Skybox.frag", GL_FRAGMENT_SHADER);

  //m_VertexShaders[static_cast<unsigned>(Shader::Vertex::REFLECTION)] =
  //  CreateAndLoadShader("Reflection.vert", GL_VERTEX_SHADER);
  //m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::REFLECTION)] =
  //  CreateAndLoadShader("Reflection.frag", GL_FRAGMENT_SHADER);

  //m_VertexShaders[static_cast<unsigned>(Shader::Vertex::PHONGLIGHT)] =
  //  CreateAndLoadShader("PhongLighting.vert", GL_VERTEX_SHADER);
  //m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::PHONGLIGHT)] =
  //  CreateAndLoadShader("PhongLighting.frag", GL_FRAGMENT_SHADER);

  //m_VertexShaders[static_cast<unsigned>(Shader::Vertex::PHONGSHADE)] =
  //  CreateAndLoadShader("PhongShading.vert", GL_VERTEX_SHADER);
  //m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::PHONGSHADE)] =
  //  CreateAndLoadShader("PhongShading.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::BLINN_PHONG)] =
    CreateAndLoadShader("BlinnPhong.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::BLINN_PHONG)] =
    CreateAndLoadShader("BlinnPhong.frag", GL_FRAGMENT_SHADER);

  //m_VertexShaders[static_cast<unsigned>(Shader::Vertex::BLINNPHONGREFRACT)] =
  //  CreateAndLoadShader("BlinnPhongRefract.vert", GL_VERTEX_SHADER);
  //m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::BLINNPHONGREFRACT)] =
  //  CreateAndLoadShader("BlinnPhongRefract.frag", GL_FRAGMENT_SHADER);

  //m_VertexShaders[static_cast<unsigned>(Shader::Vertex::PHONGTEXTURE)] =
  //  CreateAndLoadShader("PhongTexture.vert", GL_VERTEX_SHADER);
  //m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::PHONGTEXTURE)] =
  //  CreateAndLoadShader("PhongTexture.frag", GL_FRAGMENT_SHADER);

  m_VertexShaders[static_cast<unsigned>(Shader::Vertex::DEBUG)] =
    CreateAndLoadShader("Debug.vert", GL_VERTEX_SHADER);
  m_FragmentShaders[static_cast<unsigned>(Shader::Fragment::DEBUG)] =
    CreateAndLoadShader("Debug.frag", GL_FRAGMENT_SHADER);
}

void ShaderManager::UnloadShaders() noexcept
{
  for (const GLint id : m_VertexShaders)
  {
    glDeleteShader(id);
  }
  m_VertexShaders.fill(-1);

  for (const GLint id : m_FragmentShaders)
  {
    glDeleteShader(id);
  }
  m_FragmentShaders.fill(-1);
}
