//------------------------------------------------------------------------------
// File:    EnvironmentMap
// Author:  Ryan Buehler
// Created: November 30, 2021
// Desc:    Captures a cube map around a location in the scene
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"
#include "Camera.h"

class EnvironmentMap
{
public:
  const GLenum DrawBuffers[6] =
  {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
  };

public:
  EnvironmentMap() noexcept;
  ~EnvironmentMap();
  EnvironmentMap(const EnvironmentMap&) = delete;
  EnvironmentMap& operator=(const EnvironmentMap&) = delete;
  EnvironmentMap(EnvironmentMap&&) = delete;
  EnvironmentMap& operator=(EnvironmentMap&&) = delete;

  GLuint GetFBOHandle(int i) const noexcept;
  GLuint GetTextureHandle(GLuint index) const noexcept;
  Camera& GetCamera() noexcept;
  void Bind(int i) noexcept;

private:
  //TODO: Rename all OpenGL handles to hVariable
  GLuint m_hFBO[6];
  GLuint m_hTexture[6];
  GLuint m_hRBO[6];

  Camera m_Camera;
};
