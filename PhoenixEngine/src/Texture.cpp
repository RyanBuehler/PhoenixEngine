//------------------------------------------------------------------------------
// File:    Texture.h
// Author:  Ryan Buehler
// Created: November 8th, 2021
// Desc:    Texture data formatted for use with an OpenGL rendering pipeline
//------------------------------------------------------------------------------
#include "pch.h"
#include "Texture.h"
#include "PNGReader.h"

Texture::Texture(const string& name) noexcept :
  m_Name(name),
  m_TextureID(numeric_limits<unsigned>::max()),
  m_Width(-1),
  m_Height(-1),
  m_Channels(-1)
{
  Log::Trace("Texture Created: " + name);
}

bool Texture::LoadTextureFromFile(const string& filename) noexcept
{
  PNGReader reader;

  if (!reader.LoadPNG(filename))
  {
    Log::Error("[Texture.cpp] Couldn't load texture: " + filename);
    return false;
  }

  PNGReader::ImageData image = reader.GetImageData();

  m_Width = image.Width;
  m_Height = image.Height;
  m_Channels = image.Channels;

  glGenTextures(1, &m_TextureID);
  glBindTexture(GL_TEXTURE_2D, m_TextureID);

  // Set up the UV wrapping and MIP mapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.Data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Disable Texture state
  glBindTexture(GL_TEXTURE_2D, 0u);

  return true;
}

GLuint Texture::GetTextureID() const noexcept
{
  return m_TextureID;
}

int Texture::GetWidth() const noexcept
{
  return m_Width;
}

int Texture::GetHeight() const noexcept
{
  return m_Height;
}
