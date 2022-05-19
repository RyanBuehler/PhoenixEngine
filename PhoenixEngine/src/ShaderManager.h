//------------------------------------------------------------------------------
// File:    ShaderManager.h
// Author:  Ryan Buehler
// Created: Nov 4, 2021
// Desc:    Managers loading/unloading shaders
//------------------------------------------------------------------------------
#pragma once
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

  unsigned GetVertexShaderID(Shader::Vertex Shader) const noexcept;
  unsigned GetFragmentShaderID(Shader::Fragment Shader) const noexcept;

  inline size_t GetVertexShaderCount() const noexcept { return m_VertexShaders.size(); }
  inline size_t GetFragmentShaderCount() const noexcept { return m_FragmentShaders.size(); }

  bool RelinkShader(GLuint& ProgramID, GLint VertexShaderID, GLint FragmentShaderID, const char* VertexFile, const char* FragmentFile) const noexcept;

private:
  static GLint CreateShader(GLenum ShaderType) noexcept;
  bool LoadShader(GLint ShaderID, const string& FileName) const noexcept;
  GLint CreateAndLoadShader(const string& FileName, GLenum ShaderType) const;

  void LoadShaders() noexcept;
  void UnloadShaders() noexcept;

  array<GLint, static_cast<unsigned>(Shader::Vertex::COUNT)> m_VertexShaders;
  array<GLint, static_cast<unsigned>(Shader::Fragment::COUNT)> m_FragmentShaders;
};
