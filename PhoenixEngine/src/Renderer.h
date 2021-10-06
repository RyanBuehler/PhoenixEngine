//------------------------------------------------------------------------------
// File:    Renderer.h
// Author:  Ryan Buehler
// Created: 10/03/21
// Desc:    Handles all Renderering of the Phoenix Engine
//------------------------------------------------------------------------------
#pragma once
#include "ContextManager.h"
#include "ShaderManager.h"
#include "MeshRenderer.h"
#include "DebugRenderer.h"
#include "GameObject.h"
#include "Camera.h"

class Renderer
{
public:

  Renderer() noexcept;
  ~Renderer();
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  void OnBeginFrame() const noexcept;
  void OnEndFrame() const noexcept;

  void RenderGameObjects(vector<GameObject>& gameObjects, Camera& activeCamera);

private:
  void RenderGameObject(GameObject& gameObject);

  ShaderManager m_ShaderManager;
  ContextManager m_ContextManager;
  MeshManager m_MeshManager;
  MeshRenderer m_MeshRenderer;

  GLint m_DiffuseContextID;
  GLint m_DebugContextID;
};
