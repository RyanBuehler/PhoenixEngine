#pragma once

class AssetLoader
{
public:

  AssetLoader() = default;
  ~AssetLoader() = default;
  AssetLoader(const AssetLoader&) = delete;
  AssetLoader& operator=(const AssetLoader&) = delete;
  AssetLoader(AssetLoader&&) = delete;
  AssetLoader& operator=(AssetLoader&&) = delete;

  static void LoadFBX(const string& File) noexcept;
};
