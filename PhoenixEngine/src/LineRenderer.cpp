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
  m_LineColor(vec4(1.f, 1.f, 1.f, 1.f)),
  m_VertexArrayID(numeric_limits<unsigned>::max()),
  m_VertexBufferObject(numeric_limits<unsigned>::max()),
  m_PositionAttributeID(numeric_limits<unsigned>::max())
{
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);
	
	glGenBuffers(1, &m_VertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3), m_LineArray.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(m_PositionAttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(m_PositionAttributeID);

	glBindVertexArray(0u);
}

void LineRenderer::RenderLines() noexcept
{
  glBindVertexArray(m_VertexArrayID);
  glDrawArrays(GL_LINES, 0, 2);// static_cast<unsigned>(m_LineArray.size()));
  glBindVertexArray(0);
  m_LineArray.clear();
}

void LineRenderer::AddLine(const vec3& point1, const vec3& point2) noexcept
{
  m_LineArray.push_back(point1);
  m_LineArray.push_back(point2);
}

void LineRenderer::SetLineWidth(float width) noexcept
{
  glLineWidth(width);
}

void LineRenderer::SetLineColor(const vec4& rgba) noexcept
{
  m_LineColor = rgba;
}