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
  m_LineColor(Colors::RED),
  m_VertexArrayID(numeric_limits<unsigned>::max()),
  m_VertexBufferObject(numeric_limits<unsigned>::max()),
  m_PositionAttributeID(numeric_limits<unsigned>::max())
{
	//glGenVertexArrays(1, &m_VertexArrayID);
	//glBindVertexArray(m_VertexArrayID);
	//
	//glGenBuffers(1, &m_VertexBufferObject);
 // glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
 // glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec4), m_LineArray.data(), GL_STATIC_DRAW);

  //glVertexAttribPointer(m_PositionAttributeID, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(m_PositionAttributeID);

	glBindVertexArray(0u);
}

DebugRenderer::~DebugRenderer()
{
	glDeleteBuffers(1, &m_VertexBufferObject);
	glDeleteVertexArrays(1, &m_VertexArrayID);
}

void DebugRenderer::RenderLine(const vec3& point1, const vec3& point2)
{
	//VertexBufferObject line[2] = { {vec4(point1, 1.f), m_LineColor }, {vec4(point2, 1.f), m_LineColor} };

	//glBindVertexArray(m_VertexArrayID);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(line), &line, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBufferObject), 0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBufferObject), (GLvoid*)sizeof(vec4));
	//glEnableVertexAttribArray(1);
	//glBindVertexArray(0);

	//glBindVertexArray(m_VertexArrayID);
	//glDrawArrays(GL_LINES, 0, 2);
	//glBindVertexArray(0);

	//	// Vertex data
	//GLfloat points[12];
	//points[0] = point1.x;
	//points[1] = point1.y;
	//points[2] = point1.z;
	//points[3] = m_LineColor.x;
	//points[4] = m_LineColor.y;
	//points[5] = m_LineColor.z;
	//points[6] = point2.x;
	//points[7] = point2.y;
	//points[8] = point2.z;
	//points[9] = m_LineColor.x;
	//points[10] = m_LineColor.y;
	//points[11] = m_LineColor.z;
	//glDeleteBuffers(1, &m_VertexBufferObject);
	//glDeleteVertexArrays(1, &m_VertexArrayID);
	//glGenBuffers(1, &m_VertexBufferObject);
	//glGenVertexArrays(1, &m_VertexArrayID);
	//glBindVertexArray(m_VertexArrayID);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glBindVertexArray(0);

	//glBindVertexArray(m_VertexArrayID);
	//glDrawArrays(GL_LINES, 0, 2);
	//glBindVertexArray(0);
}

void DebugRenderer::RenderLines() noexcept
{
	for (size_t i = 0; i < m_LineArray.size(); i += 2)
	{
		RenderLine(m_LineArray[i], m_LineArray[i + 1]);
	}
  //glBindVertexArray(m_VertexArrayID);
  //glDrawArrays(GL_LINES, 0, 2);// static_cast<unsigned>(m_LineArray.size()));
  glBindVertexArray(0);
  m_LineArray.clear();
}

void DebugRenderer::AddLine(const vec3& point1, const vec3& point2) noexcept
{
  m_LineArray.push_back(point1);
  m_LineArray.push_back(point2);
}

void DebugRenderer::SetLineWidth(float width) noexcept
{
  glLineWidth(width);
}

void DebugRenderer::SetLineColor(const vec4& rgba) noexcept
{
  m_LineColor = rgba;
}