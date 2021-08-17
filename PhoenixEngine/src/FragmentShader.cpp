#include "pch.h"
#include "FragmentShader.h"

FragmentShader::FragmentShader(const string& fileName) noexcept :
  Shader(Shader::Type::FRAGMENT, fileName)
{
}