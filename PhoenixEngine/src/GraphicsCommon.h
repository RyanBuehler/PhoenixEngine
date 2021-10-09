#pragma once
#include "glm/glm.hpp"

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
    DIFFUSE = 0u,
    DEBUG,
    COUNT
  };

  enum class Fragment : unsigned
  {
    DIFFUSE = 0u,
    DEBUG,
    COUNT
  };
}

namespace Normals
{
  enum class Type : unsigned
  {
    VERTEX,
    TRIANGLE,
    SURFACE,
    COUNT
  };
}