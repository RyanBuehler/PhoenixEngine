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
#include "LineRenderer.h"
#include "GameObject.h"
#include "Camera.h"

class Renderer
{
public:

  Renderer() noexcept;
  ~Renderer() = default;
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  void OnBeginFrame() const noexcept;
  void OnEndFrame() const noexcept;

  void RenderGameObjects(vector<GameObject>& gameObjects, Camera& activeCamera);

private:
  ShaderManager m_ShaderManager;
  ContextManager m_ContextManager;
  MeshRenderer m_MeshRenderer;
  //LineRenderer m_LineRenderer;

  GLint m_DefaultContextID;
  GLint m_LineContextID;
};
