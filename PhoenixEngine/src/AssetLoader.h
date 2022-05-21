//------------------------------------------------------------------------------
// File:    AssetLoader.h
// Author:  Ryan Buehler
// Created: Friday, May 20, 2022
// Desc:    Loads 3D assets from disk
//------------------------------------------------------------------------------
#pragma once

#include "Mesh.h"

class AssetLoader
{
public:
  AssetLoader() = delete;
  ~AssetLoader() = delete;
  AssetLoader(const AssetLoader&) = delete;
  AssetLoader& operator=(const AssetLoader&) = delete;
  AssetLoader(AssetLoader&&) = delete;
  AssetLoader& operator=(AssetLoader&&) = delete;

  static void LoadModel(const string& FileName, Mesh& NewMesh) noexcept;
};
