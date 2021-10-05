#include "pch.h"
#include "ContextManager.h"

//TODO:
#define GL_PROGRAM_ERROR INT_MAX

ContextManager::ContextManager() noexcept :
  m_Contexts(),
  m_CurrentContextIndex(numeric_limits<unsigned>::max())
{
}

ContextManager::~ContextManager()
{
  for (Context c : m_Contexts)
  {
    glDeleteProgram(c.ProgramID);
  }
}

unsigned ContextManager::CreateNewContext(GLint vertexShaderID, GLint fragmentShaderID)
{
  unsigned i = static_cast<unsigned>(m_Contexts.size());

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

  m_Contexts.push_back(programID);

  return i;
}

void ContextManager::SetContext(unsigned contextIndex) noexcept
{
  if (contextIndex != m_CurrentContextIndex)
  {
    m_CurrentContextIndex = contextIndex;
  }

  glUseProgram(m_Contexts[m_CurrentContextIndex].ProgramID);

  //TODO: Do I want to return the current program or do I want
  // instead to have the context manager handle uniforms?
}

GLuint ContextManager::GetCurrentProgram() const noexcept
{
  return m_Contexts[m_CurrentContextIndex].ProgramID;
}

const vector<ContextManager::UniformAttribute>& ContextManager::GetCurrentUniformAttributes() const noexcept
{
  return m_Contexts[m_CurrentContextIndex].UniformAttributes;
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

void ContextManager::AddNewUniformAttribute(unsigned contextIndex, const string& name)
{
  assert(contextIndex < m_Contexts.size());
  GLint id = glGetUniformLocation(m_Contexts[contextIndex].ProgramID, name.c_str());
  UniformAttribute attribute = { id, name };
  m_Contexts[contextIndex].UniformAttributes.push_back(attribute);
}
