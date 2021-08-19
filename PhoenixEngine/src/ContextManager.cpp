#include "pch.h"
#include "GLEW/glew.h"
#include "ContextManager.h"
#include "ShaderManager.h"

//TODO:
#define GL_PROGRAM_ERROR INT_MAX

ContextManager::ContextManager() noexcept :
  m_Programs(),
  m_CurrentProgram(numeric_limits<unsigned>::max())
{
}

ContextManager::~ContextManager()
{
  for (GLint id : m_Programs)
  {
    glDeleteProgram(id);
  }
}

unsigned ContextManager::CreateNewContext(GLint vertexShaderID, GLint fragmentShaderID)
{
  unsigned i = static_cast<unsigned>(m_Programs.size());

  GLint programID = glCreateProgram();

  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);

  glLinkProgram(programID);

  GLint result;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (!result)
  {
    Log::Error("Error linking OpenGL program.");
    string error;
    RetrieveProgramLog(programID, error);
    Log::Error(error);
    return GL_PROGRAM_ERROR;
  }

  m_Programs.push_back(result);

  return i;
}

void ContextManager::SwapContext(unsigned contextID) noexcept
{
  if (m_Programs[contextID] == m_CurrentProgram)
    return;

  m_CurrentProgram = m_Programs[contextID];

  glUseProgram(m_CurrentProgram);
}

void ContextManager::RetrieveProgramLog(GLint programID, string& log) const noexcept
{
  int logLength;
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
  char* programLog = new char[logLength];
  glGetProgramInfoLog(programID, logLength, &logLength, programLog);
  log.clear();
  log = programLog;
  delete[] programLog;
}
