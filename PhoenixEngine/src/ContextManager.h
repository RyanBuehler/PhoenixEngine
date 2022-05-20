//------------------------------------------------------------------------------
// File:    ContextManager.h
// Author:  Ryan Buehler
// Created: September 27, 2021
// Desc:    Manages the Contexts of the Rendering Engine
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

class ContextManager
{
public:
  struct UniformAttribute
  {
    string Name;
    GLint ID;
  };

  struct VertexAttribute
  {
    VertexAttribute(
      string Name,
      const GLint ElementCount,
      const GLenum ElementType,
      const GLboolean IsNormalized,
      const GLsizei Stride,
      const size_t Offset
    ) :
      Name(std::move(Name)),
      ID(Error::Context::INVALID_CONTEXT),
      ElementCount(ElementCount),
      ElementType(ElementType),
      bIsNormalized(IsNormalized),
      Stride(Stride),
      Offset(Offset)
    {}

    string Name;
    GLuint ID;
    GLint ElementCount;
    GLenum ElementType;
    GLboolean bIsNormalized;
    GLsizei Stride;
    size_t Offset;
  };

private:
  struct Context
  {
    explicit Context(string Name, const GLuint ProgramID = Error::INVALID_INDEX
    ) :
      Name(std::move(Name)),
      ProgramID(ProgramID)
    {}

    string Name;
    GLuint ProgramID;
    vector<UniformAttribute> UniformAttributes;
    vector<VertexAttribute> VertexAttributes;
  };

public:
  ContextManager() noexcept;
  ~ContextManager();
  ContextManager(const ContextManager&) = delete;
  ContextManager& operator=(const ContextManager&) = delete;
  ContextManager(ContextManager&&) = delete;
  ContextManager& operator=(ContextManager&&) = delete;

  unsigned CreateNewContext(const string& Name, GLint VertexShaderID, GLint FragmentShaderID);

  void SetContext(unsigned ContextID) noexcept;

  GLuint& GetProgram(unsigned ContextID) noexcept;
  [[nodiscard]] GLuint GetCurrentProgram() const noexcept;
  [[nodiscard]] const vector<UniformAttribute>& GetCurrentUniformAttributes() const noexcept;
  [[nodiscard]] const vector<VertexAttribute>& GetCurrentVertexAttributes() const noexcept;

  void AddNewUniformAttribute(unsigned ContextID, const string& Name);
  void AddNewVertexAttribute(unsigned ContextID, const VertexAttribute& Attribute);

private:
  vector<Context> m_Contexts;

  unsigned m_CurrentContextIndex;
};
