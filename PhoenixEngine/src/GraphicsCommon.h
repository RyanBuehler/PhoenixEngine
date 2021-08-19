#pragma once

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
    BASIC = 0u,
    COUNT
  };

  enum class Fragment : unsigned
  {
    BASIC = 0u,
    COUNT
  };
}