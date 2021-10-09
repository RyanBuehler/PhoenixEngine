//------------------------------------------------------------------------------
// File:    Renderer.h
// Author:  Ryan Buehler
// Created: 10/03/21
// Desc:    Handles all Renderering of the Phoenix Engine
//------------------------------------------------------------------------------
#pragma once
#include "ContextManager.h"
#include "ShaderManager.h"
#include "DebugRenderer.h"
#include "GameObject.h"
#include "Camera.h"
#include "MeshManager.h"

class Renderer
{
public:
  enum class RenderMode
  {
    Fill,
    Wireframe,
    COUNT
  };
public:

  Renderer(bool depthBufferEnabled = true, bool backFaceCullEnabled = true) noexcept;
  ~Renderer();
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  void OnBeginFrame() const noexcept;
  void OnEndFrame() const noexcept;

  void RenderGameObjects(vector<GameObject>& gameObjects, Camera& activeCamera);

  void EnableDepthBuffer() noexcept;
  void DisableDepthBuffer() noexcept;
  inline bool DepthBufferIsEnabled() const noexcept;

  void EnableBackFaceCull() noexcept;
  void DisableBackFaceCull() noexcept;
  inline bool BackFaceCullIsEnabled() const noexcept;

  static void SetRenderModeFill() noexcept;
  static void SetRenderModeWireframe() noexcept;

private:
  void RenderGameObject(GameObject& gameObject);

#pragma region ImGui

#ifdef _IMGUI

  void RenderNormals(GameObject& gameObject, float length, Normals::Type normalType) noexcept;

#endif

#pragma endregion

  void LoadContexts() noexcept;

  ShaderManager m_ShaderManager;
  ContextManager m_ContextManager;
  MeshManager m_MeshManager;

  GLint m_DiffuseContextID;
  GLint m_DebugContextID;
};

