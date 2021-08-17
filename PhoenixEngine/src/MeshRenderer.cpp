#include "pch.h"
#include "GL/glew.h"
#include "MeshRenderer.h"

//TODO:
#include "Paths.h"
#include "TriangleMesh.h"
#include "VertexShader.h"
#include "FragmentShader.h"

MeshRenderer::MeshRenderer(bool depthBufferEnabled, bool backFaceCullEnabled) noexcept
{
  depthBufferEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  backFaceCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  glClearDepth(1.0);
}

void MeshRenderer::Init() noexcept
{
  //TODO:
  TriangleMesh mesh;
  VertexShader vs("basic.vert");
  FragmentShader fs("basic.frag");

  Log::Trace("Mesh Renderer initialized.");
}

void MeshRenderer::Update() noexcept
{
  // Clear the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Clear the bound buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0u);
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
