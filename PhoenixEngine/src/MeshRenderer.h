//------------------------------------------------------------------------------
// File:    MeshRenderer.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    MeshRenderer class definitions
//------------------------------------------------------------------------------
#pragma once
#include "MeshManager.h"
#include "ShaderManager.h"
#include "ContextManager.h"

class Camera;
class CubeMesh;
class Transform;

class MeshRenderer
{
public:
  MeshRenderer(bool depthBufferEnabled = true, bool backFaceCullEnabled = true) noexcept;
  ~MeshRenderer();
  MeshRenderer(const MeshRenderer&) = delete;
  MeshRenderer& operator=(const MeshRenderer&) = delete;
  MeshRenderer(MeshRenderer&&) = delete;
  MeshRenderer& operator=(MeshRenderer&&) = delete;

  void Init() noexcept;

  void RenderFrame() noexcept;

  void EnableDepthBuffer() noexcept;
  void DisableDepthBuffer() noexcept;
  inline bool DepthBufferIsEnabled() const noexcept;

  void EnableBackFaceCull() noexcept;
  void DisableBackFaceCull() noexcept;
  inline bool BackFaceCullIsEnabled() const noexcept;

private:
  //TODO:
  unique_ptr<Camera> camera;
  unique_ptr<Transform> transform;

  MeshManager m_MeshManager;
  ShaderManager m_ShaderManager;
  ContextManager m_ContextManager;

  unsigned meshID; //TODO
};
