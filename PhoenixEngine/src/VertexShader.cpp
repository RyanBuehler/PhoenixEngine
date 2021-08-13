#include "pch.h"
#include "GLEW/glew.h"
#include "Shader.h"
#include "VertexShader.h"

VertexShader::VertexShader(const string& fileName) noexcept :
  Shader(GL_VERTEX_SHADER, fileName)
{
}