//------------------------------------------------------------------------------
// File:    TextureManager.h
// Author:  Ryan Buehler
// Created: November 8th, 2021
// Desc:    Handles all Textures for the Rendering pipeline
//------------------------------------------------------------------------------
#pragma once

class TextureManager
{
public:
  TextureManager() noexcept;
  ~TextureManager() = default;
  TextureManager(const TextureManager&) = delete;
  TextureManager& operator=(const TextureManager&) = delete;
  TextureManager(TextureManager&&) = delete;
  TextureManager& operator=(TextureManager&&) = delete;
};
