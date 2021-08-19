#pragma once
#include "IBindable.h"

namespace Render
{
  enum class Context : unsigned
  {
    UNKNOWN = 0u,
    BASIC,
    COUNT
  };
}

class IRenderContext : public IBindable
{
public:
  IRenderContext(
    Shader::Vertex vertexShader = Shader::Vertex::NONE,
    Shader::Fragment fragmentShader = Shader::Fragment::NONE
  ) noexcept :
    m_VertexShader(vertexShader),
    m_FragmentShader(fragmentShader),
    m_Program(glCreateProgram())
  {
    VertexShader vs("basic.vert");
    FragmentShader fs("basic.frag");

    glAttachShader(m_Program, vs.GetShaderID());
    glAttachShader(m_Program, fs.GetShaderID());

    //TODO:
    //glDeleteShader(vs.GetShaderID());
    //glDeleteShader(fs.GetShaderID());

    glLinkProgram(m_Program);
    GLint result;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
    if (!result)
    {
      Log::Error(string("Error linking OpenGL program : ") + GetContextName());
      string error;
      RetrieveProgramLog(error);
      Log::Error(error);
      return;
    }
  };
  virtual ~IRenderContext() = default;

  virtual Render::Context GetContext() const noexcept = 0;
  virtual const char* GetContextName() const noexcept = 0;

  void RetrieveProgramLog(string& log) const noexcept
  {
    int logLength;
    glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &logLength);
    char* programLog = new char[logLength];
    glGetProgramInfoLog(m_Program, logLength, &logLength, programLog);
    log.clear();
    log = programLog;
    delete[] programLog;
  }

protected:
  Shader::Vertex m_VertexShader;
  Shader::Fragment m_FragmentShader;

  GLint m_Program;
};
