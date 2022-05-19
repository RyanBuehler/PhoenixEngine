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
      string name,
      GLint elementCount,
      GLenum elementType,
      GLboolean isNormalized,
      GLsizei stride,
      size_t offset
    ) :
      Name(name),
      ID(Error::Context::INVALID_CONTEXT),
      ElementCount(elementCount),
      ElementType(elementType),
      bIsNormalized(isNormalized),
      Stride(stride),
      Offset(offset)
    {};

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
    Context(const string& name, GLint programID = Error::INVALID_INDEX) :
      Name(name),
      ProgramID(programID),
      UniformAttributes(),
      VertexAttributes()
    {}

    string Name;
    GLuint ProgramID;
    vector<ContextManager::UniformAttribute> UniformAttributes;
    vector<ContextManager::VertexAttribute> VertexAttributes;
  };

public:
  ContextManager() noexcept;
  ~ContextManager();
  ContextManager(const ContextManager&) = delete;
  ContextManager& operator=(const ContextManager&) = delete;
  ContextManager(ContextManager&&) = delete;
  ContextManager& operator=(ContextManager&&) = delete;

  unsigned CreateNewContext(const string& name, GLint vertexShaderID, GLint fragmentShaderID);

  void SetContext(unsigned contextID) noexcept;

  GLuint& GetProgram(unsigned contextID) noexcept;
  GLuint GetCurrentProgram() const noexcept;
  const vector<UniformAttribute>& GetCurrentUniformAttributes() const noexcept;
  const vector<VertexAttribute>& GetCurrentVertexAttributes() const noexcept;

  void AddNewUniformAttribute(unsigned contextIndex, const string& name);
  void AddNewVertexAttribute(unsigned contextIndex, const VertexAttribute& vertexAttribute);

  /*bool ReloadContext(GLuint programID, GLint vertexShaderID, GLint fragmentShaderID) noexcept;*/

private:
  vector<Context> m_Contexts;

  unsigned m_CurrentContextIndex;
};
