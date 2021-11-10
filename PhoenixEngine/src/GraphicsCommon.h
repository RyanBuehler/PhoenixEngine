#pragma once
#include "glm/glm.hpp"
#include "GLEW/glew.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Graphics
{
  enum class DataUsage
  {
    STATIC = 0,
    DYNAMIC,
    STREAM
  };

  static void RetrieveShaderLog(GLint shaderID, string& log) noexcept
  {
    int logLength;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
    char* shaderLog = new char[logLength];
    glGetShaderInfoLog(shaderID, logLength, &logLength, shaderLog);
    log.clear();
    log = shaderLog;
    delete[] shaderLog;
  }

  static void RetrieveProgramLog(GLint programID, string& log) noexcept
  {
    int logLength;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
    char* programLog = new char[logLength];
    glGetProgramInfoLog(programID, logLength, &logLength, programLog);
    log.clear();
    log = programLog;
    delete[] programLog;
  }
}

namespace Shader
{
  enum class Type : unsigned
  {
    UNKNOWN = 0u,
    VERTEX,
    FRAGMENT
  };

  enum class Vertex : unsigned
  {
    UNKNOWN = 0u,
    DIFFUSE,
    PHONGLIGHT,
    PHONGSHADE,
    DEBUG,
    COUNT
  };

  enum class Fragment : unsigned
  {
    UNKNOWN = 0u,
    DIFFUSE,
    PHONGLIGHT,
    PHONGSHADE,
    DEBUG,
    COUNT
  };
}

namespace Normals
{
  enum class Type : unsigned
  {
    VERTEX = 0u,
    TRIANGLE,
    SURFACE,
    COUNT
  };
}

namespace UV
{
  enum class Generation : unsigned
  {
    CUSTOM = 0u,
    SPHERICAL,
    CYLINDRICAL,
    PLANAR
  };
}