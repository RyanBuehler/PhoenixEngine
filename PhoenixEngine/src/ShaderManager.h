//------------------------------------------------------------------------------
// File:    ShaderManager.h
// Author:  Ryan Buehler
// Created: Nov 4, 2021
// Desc:    Managers loading/unloading shaders
//------------------------------------------------------------------------------
#pragma once
#include "GLEW/glew.h"
#include "GraphicsCommon.h"

class ShaderManager
{
public:
  ShaderManager() noexcept;
  ~ShaderManager();
  ShaderManager(const ShaderManager&) = delete;
  ShaderManager& operator=(const ShaderManager&) = delete;
  ShaderManager(ShaderManager&&) = delete;
  ShaderManager& operator=(ShaderManager&&) = delete;

  unsigned GetVertexShaderID(Shader::Vertex shader) const noexcept;
  unsigned GetFragmentShaderID(Shader::Fragment shader) const noexcept;

  inline size_t GetVertexShaderCount() const noexcept { return m_VertexShaders.size(); }
  inline size_t GetFragmentShaderCount() const noexcept { return m_FragmentShaders.size(); }

  void ReloadShaders() noexcept;

private:
  GLint LoadShader(const string& fileName, GLenum shaderType) noexcept;
  void RetrieveShaderLog(GLint shaderID, string& log) const noexcept;
  
  void LoadShaders() noexcept;
  void UnloadShaders() noexcept;

  array<GLint, static_cast<unsigned>(Shader::Vertex::COUNT)> m_VertexShaders;
  array<GLint, static_cast<unsigned>(Shader::Fragment::COUNT)> m_FragmentShaders;
};
