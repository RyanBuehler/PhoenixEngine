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
  m_LineWidth(5.f),
  m_LineColor(vec4(1.f, 1.f, 1.f, 1.f)),
  m_VertexArray(numeric_limits<unsigned>::max()),
  m_VertexAttributeID(numeric_limits<unsigned>::max())
{
  glGenBuffers(1, &m_VertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexArray);
  glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), m_LineArray.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(m_VertexAttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void LineRenderer::RenderLines() noexcept
{
  glLineWidth(m_LineWidth);
  glBindVertexArray(m_VertexArray);
  glDrawArrays(GL_LINES, 0, static_cast<unsigned>(m_LineArray.size()));
  m_LineArray.clear();
}

void LineRenderer::AddLine(const vec3& point1, const vec3& point2) noexcept
{
  m_LineArray.push_back(point1);
  m_LineArray.push_back(point2);
}

void LineRenderer::SetLineWidth(float width) noexcept
{
  m_LineWidth = width;
}

void LineRenderer::SetLineColor(const vec4& rgba) noexcept
{
  m_LineColor = rgba;
}
