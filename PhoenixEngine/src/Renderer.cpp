#include "pch.h"
#include "Renderer.h"
#include "GameObject.h"

#pragma region ImGui

#ifdef _IMGUI

#include "ImGuiManager.h"

#endif

#pragma endregion

Renderer::Renderer() noexcept :
  m_ShaderManager(),
  m_ContextManager(),
  m_MeshManager(),
  m_MeshRenderer(),
  m_DiffuseContextID(ContextManager::CONTEXT_ERROR),
  m_DebugContextID(ContextManager::CONTEXT_ERROR)
{
  // Load a default context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DIFFUSE);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DIFFUSE);
  m_DiffuseContextID = m_ContextManager.CreateNewContext("Diffuse", vID, fID);
  m_ContextManager.SetContext(m_DiffuseContextID);

  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "model_matrix");

  // Load a Line Drawing context
  vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DEBUG);
  fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DEBUG);
  m_DebugContextID = m_ContextManager.CreateNewContext("Debug", vID, fID);
  m_ContextManager.SetContext(m_DebugContextID);

  m_ContextManager.AddNewUniformAttribute(m_DebugContextID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DebugContextID, "view_matrix");

  ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0u);
  ContextManager::VertexAttribute vaColor("color", 4, GL_FLOAT, GL_FALSE, sizeof(vec4), sizeof(vec4));
  m_ContextManager.AddNewVertexAttribute(m_DebugContextID, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_DebugContextID, vaColor);

  Log::Trace("Renderer initialized.");
}

Renderer::~Renderer()
{
  Log::Trace("Renderer destroyed.");
}

void Renderer::OnBeginFrame() const noexcept
{
  // Clear the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::OnEndFrame() const noexcept
{
#pragma region ImGUI
#ifdef _IMGUI

  if (ImGui::GraphicsWindowEnabled)
  {
    ImGui::Manager->OnImGuiGraphicsUpdate();
  }

#endif // _IMGUI
#pragma endregion

  glBindVertexArray(0u);
  glUseProgram(0u);
}

void Renderer::RenderGameObject(GameObject& gameObject)
{
  if (gameObject.m_bIsDirty)
  {
    // Unknown Mesh ID, check for new id with file name
    if (gameObject.m_MeshID == MeshManager::MESH_INDEX_ERROR)
    {
      gameObject.m_MeshID = m_MeshManager.LoadMesh(gameObject.GetMeshFileName());
      if (gameObject.m_MeshID == MeshManager::MESH_INDEX_ERROR)
      {
        Log::Error("Could not load mesh: " + gameObject.GetMeshFileName());
        return;
      }
    }
  }
  //TODO: batch rendering by mesh
  // Bind the model transform matrix
  glUniformMatrix4fv(m_ContextManager.GetCurrentUniformAttributes()[2].ID, 1, false, &gameObject.GetMatrix()[0][0]);

  m_MeshManager.RenderMesh(gameObject.m_MeshID);
}

void Renderer::RenderGameObjects(vector<GameObject>& gameObjects, Camera& activeCamera)
{
  m_ContextManager.SetContext(m_DiffuseContextID);
  //TODO: Combine these for efficiency
  // Set Perspective Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[0].ID,
    1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
  // Set View Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[1].ID,
    1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);

  vec3 lastPosition = vec3(gameObjects[7].GetPosition());
  // Render our list of game objects
  for (GameObject& go : gameObjects)
  {
    // Skip disabled game objects
    if (go.IsActive())
    {
      RenderGameObject(go);
      if (go.GetMeshFileName() == "sphere")
      {
        ;
        //DebugRenderer::I().AddLine(go.GetPosition(), lastPosition);
      }
      lastPosition = go.GetPosition();
    }
  }
  glUseProgram(0u);

  m_ContextManager.SetContext(m_DebugContextID);
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[0].ID,
    1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
  // Set View Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[1].ID,
    1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);

  for (int x = -10; x <= 10; ++x)
  {
    for (int y = -10; y <= 10; ++y)
    {
      DebugRenderer::I().AddLine(vec3(x, y, 0.f), vec3(-x, y, 0.f));
      DebugRenderer::I().AddLine(vec3(x, y, 0.f), vec3(x, -y, 0.f));
    }
  }
  DebugRenderer::I().RenderLines();
}
