#pragma once
#include "Shader.h"

class VertexShader : public Shader
{
public:

  VertexShader(const string& filePath) noexcept;
  ~VertexShader() = default;
  VertexShader(const VertexShader&) = delete;
  VertexShader& operator=(const VertexShader&) = delete;
  VertexShader(VertexShader&&) = delete;
  VertexShader& operator=(VertexShader&&) = delete;

  void Bind() noexcept {};

private:
};

