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
#include "CameraManager.h"
#include "GameObject.h"

#pragma region ImGUI
#ifdef _IMGUI
#include "ImGuiManager.h"
#endif // _IMGUI
#pragma endregion

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

  void Init(const ShaderManager& shaderManager, ContextManager& contextManager) noexcept;

  void RenderGameObjects(vector<GameObject>& gameObjects, Camera& activeCamera) noexcept;

  void EnableDepthBuffer() noexcept;
  void DisableDepthBuffer() noexcept;
  inline bool DepthBufferIsEnabled() const noexcept;

  void EnableBackFaceCull() noexcept;
  void DisableBackFaceCull() noexcept;
  inline bool BackFaceCullIsEnabled() const noexcept;

private:
  MeshManager m_MeshManager;

  GLint m_ContextID;

  GLuint m_ModelAttributeID;
  GLuint m_PersAttributeID;
  GLuint m_ViewAttributeID; 

  bool m_DebugRenderNormals;

#pragma region ImGUI
#ifdef _IMGUI
  friend class ImGuiManager;
#endif // _IMGUI
#pragma endregion
};
