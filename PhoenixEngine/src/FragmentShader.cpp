#include "pch.h"
#include "GLEW/glew.h"
#include "Shader.h"
#include "FragmentShader.h"

FragmentShader::FragmentShader(const string& fileName) noexcept :
  Shader(GL_FRAGMENT_SHADER, fileName)
{
}