#include "pch.h"
#include "GLEW/glew.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(bool depthBufferEnabled, bool backFaceCullEnabled) noexcept :
  m_ShaderManager(),
  m_ContextManager()
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
  mesh = make_unique<CubeMesh>();
  transform = make_unique<Transform>();

  m_ShaderManager.Init();
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::BASIC);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BASIC);
  static unsigned contextID = m_ContextManager.CreateNewContext(vID, fID);
  GLint program = m_ContextManager.SwapContext(contextID);
  glUseProgram(program);
  camera->Init(program);
  transform->Init(program);
  camera->EnableCamera();

  Log::Trace("Mesh Renderer initialized.");
}

void MeshRenderer::Update() noexcept
{
  // Clear the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  transform->RotateY(0.5f);

  mesh->Bind();
  camera->Bind();
  transform->Bind();


  glDrawElements(GL_TRIANGLES, 3 * mesh->GetFaceCount(), GL_UNSIGNED_INT, 0);

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
