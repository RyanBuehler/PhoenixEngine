#include "pch.h"
#include "VertexShader.h"

VertexShader::VertexShader(const string& fileName) noexcept :
  Shader(Shader::Type::VERTEX, fileName)
{}