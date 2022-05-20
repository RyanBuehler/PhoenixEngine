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
  EnvironmentMap() noexcept;
  ~EnvironmentMap();
  EnvironmentMap(const EnvironmentMap&) = delete;
  EnvironmentMap& operator=(const EnvironmentMap&) = delete;
  EnvironmentMap(EnvironmentMap&&) = delete;
  EnvironmentMap& operator=(EnvironmentMap&&) = delete;

  [[nodiscard]] GLuint GetFBOHandle(int i) const noexcept;
  [[nodiscard]] GLuint GetTextureHandle(GLuint ID) const noexcept;
  Camera& GetCamera() noexcept;
  void Bind(int Index) noexcept;
  void EnableTextures() noexcept;

private:
  GLuint m_hFBO[6];
  GLuint m_hTexture[6];
  GLuint m_hRBO[6];

  Camera m_Camera;
};
