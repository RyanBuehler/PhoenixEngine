#pragma once

namespace Utility
{
  template<typename T>
  constexpr inline size_t ID(T value) { return static_cast<size_t>(value); }
}