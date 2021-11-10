//------------------------------------------------------------------------------
// File:    Texture.h
// Author:  Ryan Buehler
// Created: November 8th, 2021
// Desc:    Texture data formatted for use with an OpenGL rendering pipeline
//------------------------------------------------------------------------------
#include "pch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const string& name) noexcept :
  m_Name(name),
  m_TextureID(numeric_limits<unsigned>::max()),
  m_Width(-1),
  m_Height(-1),
  m_ChannelCount(-1)
{
  Log::Trace("Texture Created: " + name);
}

GLuint Texture::LoadTextureFromFile(const string& file) noexcept
{
  glGenTextures(1, &m_TextureID);
  glBindTexture(GL_TEXTURE_2D, m_TextureID);

  // Load the data
  unsigned char* data;
  data = stbi_load(file.c_str(), &m_Width, &m_Height, &m_ChannelCount, 0);
  
  // Set up the UV wrapping and MIP mapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    Log::Error("Could not load texture from file: " + file);
    return TEXTURE_ERROR;
  }

  // Free the data
  stbi_image_free(data);

  // Disable Texture state
  glBindTexture(GL_TEXTURE_2D, 0u);

  return m_TextureID;
}

GLuint Texture::GetTextureID() const noexcept
{
  return m_TextureID;
}
