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
#include "Camera.h"

class DebugRenderer
{
public:
  struct VertexBufferObject
  {
    vec4 VertexPosition;
    vec4 VertexColor;
  };

public:
  /// <summary>
  /// Singleton Pattern Instance
  /// </summary>
  /// <returns>The DebugRenderer</returns>
  static DebugRenderer& I()
  {
    static DebugRenderer m_Instance = DebugRenderer();
    return m_Instance;
  }

  ~DebugRenderer();
  DebugRenderer(const DebugRenderer&) = delete;
  DebugRenderer& operator=(const DebugRenderer&) = delete;
  DebugRenderer(DebugRenderer&&) = delete;
  DebugRenderer& operator=(DebugRenderer&&) = delete;

  void RenderLines() noexcept;

  void AddLine(const vec3& point1, const vec3& point2) noexcept;
  void AddPermanentLine(const vec3& point1, const vec3& point2) noexcept;

  void SetLineWidth(float width) noexcept;
  void SetLineColor(const vec4& rgba) noexcept;

private:
  DebugRenderer() noexcept;

  void RenderLine(const vec3& point1, const vec3& point2);

  vector<vec3> m_LineArray;
  vector<vec3> m_PermanentLineArray;

  vec4 m_LineColor;

  GLuint m_VertexArrayID;
  GLuint m_VertexBufferObject;

  GLuint m_PositionAttributeID;
};
