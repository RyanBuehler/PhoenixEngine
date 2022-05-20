#pragma once

namespace Utility
{
  template<typename T>
  constexpr size_t ID(T Value) { return static_cast<size_t>(Value); }
}