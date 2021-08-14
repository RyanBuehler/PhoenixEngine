#include "pch.h"
#include "GLEW/glew.h"
#include "MeshRenderer.h"

//TODO:
#include "TriangleMesh.h"

MeshRenderer::MeshRenderer(bool depthBufferEnabled, bool backFaceCullEnabled) noexcept
{
  depthBufferEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  backFaceCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  glClearDepth(1.0);
}

void MeshRenderer::Init() noexcept
{
  ////TODO:
  //float positions[6] =
  //{
  //  -0.5f, -0.5f,
  //  0.f, 0.5f,
  //  0.5f, -0.5f
  //};

  //unsigned int buffer;
  //glGenBuffers(1, &buffer);
  //glBindBuffer(GL_ARRAY_BUFFER, buffer);
  //glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
  //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  //glEnableVertexAttribArray(0);
  //glBindBuffer(GL_ARRAY_BUFFER, 0u);
  TriangleMesh mesh;

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
