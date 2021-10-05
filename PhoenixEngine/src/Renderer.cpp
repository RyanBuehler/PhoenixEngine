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
  m_LineRenderer(),
  m_DefaultContextID(numeric_limits<unsigned>::max()),
  m_LineContextID(numeric_limits<unsigned>::max())
{
  // Load a default context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::BASIC);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BASIC);
  m_DefaultContextID = m_ContextManager.CreateNewContext(vID, fID);
  m_ContextManager.SetContext(m_DefaultContextID);

  m_ContextManager.AddNewUniformAttribute(m_DefaultContextID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DefaultContextID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DefaultContextID, "model_matrix");

  // Load a Line Drawing context
  vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::LINE);
  fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::LINE);
  m_LineContextID = m_ContextManager.CreateNewContext(vID, fID);
  m_ContextManager.SetContext(m_LineContextID);

  m_ContextManager.AddNewUniformAttribute(m_LineContextID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_LineContextID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_LineContextID, "model_matrix");

  m_MeshManager.LoadMesh("quad");

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
  m_ContextManager.SetContext(m_DefaultContextID);
  //TODO: Combine these for efficiency
  // Set Perspective Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[0].ID,
    1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
  // Set View Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[1].ID,
    1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);

  vec3 lastPosition = vec3(gameObjects[0].GetPosition());
  // Render our list of game objects
  for (GameObject& go : gameObjects)
  {
    // Skip disabled game objects
    if (go.IsActive())
    {
      RenderGameObject(go);
      m_LineRenderer.AddLine(go.GetPosition(), lastPosition);
      lastPosition = go.GetPosition();
    }
  }

  m_ContextManager.SetContext(m_LineContextID);
  m_LineRenderer.RenderLines();
}
