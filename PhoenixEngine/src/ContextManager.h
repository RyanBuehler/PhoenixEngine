#pragma once
#include "GraphicsCommon.h"
#include "GLEW/glew.h"

class ContextManager
{
public:
  constexpr static unsigned CONTEXT_ERROR = numeric_limits<unsigned>::max();

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
      ID(CONTEXT_ERROR),
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
    Context(const string& name, GLint programID = numeric_limits<int>::max()) :
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

  unsigned CreateNewContext(
    const string& name,
    GLint vertexShaderID,
    GLint fragmentShaderID
  );

  void SetContext(unsigned contextID) noexcept;

  GLuint GetCurrentProgram() const noexcept;
  const vector<UniformAttribute>& GetCurrentUniformAttributes() const noexcept;
  const vector<VertexAttribute>& GetCurrentVertexAttributes() const noexcept;

  void AddNewUniformAttribute(unsigned contextIndex, const string& name);
  void AddNewVertexAttribute(unsigned contextIndex, const VertexAttribute& vertexAttribute);

private:
  void RetrieveProgramLog(GLint programID, string& log) const noexcept;

  vector<Context> m_Contexts;

  unsigned m_CurrentContextIndex;
};
