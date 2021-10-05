#pragma once
#include "GraphicsCommon.h"
#include "GLEW/glew.h"

class ContextManager
{
public:
  struct UniformAttribute
  {
    GLint ID;
    string Name;
  };

  struct Context
  {
    Context(GLint programID = numeric_limits<int>::max()) :
      ProgramID(programID),
      UniformAttributes() {}

    GLuint ProgramID;
    vector<UniformAttribute> UniformAttributes;
  };

public:
  ContextManager() noexcept;
  ~ContextManager();
  ContextManager(const ContextManager&) = delete;
  ContextManager& operator=(const ContextManager&) = delete;
  ContextManager(ContextManager&&) = delete;
  ContextManager& operator=(ContextManager&&) = delete;

  unsigned CreateNewContext(
    GLint vertexShaderID,
    GLint fragmentShaderID
  );

  void SetContext(unsigned contextID) noexcept;

  GLuint GetCurrentProgram() const noexcept;
  const vector<UniformAttribute>& GetCurrentUniformAttributes() const noexcept;

  void AddNewUniformAttribute(unsigned contextIndex, const string& name);

private:
  void RetrieveProgramLog(GLint programID, string& log) const noexcept;

  vector<Context> m_Contexts;

  unsigned m_CurrentContextIndex;
};
