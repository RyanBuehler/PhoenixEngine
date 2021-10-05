//------------------------------------------------------------------------------
// File:    MeshRenderer.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    MeshRenderer class definitions
//------------------------------------------------------------------------------
#pragma once
#include "MeshManager.h"
#include "GameObject.h"

class MeshRenderer
{
public:
  MeshRenderer(bool depthBufferEnabled = true, bool backFaceCullEnabled = true) noexcept;
  ~MeshRenderer();
  MeshRenderer(const MeshRenderer&) = delete;
  MeshRenderer& operator=(const MeshRenderer&) = delete;
  MeshRenderer(MeshRenderer&&) = delete;
  MeshRenderer& operator=(MeshRenderer&&) = delete;

  void EnableDepthBuffer() noexcept;
  void DisableDepthBuffer() noexcept;
  inline bool DepthBufferIsEnabled() const noexcept;

  void EnableBackFaceCull() noexcept;
  void DisableBackFaceCull() noexcept;
  inline bool BackFaceCullIsEnabled() const noexcept;

private:
};
