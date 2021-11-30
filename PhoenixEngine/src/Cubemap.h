//------------------------------------------------------------------------------
// File:    Cubemap.h
// Author:  Ryan Buehler
// Created: November 30, 2021
// Desc:    A Cubemap of 6 textures for use with skybox, reflection/refraction
//------------------------------------------------------------------------------
#pragma once
#include "GraphicsCommon.h"

class Cubemap
{
public:
  Cubemap(const char* filenames[6]) noexcept;
  ~Cubemap() = default;
  Cubemap(const Cubemap&) = delete;
  Cubemap& operator=(const Cubemap&) = delete;
  Cubemap(Cubemap&&) = delete;
  Cubemap& operator=(Cubemap&&) = delete;

  GLuint GetID();

  // Front  = +Z 
  // Back   = -Z
  // Right  = +X 
  // Left   = -X
  // Top    = +Y
  // Bottom = -Y
private:
  GLuint m_CubemapID;
};
