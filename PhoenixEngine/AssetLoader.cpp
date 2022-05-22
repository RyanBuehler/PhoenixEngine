//------------------------------------------------------------------------------
// File:    AssetLoader.cpp
// Author:  Ryan Buehler
// Created: Friday, May 20, 2022
// Desc:    Loads 3D assets from disk
//------------------------------------------------------------------------------
#include "pch.h"
#include "AssetLoader.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void AssetLoader::LoadModel(const string& FileName, Mesh& NewMesh) noexcept
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(Paths::MODEL_PATH + FileName,
    aiProcess_JoinIdenticalVertices &
    aiProcess_Triangulate &
    aiProcess_GenNormals &
    aiProcess_GenUVCoords &
    aiProcess_PreTransformVertices);

  const auto mesh = scene->mMeshes[0];
  NewMesh.m_NormalsAreCalculated = mesh->HasNormals();
  const bool bHasTexCoords = mesh->mTextureCoords[0];

  for (unsigned i = 0u; i < mesh->mNumVertices; ++i)
  {
    NewMesh.AddVertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

    if (NewMesh.m_NormalsAreCalculated)
    {
      NewMesh.AddVertexNormal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
    }

    if (bHasTexCoords) // does the mesh contain texture coordinates?
    {
      NewMesh.AddTexcoord(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    else
    {
      NewMesh.AddTexcoord(0.0f, 0.0f);
    }
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    const aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j += 3)
    {
      NewMesh.AddTriangle(face.mIndices[j], face.mIndices[j + 1], face.mIndices[j + 2]);
    }
  }

  if (!NewMesh.m_NormalsAreCalculated)
  {
    NewMesh.CalculateNormals();
  }
}
