//------------------------------------------------------------------------------
// File:    LineRenderer.h
// Author:  Ryan Buehler
// Created: 10/02/21
// Desc:    Responsible for drawing lines on the screen
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

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
    static auto instance = DebugRenderer();
    return instance;
  }

  ~DebugRenderer();
  DebugRenderer(const DebugRenderer&) = delete;
  DebugRenderer& operator=(const DebugRenderer&) = delete;
  DebugRenderer(DebugRenderer&&) = delete;
  DebugRenderer& operator=(DebugRenderer&&) = delete;

  void RenderLines() noexcept;
  void RenderPermanentLines() const noexcept;

  void AddLine(const vec3& Point1, const vec3& Point2) noexcept;
  void AddLine(const vec3& Point1, const vec4& Color1, const vec3& Point2, const vec4& Color2) noexcept;
  void AddPermanentLine(const vec3& Point1, const vec3& Point2) noexcept;
  void AddPermanentLine(const vec3& Point1, const vec4& Color1, const vec3& Point2, const vec4& Color2) noexcept;

  static void SetLineWidth(float Width) noexcept;
  void SetDefaultLineColor(const vec4& Rgba) noexcept;

private:
  DebugRenderer() noexcept;

  void RenderLine(const vec3& Point1, const vec4& Color1, const vec3& Point2, const vec4& Color2) const;

  vector<vec3> m_LineArray;
  vector<vec3> m_PermanentLineArray;
  vector<vec4> m_ColorArray;
  vector<vec4> m_PermanentColorArray;

  vec4 m_DefaultLineColor;

  GLuint m_hVertexArray;
  GLuint m_hVertexBufferObject;
};
