#pragma once
//------------------------------------------------------------------------------
// File:    Texture.h
// Author:  Ryan Buehler
// Created: November 8th, 2021
// Desc:    Texture data formatted for use with an OpenGL rendering pipeline
//------------------------------------------------------------------------------
#include "GraphicsCommon.h"

class Texture
{
public:
  enum class Format : unsigned
  {
    PNG = 0u,
    COUNT
  };

  static const unsigned TEXTURE_ERROR = numeric_limits<unsigned>::max();

public:
  Texture(const string& name) noexcept;
  ~Texture() = default;
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&&) = delete;
  Texture& operator=(Texture&&) = delete;

  GLuint LoadTextureFromFile(const string& file) noexcept;

private:
  string m_Name;
  GLuint m_TextureID;

  int m_Width;
  int m_Height;
  int m_ChannelCount;
};
