//------------------------------------------------------------------------------
// File:    LineRenderer.h
// Author:  Ryan Buehler
// Created: 10/02/21
// Desc:    Responsible for drawing lines on the screen
//------------------------------------------------------------------------------
#include "pch.h"
#include "DebugRenderer.h"
#include "Colors.h"

DebugRenderer::DebugRenderer() noexcept :
  m_LineArray(),
  m_DefaultLineColor(Colors::PURPLE),
  m_hVertexArray(Error::INVALID_INDEX),
  m_hVertexBufferObject(Error::INVALID_INDEX),
  m_hPositionAttribute(Error::INVALID_INDEX)
{
  glGenVertexArrays(1, &m_hVertexArray);
  glBindVertexArray(m_hVertexArray);

  glGenBuffers(1, &m_hVertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec4), m_LineArray.data(), GL_STATIC_DRAW);

  glBindVertexArray(0u);
}

DebugRenderer::~DebugRenderer()
{
  glDeleteBuffers(1, &m_hVertexBufferObject);
  glDeleteVertexArrays(1, &m_hVertexArray);
}

void DebugRenderer::RenderLine(const vec3& point1, const vec4& color1, const vec3& point2, const vec4& color2)
{
  VertexBufferObject line[2] = { {vec4(point1, 1.f), color1 }, {vec4(point2, 1.f), color2} };

  glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(line), &line, GL_STATIC_DRAW);

  glDrawArrays(GL_LINES, 0, 2);
}

void DebugRenderer::RenderLines() noexcept
{
  //TODO: Move these into the context manager
  glBindVertexArray(m_hVertexArray);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBufferObject), 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBufferObject), (GLvoid*)sizeof(vec4));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  for (size_t i = 0; i < m_LineArray.size(); i += 2)
  {
    RenderLine(m_LineArray[i], m_ColorArray[i], m_LineArray[i + 1], m_ColorArray[i + 1]);
  }

  m_LineArray.clear();
  m_ColorArray.clear();

  glBindVertexArray(0);
}

void DebugRenderer::RenderPermanentLines() noexcept
{
  //TODO: Move these into the context manager
  glBindVertexArray(m_hVertexArray);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBufferObject), 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBufferObject), (GLvoid*)sizeof(vec4));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  for (size_t i = 0; i < m_PermanentLineArray.size(); i += 2)
  {
    RenderLine(m_PermanentLineArray[i], m_PermanentColorArray[i],
      m_PermanentLineArray[i + 1], m_PermanentColorArray[i + 1]);
  }
}

void DebugRenderer::AddLine(const vec3& point1, const vec3& point2) noexcept
{
  AddLine(point1, m_DefaultLineColor, point2, m_DefaultLineColor);
}

void DebugRenderer::AddLine(const vec3& point1, const vec4& color1, const vec3& point2, const vec4& color2) noexcept
{
  m_LineArray.push_back(point1);
  m_LineArray.push_back(point2);
  m_ColorArray.push_back(color1);
  m_ColorArray.push_back(color2);
}

void DebugRenderer::AddPermanentLine(const vec3& point1, const vec3& point2) noexcept
{
  AddPermanentLine(point1, m_DefaultLineColor, point2, m_DefaultLineColor);
}

void DebugRenderer::AddPermanentLine(const vec3& point1, const vec4& color1, const vec3& point2, const vec4& color2) noexcept
{
  m_PermanentLineArray.push_back(point1);
  m_PermanentLineArray.push_back(point2);
  m_PermanentColorArray.push_back(color1);
  m_PermanentColorArray.push_back(color2);
}

void DebugRenderer::SetLineWidth(float width) noexcept
{
  glLineWidth(width);
}

void DebugRenderer::SetDefaultLineColor(const vec4& rgba) noexcept
{
  m_DefaultLineColor = rgba;
}