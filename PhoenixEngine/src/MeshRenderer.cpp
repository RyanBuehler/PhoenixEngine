//------------------------------------------------------------------------------
// File:    MeshRenderer.cpp
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    A wrapper around the OpenGL graphics API that will render meshes to
//          screen. Current functionality includes:
//          * Rendering static meshes
//------------------------------------------------------------------------------

#include "pch.h"
#include "GLEW/glew.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(bool depthBufferEnabled, bool backFaceCullEnabled) noexcept :
  m_ShaderManager(),
  m_ContextManager(),
  meshID(numeric_limits<unsigned>::max())
{
  depthBufferEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  backFaceCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.0);
}

MeshRenderer::~MeshRenderer()
{
  glUseProgram(0u);
  Log::Trace("MeshRenderer destroyed.");
}

void MeshRenderer::Init() noexcept
{
  camera = make_unique<Camera>();
  transform = make_unique<Transform>();

  m_ShaderManager.Init();
  meshID = m_MeshManager.LoadPrimitive(Mesh::Primitive::CUBE);

  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::BASIC);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BASIC);
  static unsigned contextID = m_ContextManager.CreateNewContext(vID, fID);
  GLint program = m_ContextManager.SwapContext(contextID);
  glUseProgram(program);
  camera->Init(program);
  transform->Init(program);

  camera->EnableCamera();

  Log::Trace("MeshRenderer initialized.");
}

void MeshRenderer::RenderFrame() noexcept
{
  // Clear the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  transform->RotateY(0.5f);

  camera->Bind();
  transform->Bind();
  
  m_MeshManager.RenderMesh(meshID);

  glBindVertexArray(0u);
}

void MeshRenderer::EnableDepthBuffer() noexcept
{
  if (DepthBufferIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to enable. Already enabled.");
    return;
  }

  glEnable(GL_DEPTH_TEST);
}

void MeshRenderer::DisableDepthBuffer() noexcept
{
  if (!DepthBufferIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to disable. Already disabled.");
    return;
  }

  glDisable(GL_DEPTH_TEST);
}

bool MeshRenderer::DepthBufferIsEnabled() const noexcept
{
  return glIsEnabled(GL_DEPTH_TEST);
}

void MeshRenderer::EnableBackFaceCull() noexcept
{
  if (BackFaceCullIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to enable. Already enabled.");
    return;
  }

  glEnable(GL_CULL_FACE);
}

void MeshRenderer::DisableBackFaceCull() noexcept
{
  if (!BackFaceCullIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to disable. Already disabled.");
    return;
  }

  glDisable(GL_CULL_FACE);
}

bool MeshRenderer::BackFaceCullIsEnabled() const noexcept
{
  return glIsEnabled(GL_CULL_FACE);
}
