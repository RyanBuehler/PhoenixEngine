//------------------------------------------------------------------------------
// File:    LineRenderer.h
// Author:  Ryan Buehler
// Created: 10/02/21
// Desc:    Responsible for drawing lines on the screen
//------------------------------------------------------------------------------
#include "pch.h"
#include "LineRenderer.h"

LineRenderer::LineRenderer() noexcept :
  m_LineArray(),
  m_ContextID(numeric_limits<unsigned>::max()),
  m_LineWidth(1.f),
  m_LineColor(vec4(1.f, 1.f, 1.f, 1.f)),
  m_VertexArray(numeric_limits<unsigned>::max()),
  m_VertexAttributeID(numeric_limits<unsigned>::max())
{}

void LineRenderer::Init(const ShaderManager& shaderManager, ContextManager& contextManager) noexcept
{
  unsigned vID = shaderManager.GetVertexShaderID(Shader::Vertex::LINE);
  unsigned fID = shaderManager.GetFragmentShaderID(Shader::Fragment::LINE);
  m_ContextID = contextManager.CreateNewContext(vID, fID);
  GLint program = contextManager.SwapContext(m_ContextID);

  //TODO: Move this to constructor
  glGenBuffers(1, &m_VertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexArray);
  glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), m_LineArray.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(m_VertexAttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);

  m_LineArray.push_back(vec3(0.f, 0.f, 0.f));
  m_LineArray.push_back(vec3(2.f, 0.f, 0.f));
}

void LineRenderer::RenderLines() const noexcept
{
  glLineWidth(m_LineWidth);
  glBindVertexArray(m_VertexArray);
  glDrawArrays(GL_LINES, 0, static_cast<unsigned>(m_LineArray.size()));
}

void LineRenderer::SetLineWidth(float width) noexcept
{
  m_LineWidth = width;
}

void LineRenderer::SetLineColor(const vec4& rgba) noexcept
{
  m_LineColor = rgba;
}
