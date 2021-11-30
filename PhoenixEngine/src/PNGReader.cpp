//------------------------------------------------------------------------------
// File:    PNGReader.cpp
// Author:  Ryan Buehler
// Created: November 30, 2021
// Desc:    Utilizes a simple PNG reader stb_image
//------------------------------------------------------------------------------
#include "pch.h"
#include "PNGReader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

PNGReader::PNGReader() noexcept :
  m_Image()
{
}

PNGReader::~PNGReader()
{
  // Free the data
  stbi_image_free(m_Image.Data);
}

bool PNGReader::LoadPNG(const string& filename) noexcept
{
  // Load the data
  m_Image.Data = stbi_load(filename.c_str(), &m_Image.Width, &m_Image.Height, &m_Image.Channels, 0);
  if(!m_Image.Data)
  {
    Log::Error("Could not load texture from file: " + filename);
    return false;
  }

  return true;
}

PNGReader::ImageData PNGReader::GetImageData() noexcept
{
  return m_Image;
}
