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

  static constexpr unsigned TEXTURE_ERROR = numeric_limits<unsigned>::max();

  Texture(const string& Name) noexcept;
  ~Texture() = default;
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&&) = delete;
  Texture& operator=(Texture&&) = delete;

  bool LoadTextureFromFile(const string& FileName) noexcept;

  [[nodiscard]] GLuint GetTextureID() const noexcept;

  [[nodiscard]] int GetWidth() const noexcept;
  [[nodiscard]] int GetHeight() const noexcept;

private:
  string m_Name;
  GLuint m_TextureID;

  int m_Width;
  int m_Height;
  int m_Channels;
};
