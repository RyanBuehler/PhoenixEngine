#include "pch.h"
#include "ContextManager.h"

//TODO:
#define GL_PROGRAM_ERROR INT_MAX

ContextManager::ContextManager() noexcept :
  m_Contexts(),
  m_CurrentContextIndex(CONTEXT_ERROR)
{
}

ContextManager::~ContextManager()
{
  for (Context c : m_Contexts)
  {
    glDeleteProgram(c.ProgramID);
  }
}

unsigned ContextManager::CreateNewContext(const string& name, GLint vertexShaderID, GLint fragmentShaderID)
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

  m_Contexts.push_back({ name, programID });

  return i;
}

void ContextManager::SetContext(unsigned contextIndex) noexcept
{
  if (contextIndex != m_CurrentContextIndex)
  {
    if (m_CurrentContextIndex != CONTEXT_ERROR)
    {
      for (size_t i = 0; i < m_Contexts[m_CurrentContextIndex].VertexAttributes.size(); ++i)
      {
        glDisableVertexAttribArray(m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ID);
      }
    }
    m_CurrentContextIndex = contextIndex;
  }

  glUseProgram(m_Contexts[m_CurrentContextIndex].ProgramID);

  for (size_t i = 0; i < m_Contexts[m_CurrentContextIndex].VertexAttributes.size(); ++i)
  {
    glVertexAttribPointer(
      m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ID,
      m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ElementCount,
      m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ElementType,
      m_Contexts[m_CurrentContextIndex].VertexAttributes[i].bIsNormalized,
      m_Contexts[m_CurrentContextIndex].VertexAttributes[i].Stride,
      reinterpret_cast<GLvoid*>(m_Contexts[m_CurrentContextIndex].VertexAttributes[i].Offset));
    glEnableVertexAttribArray(m_Contexts[m_CurrentContextIndex].VertexAttributes[i].ID);
  }

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

const vector<ContextManager::VertexAttribute>& ContextManager::GetCurrentVertexAttributes() const noexcept
{
  return m_Contexts[m_CurrentContextIndex].VertexAttributes;
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
  glUseProgram(m_Contexts[contextIndex].ProgramID);
  GLint id = glGetUniformLocation(m_Contexts[contextIndex].ProgramID, name.c_str());
  UniformAttribute attribute = { name, id };
  m_Contexts[contextIndex].UniformAttributes.push_back(attribute);
  Log::Trace("New Uniform Attribute \"" + name + "\" added to Context: " + m_Contexts[contextIndex].Name);
  glUseProgram(0u);
}

void ContextManager::AddNewVertexAttribute(unsigned contextIndex, const VertexAttribute& vertexAttribute)
{
  assert(contextIndex < m_Contexts.size());
  VertexAttribute va = vertexAttribute;
  va.ID = glGetAttribLocation(m_Contexts[contextIndex].ProgramID, va.Name.c_str());
  m_Contexts[contextIndex].VertexAttributes.push_back(va);
  Log::Trace("New Vertex Attribute \"" + va.Name + "\" added to Context: " + m_Contexts[contextIndex].Name);
}
