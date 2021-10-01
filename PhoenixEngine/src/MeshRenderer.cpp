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
#include "Transform.h"

MeshRenderer::MeshRenderer(bool depthBufferEnabled, bool backFaceCullEnabled) noexcept :
  m_MeshManager(),
  m_ShaderManager(),
  m_ContextManager(),
  m_ModelAttributeID(numeric_limits<GLuint>::max()),
  m_PersAttributeID(numeric_limits<GLuint>::max()),
  m_ViewAttributeID(numeric_limits<GLuint>::max()),
  m_DebugRenderNormals(false)
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
  // Load a default context
  m_ShaderManager.Init();
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::BASIC);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BASIC);
  static unsigned contextID = m_ContextManager.CreateNewContext(vID, fID);
  GLint program = m_ContextManager.SwapContext(contextID);
  glUseProgram(program);

  //TODO: Add this to the context manager
  // Enable the default camera with the current program
  m_PersAttributeID = glGetUniformLocation(program, "pers_matrix");
  m_ViewAttributeID = glGetUniformLocation(program, "view_matrix");
  m_ModelAttributeID = glGetUniformLocation(program, "model_matrix");

  Log::Trace("MeshRenderer initialized.");
}

void MeshRenderer::RenderGameObjects(vector<GameObject>& gameObjects, Camera& activeCamera) noexcept
{
  // Clear the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set View Matrix
  glUniformMatrix4fv(m_ViewAttributeID, 1, false, &activeCamera.GetViewMatrix()[0][0]);
  // Set Perspective Matrix
  glUniformMatrix4fv(m_PersAttributeID, 1, false, &activeCamera.GetPersMatrix()[0][0]);

  // Render our list of game objects
  for (GameObject& go : gameObjects)
  {
    // Skip disabled game objects
    if (!go.IsActive())
    {
      continue;
    }

    //TODO: batch rendering by mesh

    if (go.m_bIsDirty)
    {
      // Unknown Mesh ID, check for new id with file name
      if (go.m_MeshID == MeshManager::MESH_INDEX_ERROR)
      {
        go.m_MeshID = m_MeshManager.LoadMesh(go.GetMeshFileName());
        if (go.m_MeshID == MeshManager::MESH_INDEX_ERROR)
        {
          Log::Error("Could not load mesh: " + go.GetMeshFileName());
          continue;
        }
      }
    }

    // Bind the transform
    glUniformMatrix4fv(m_ModelAttributeID, 1, false, &go.GetMatrix()[0][0]);
    m_MeshManager.RenderMesh(go.m_MeshID);

#pragma region ImGUI
#ifdef _IMGUI

    if (ImGui::GraphicsDebugRenderNormals)
    {
      m_MeshManager.RenderNormals(go.m_MeshID);
    }

#endif // _IMGUI
#pragma endregion

  }

  glBindVertexArray(0u);

#pragma region ImGUI
#ifdef _IMGUI

  if (ImGui::GraphicsWindowEnabled)
  {
    ImGui::Manager->OnImGuiGraphicsUpdate(*this);
  }

#endif // _IMGUI
#pragma endregion
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