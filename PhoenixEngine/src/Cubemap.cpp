//------------------------------------------------------------------------------
// File:    Cubemap.cpp
// Author:  Ryan Buehler
// Created: November 30, 2021
// Desc:    A Cubemap of 6 textures for use with skybox, reflection/refraction
//------------------------------------------------------------------------------
#include "pch.h"
#include "Cubemap.h"
#include "PNGReader.h"

Cubemap::Cubemap(const char* filenames[6]) noexcept :
  m_CubemapID(numeric_limits<GLuint>::max())
{
  glGenTextures(1, &m_CubemapID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapID);

  // Load the 6 faces
  for (unsigned i = 0; i < 6; ++i)
  {
    PNGReader reader;
    if (!reader.LoadPNG(filenames[i]))
    {
      Log::Error("[Cubemap.cpp] Error loading image \"" + string(filenames[i]) + "\"");
      return;
    }
    PNGReader::ImageData image = reader.GetImageData();

    GLenum format;
    switch (image.Channels)
    {
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      Log::Error("[Cubemap.cpp] Unknown channel number in loaded texture.");
      return;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image.Width,
      image.Height, 0, format, GL_UNSIGNED_BYTE, image.Data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0u);

  //TODO: Add a name to the cubemap
  Log::Trace("Cubemap loaded.");
}

GLuint Cubemap::GetID()
{
  return m_CubemapID;
}
