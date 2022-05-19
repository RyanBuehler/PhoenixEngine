#include "pch.h"
#include "AssetLoader.h"
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void AssetLoader::LoadFBX(const string& File) noexcept
{
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(File.c_str(),
    aiProcess_Triangulate &
    aiProcess_GenNormals);
}
