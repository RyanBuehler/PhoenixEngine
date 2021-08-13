#pragma once

class VertexShader : public Shader
{
public:

  VertexShader(const string& filePath) noexcept;
  ~VertexShader() = default;
  VertexShader(const VertexShader&) = delete;
  VertexShader& operator=(const VertexShader&) = delete;
  VertexShader(VertexShader&&) = delete;
  VertexShader& operator=(VertexShader&&) = delete;

private:

};

