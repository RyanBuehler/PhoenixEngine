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
	for (size_t i = 0; i < m_LineArray.size(); i += 2)
	{
		drawLine(m_LineArray[i], m_LineArray[i + 1], vec3(1.f, 0.f, 0.f));
	}
  //glBindVertexArray(m_VertexArrayID);
  //glDrawArrays(GL_LINES, 0, 2);// static_cast<unsigned>(m_LineArray.size()));
  //glBindVertexArray(0);
  m_LineArray.clear();
}

void LineRenderer::AddLine(const vec3& point1, const vec3& point2) noexcept
{
  m_LineArray.push_back(point1);
  m_LineArray.push_back(point2);
}

void LineRenderer::drawLine(const vec3& from, const vec3& to, const vec3& color)
{
	// Vertex data
	GLfloat points[12];

	points[0] = from.x;
	points[1] = from.y;
	points[2] = from.z;
	points[3] = color.x;
	points[4] = color.y;
	points[5] = color.z;

	points[6] = to.x;
	points[7] = to.y;
	points[8] = to.z;
	points[9] = color.x;
	points[10] = color.y;
	points[11] = color.z;

	glDeleteBuffers(1, &m_VertexBufferObject);
	glDeleteVertexArrays(1, &m_VertexArrayID);
	glGenBuffers(1, &m_VertexBufferObject);
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	glBindVertexArray(m_VertexArrayID);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

}

void LineRenderer::SetLineWidth(float width) noexcept
{
  glLineWidth(width);
}

void LineRenderer::SetLineColor(const vec4& rgba) noexcept
{
  m_LineColor = rgba;
}