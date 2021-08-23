#pragma once
#include "GraphicsCommon.h"

class ContextManager
{
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

  GLint SwapContext(unsigned contextID) noexcept;

private:
  void RetrieveProgramLog(GLint programID, string& log) const noexcept;

  vector<GLint> m_Programs;

  GLint m_CurrentProgram;
};
