#pragma once
#include "Shader.h"

class FragmentShader : public Shader
{
public:

  FragmentShader(const string& filePath) noexcept;
  ~FragmentShader() noexcept = default;
  FragmentShader(const FragmentShader&) = delete;
  FragmentShader& operator=(const FragmentShader&) = delete;
  FragmentShader(FragmentShader&&) = delete;
  FragmentShader& operator=(FragmentShader&&) = delete;

  void Bind() noexcept {};

private:
};
