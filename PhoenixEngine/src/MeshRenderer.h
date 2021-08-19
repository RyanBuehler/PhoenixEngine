#pragma once
#include "ShaderManager.h"
#include "ContextManager.h"

class MeshRenderer
{
public:
  MeshRenderer(bool depthBufferEnabled = true, bool backFaceCullEnabled = true) noexcept;
  ~MeshRenderer() = default;
  MeshRenderer(const MeshRenderer&) = delete;
  MeshRenderer& operator=(const MeshRenderer&) = delete;
  MeshRenderer(MeshRenderer&&) = delete;
  MeshRenderer& operator=(MeshRenderer&&) = delete;

  void Init() noexcept;

  void Update() noexcept;

  void EnableDepthBuffer() noexcept;
  void DisableDepthBuffer() noexcept;
  inline bool DepthBufferIsEnabled() const noexcept;

  void EnableBackFaceCull() noexcept;
  void DisableBackFaceCull() noexcept;
  inline bool BackFaceCullIsEnabled() const noexcept;

private:
  ShaderManager m_ShaderManager;
  ContextManager m_ContextManager;
};
