//------------------------------------------------------------------------------
// File:    LineRenderer.h
// Author:  Ryan Buehler
// Created: 10/02/21
// Desc:    Responsible for drawing lines on the screen
//------------------------------------------------------------------------------
#pragma once
#include "GLEW/glew.h"
#include "GraphicsCommon.h"
#include "ShaderManager.h"
#include "ContextManager.h"

class LineRenderer
{
public:

  LineRenderer() noexcept;
  ~LineRenderer() = default;
  LineRenderer(const LineRenderer&) = delete;
  LineRenderer& operator=(const LineRenderer&) = delete;
  LineRenderer(LineRenderer&&) = delete;
  LineRenderer& operator=(LineRenderer&&) = delete;

  void RenderLines() noexcept;

  void AddLine(const vec3& point1, const vec3& point2) noexcept;

  void SetLineWidth(float width) noexcept;
  void SetLineColor(const vec4& rgba) noexcept;

private:
  vector<vec3> m_LineArray;

  GLint m_ContextID;

  GLfloat m_LineWidth;
  vec4 m_LineColor;
  GLuint m_VertexArray;
  GLuint m_VertexAttributeID;
};
