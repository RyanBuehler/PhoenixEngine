//------------------------------------------------------------------------------
// File:    ContextManager.cpp
// Author:  Ryan Buehler
// Created: September 27, 2021
// Desc:    Manages the Contexts of the Rendering Engine
//------------------------------------------------------------------------------
#include "pch.h"
#include "ContextManager.h"

ContextManager::ContextManager() noexcept :
  m_CurrentContextIndex(Error::Context::INVALID_CONTEXT)
{
}

ContextManager::~ContextManager()
{
  for (const Context& context : m_Contexts)
  {
    glDeleteProgram(context.ProgramID);
  }
}

unsigned ContextManager::CreateNewContext(const string& Name, const GLint VertexShaderID, const GLint FragmentShaderID)
{
  const auto i = static_cast<unsigned>(m_Contexts.size());

  GLint programID = glCreateProgram();

  glAttachShader(programID, VertexShaderID);
  glAttachShader(programID, FragmentShaderID);

  glLinkProgram(programID);

  GLint result;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (!result)
  {
    Log::error("Error linking OpenGL program.");
    string error;
    Graphics::retrieve_program_log(programID, error);
    Log::error(error);
    return Error::OpenGL::PROGRAM_ERROR;
  }

  m_Contexts.emplace_back(Name, programID);

  return i;
}

void ContextManager::SetContext(const unsigned ContextID) noexcept
{
  if (ContextID != m_CurrentContextIndex)
  {
    if (m_CurrentContextIndex != Error::Context::INVALID_CONTEXT)
    {
      for (const auto& vertexAttribute : m_Contexts[m_CurrentContextIndex].VertexAttributes)
      {
        glDisableVertexAttribArray(vertexAttribute.ID);
      }
    }
    m_CurrentContextIndex = ContextID;
  }

  glUseProgram(m_Contexts[m_CurrentContextIndex].ProgramID);

  //TODO: Must bind vertex array before attribute pointers will have any effect

  for (const auto& vertexAttribute : m_Contexts[m_CurrentContextIndex].VertexAttributes)
  {
    //glVertexAttribPointer(
    //  m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ID,
    //  m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ElementCount,
    //  m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ElementType,
    //  m_Contexts[m_CurrentContextIndex].VertexAttributes[i].bIsNormalized,
    //  m_Contexts[m_CurrentContextIndex].VertexAttributes[i].Stride,
    //  reinterpret_cast<GLvoid*>(m_Contexts[m_CurrentContextIndex].VertexAttributes[i].Offset));
    glEnableVertexAttribArray(vertexAttribute.ID);
  }

  //TODO: Do I want to return the current program or do I want
  // instead to have the context manager handle uniforms?
}

GLuint& ContextManager::GetProgram(const unsigned ContextID) noexcept
{
  return m_Contexts[ContextID].ProgramID;
}

GLuint ContextManager::GetCurrentProgram() const noexcept
{
  return m_Contexts[m_CurrentContextIndex].ProgramID;
}

const vector<ContextManager::UniformAttribute>& ContextManager::GetCurrentUniformAttributes() const noexcept
{
  return m_Contexts[m_CurrentContextIndex].UniformAttributes;
}

const vector<ContextManager::VertexAttribute>& ContextManager::GetCurrentVertexAttributes() const noexcept
{
  return m_Contexts[m_CurrentContextIndex].VertexAttributes;
}

void ContextManager::AddNewUniformAttribute(const unsigned ContextID, const string& Name)
{
  assert(ContextID < m_Contexts.size());
  glUseProgram(m_Contexts[ContextID].ProgramID);
  const GLint id = glGetUniformLocation(m_Contexts[ContextID].ProgramID, Name.c_str());
  const UniformAttribute attribute = { Name, id };
  m_Contexts[ContextID].UniformAttributes.push_back(attribute);
  Log::trace("New Uniform Attribute \"" + Name + "\" added to Context: " + m_Contexts[ContextID].Name);
  glUseProgram(0u);
}

void ContextManager::AddNewVertexAttribute(const unsigned ContextID, const VertexAttribute& Attribute)
{
  assert(ContextID < m_Contexts.size());
  VertexAttribute va = Attribute;
  va.ID = glGetAttribLocation(m_Contexts[ContextID].ProgramID, va.Name.c_str());
  m_Contexts[ContextID].VertexAttributes.push_back(va);
  Log::trace("New Vertex Attribute \"" + va.Name + "\" added to Context: " + m_Contexts[ContextID].Name);
}