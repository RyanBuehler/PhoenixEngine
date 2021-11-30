//------------------------------------------------------------------------------
// File:    PNGReader.h
// Author:  Ryan Buehler
// Created: November 30, 2021
// Desc:    Utilizes a simple PNG reader stb_image
//------------------------------------------------------------------------------
#pragma once

class PNGReader
{
public:
  struct ImageData
  {
    int Width, Height, Channels;
    unsigned char* Data;
  };

public:
  PNGReader() noexcept;
  ~PNGReader();
  PNGReader(const PNGReader&) = delete;
  PNGReader& operator=(const PNGReader&) = delete;
  PNGReader(PNGReader&&) = delete;
  PNGReader& operator=(PNGReader&&) = delete;

  bool LoadPNG(const string& filename) noexcept;

  ImageData GetImageData() noexcept;

private:
  ImageData m_Image;
};


