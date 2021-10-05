//------------------------------------------------------------------------------
// File:    LineRenderer.h
// Author:  Ryan Buehler
// Created: 10/02/21
// Desc:    Responsible for drawing lines on the screen
//------------------------------------------------------------------------------
#pragma once
#include "GLEW/glew.h"
#include "GraphicsCommon.h"

class LineRenderer
{
public:

  LineRenderer() noexcept;
  ~LineRenderer() = default;
  LineRenderer(const LineRenderer&) = delete;
  LineRenderer& operator=(const LineRenderer&) = delete;
  LineRenderer(LineRenderer&&) = delete;
  LineRenderer& operator=(LineRenderer&&) = delete;

  void Init() noexcept;

  void RenderLines() const noexcept;

  void SetLineWidth(float width) noexcept;
  void SetLineColor(const vec4& rgba) noexcept;

private:
  vector<vec3> m_LineArray;

  GLfloat m_LineWidth;
  vec4 m_LineColor;
  GLuint m_VertexArray;
  GLuint m_VertexAttributeID;
};
