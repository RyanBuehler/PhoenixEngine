#include "pch.h"
#include "Renderer.h"
#include "GameObject.h"
#include "LightingSystem.h"
#include "Material.h"

#pragma region ImGui

#ifdef _IMGUI

#include "ImGuiManager.h"

#endif

#pragma endregion

Renderer::Renderer(bool depthBufferEnabled, bool backFaceCullEnabled) noexcept :
  m_ShaderManager(),
  m_ContextManager(),
  m_MeshManager(),
  m_Lighting(),
  m_DiffuseContextID(ContextManager::CONTEXT_ERROR),
  m_DebugContextID(ContextManager::CONTEXT_ERROR),
  diffTex("Debug Diff Texture"),
  specTex("Debug Spec Texture")
{
  depthBufferEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  backFaceCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.0);

  LoadContexts();

  Log::Trace("Renderer initialized.");

  //TODO:
  diffTex.LoadTextureFromFile("res/textures/metal_roof_diff_512x512.png");
  specTex.LoadTextureFromFile("res/textures/metal_roof_spec_512x512.png");
}

Renderer::~Renderer()
{
  Log::Trace("Renderer destroyed.");
}

void Renderer::OnBeginFrame() const noexcept
{
  // Clear the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //TODO: For testing purposes only
  glBindBuffer(GL_UNIFORM_BUFFER, uboBuffer);
  //glBufferSubData(GL_UNIFORM_BUFFER, 0, uboSize, &ImGui::LightingDataArray[0]);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(Light::Data) * 16, &ImGui::LightingDataArray[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

void Renderer::RenderGameObjects(vector<GameObject>& gameObjects, Camera& activeCamera)
{
  //TODO: Remove this
  if (ImGui::SceneDrawOrbit)
  {
    // Render any previously stored line
    m_ContextManager.SetContext(m_DebugContextID);
    //TODO: Check if this is being used before Rendering
    DebugRenderer::I().RenderLines();
  }

  //m_ContextManager.SetContext(m_PhongTextureID);
  //const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();
  ////TODO: Combine these for efficiency
  //// Set Perspective Matrix
  //glUniformMatrix4fv(uniforms[0].ID, 1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
  //// Set View Matrix
  //glUniformMatrix4fv(uniforms[1].ID, 1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);
  //
  //// Set Cam Position
  //glUniform3fv(uniforms[2].ID, 1, &activeCamera.GetPosition()[0]);

  //const LightingSystem::GlobalLightingData& globalLighting = ImGui::LightingGlobalData;
  //glUniform3fv(uniforms[3].ID, 1, &globalLighting.AmbientIntensity[0]);
  //glUniform3fv(uniforms[4].ID, 1, &globalLighting.FogIntensity[0]);
  //glUniform1f(uniforms[5].ID, globalLighting.FogNear);
  //glUniform1f(uniforms[6].ID, globalLighting.FogFar);
  //glUniform1f(uniforms[7].ID, globalLighting.AttConstant);
  //glUniform1f(uniforms[8].ID, globalLighting.AttLinear);
  //glUniform1f(uniforms[9].ID, globalLighting.AttQuadratic);
  //
  //glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, diffTex.GetTextureID());
  //glActiveTexture(GL_TEXTURE1);
  //glBindTexture(GL_TEXTURE_2D, specTex.GetTextureID());

  // Render our list of game objects
  for (GameObject& go : gameObjects)
  {
    if (!go.IsActive())
      continue;

    if (go.GetMaterial().GetType() == Material::Type::TEXTURE)
    {
      m_ContextManager.SetContext(m_PhongTextureID);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffTex.GetTextureID());
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specTex.GetTextureID());
    }
    else
      m_ContextManager.SetContext(m_PhongShadingID);

    const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();
    //TODO: Combine these for efficiency
    // Set Perspective Matrix
    glUniformMatrix4fv(uniforms[0].ID, 1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
    // Set View Matrix
    glUniformMatrix4fv(uniforms[1].ID, 1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);

    // Set Cam Position
    glUniform3fv(uniforms[2].ID, 1, &activeCamera.GetPosition()[0]);

    const LightingSystem::GlobalLightingData& globalLighting = ImGui::LightingGlobalData;
    glUniform3fv(uniforms[3].ID, 1, &globalLighting.AmbientIntensity[0]);
    glUniform3fv(uniforms[4].ID, 1, &globalLighting.FogIntensity[0]);
    glUniform1f(uniforms[5].ID, globalLighting.FogNear);
    glUniform1f(uniforms[6].ID, globalLighting.FogFar);
    glUniform1f(uniforms[7].ID, globalLighting.AttConstant);
    glUniform1f(uniforms[8].ID, globalLighting.AttLinear);
    glUniform1f(uniforms[9].ID, globalLighting.AttQuadratic);


    // Skip disabled game objects
    RenderGameObject(go);
  }
  glUseProgram(0u);

#pragma region ImGui

#ifdef _IMGUI
  m_ContextManager.SetContext(m_DebugContextID);

  // Set Perspective Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[0].ID,
    1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
  // Set View Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[1].ID,
    1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);

  static constexpr mat4 matIdentity(1.f);

  // Bind the identity for permanent line's model matrix
  glUniformMatrix4fv(m_ContextManager.GetCurrentUniformAttributes()[2].ID, 1, false, &matIdentity[0][0]);
  //DebugRenderer::I().RenderPermanentLines();

  if (ImGui::GraphicsDebugRenderSurfaceNormals)
  {
    // RenderNormals(id, ImGui::GraphicsDebugNormalLength);
    // Render our list of game objects
    for (GameObject& go : gameObjects)
    {
      // Skip disabled game objects
      if (go.IsActive())
      {
        RenderNormals(go, ImGui::GraphicsDebugNormalLength, Normals::Type::SURFACE);
      }
    }
    glUseProgram(0u);
  }
  else if (ImGui::GraphicsDebugRenderVertexNormals)
  {
    m_ContextManager.SetContext(m_DebugContextID);

    // Render our list of game objects
    for (GameObject& go : gameObjects)
    {
      // Skip disabled game objects
      if (go.IsActive())
      {
        RenderNormals(go, ImGui::GraphicsDebugNormalLength, Normals::Type::VERTEX);
      }
    }
    glUseProgram(0u);
  }
  else
  {
    DebugRenderer::I().RenderLines();
  }

  if (ImGui::GraphicsRebuildShaders)
  {
    ImGui::GraphicsRebuildShaders = false;

    m_ShaderManager.RelinkShader(
      m_ContextManager.GetProgram(m_PhongShadingID),
      m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGSHADE),
      m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGSHADE),
      "PhongShading.vert", "PhongShading.frag");

    m_ShaderManager.RelinkShader(
      m_ContextManager.GetProgram(m_PhongTextureID),
      m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGTEXTURE),
      m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGTEXTURE),
      "PhongTexture.vert", "PhongTexture.frag");
  }

#endif

#pragma endregion
}

void Renderer::RenderGameObject(GameObject& gameObject)
{
  if (gameObject.m_bIsDirty)
  {
    // Unknown Mesh ID, check for new id with file name
    if (gameObject.m_MeshID == MeshManager::MESH_INDEX_ERROR)
    {
      gameObject.m_MeshID = m_MeshManager.LoadMesh(gameObject.GetMeshFileName(), true, true);
      if (gameObject.m_MeshID == MeshManager::MESH_INDEX_ERROR)
      {
        Log::Error("Could not load mesh: " + gameObject.GetMeshFileName());
        return;
      }
    }
  }

  if (gameObject.GetMaterial().GetType() == Material::Type::TEXTURE)
  {
    m_ContextManager.SetContext(m_PhongTextureID);
  }


  const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();

  //TODO: batch rendering by mesh
  // Bind the model transform matrix
  glUniformMatrix4fv(uniforms[10].ID, 1, false, &gameObject.GetMatrix()[0][0]);

  //TODO: Material faked temporarily for simplicity

  if (gameObject.GetMaterial().GetType() != Material::Type::GLOBAL)
  {
    const Material& mat = gameObject.GetMaterial();
    glUniform3fv(uniforms[11].ID, 1, &mat.GetEmissive()[0]);
    // Mat ambient
    glUniform1f(uniforms[12].ID, mat.GetAmbient());
    // Mat diffuse
    glUniform1f(uniforms[13].ID, mat.GetDiffuse());
    // Mat spec
    glUniform1f(uniforms[14].ID, mat.GetSpecular());
    // Mat spec exp
    glUniform1f(uniforms[15].ID, mat.GetSpecularExp());
  }
  else
  {
    const Material& mat = ImGui::LightingGlobalMaterial;
    glUniform3fv(uniforms[11].ID, 1, &mat.GetEmissive()[0]);
    // Mat ambient
    glUniform1f(uniforms[12].ID, mat.GetAmbient());
    // Mat diffuse
    glUniform1f(uniforms[13].ID, mat.GetDiffuse());
    // Mat spec
    glUniform1f(uniforms[14].ID, mat.GetSpecular());
    // Mat spec exp
    glUniform1f(uniforms[15].ID, mat.GetSpecularExp());
  }

  m_MeshManager.RenderMesh(gameObject.m_MeshID);
}

#pragma region ImGui

#ifdef _IMGUI

void Renderer::RenderNormals(GameObject& gameObject, float length, Normals::Type normalType) noexcept
{
  if (gameObject.m_bIsDirty)
  {
    // Unknown Mesh ID, check for new id with file name
    if (gameObject.m_MeshID == MeshManager::MESH_INDEX_ERROR)
    {
      gameObject.m_MeshID = m_MeshManager.LoadMesh(gameObject.GetMeshFileName(), true, true);
      if (gameObject.m_MeshID == MeshManager::MESH_INDEX_ERROR)
      {
        Log::Error("Could not load mesh: " + gameObject.GetMeshFileName());
        return;
      }
    }
  }
  // Bind the model transform matrix
  glUniformMatrix4fv(m_ContextManager.GetCurrentUniformAttributes()[2].ID, 1, false, &gameObject.GetMatrix()[0][0]);

  switch (normalType)
  {
  case Normals::Type::VERTEX:
    m_MeshManager.RenderVertexNormals(gameObject.m_MeshID, length);
    break;
  case Normals::Type::TRIANGLE:
    m_MeshManager.RenderSurfaceNormals(gameObject.m_MeshID, length);
    break;
  case Normals::Type::SURFACE:
    m_MeshManager.RenderSurfaceNormals(gameObject.m_MeshID, length);
    break;
  case Normals::Type::COUNT:
  default:
    break;
  }
}

#endif

#pragma endregion

void Renderer::LoadContexts() noexcept
{
  //LoadDiffuseContext();

  //LoadPhongLightingContext();

  LoadPhongShadingContext();

  LoadPhongTextureContext();

  LoadDebugContext();

  GLuint program = m_ContextManager.GetProgram(m_PhongLightingID);

  //TODO: For testing only
  uboIndex = glGetUniformBlockIndex(program, "LightArray");
  // Now get the size
  glGetActiveUniformBlockiv(program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);

  const GLchar* names[] = { "Light.Position", "Light.Ambience", "Light.Diffuse", "Light.Specular" };
  glGetUniformIndices(program, 2, names, indices);
  glGetActiveUniformsiv(program, 2, indices, GL_UNIFORM_OFFSET, offsets);

  glGenBuffers(1, &uboBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, uboBuffer);
  glBufferData(GL_UNIFORM_BUFFER, uboSize, NULL, GL_DYNAMIC_DRAW);
  //glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, uboHandle);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboBuffer, 0, uboSize);
}

void Renderer::LoadDiffuseContext() noexcept
{
  // Load a default context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DIFFUSE);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DIFFUSE);
  m_DiffuseContextID = m_ContextManager.CreateNewContext("Diffuse", vID, fID);
  m_ContextManager.SetContext(m_DiffuseContextID);

  // TODO: Convert this to a Uniform Block
  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "model_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "diffuse_light");
  m_ContextManager.AddNewUniformAttribute(m_DiffuseContextID, "ambient_light");

  ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  ContextManager::VertexAttribute vaNormal("normal", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
  m_ContextManager.AddNewVertexAttribute(m_DiffuseContextID, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_DiffuseContextID, vaNormal);

  Log::Trace("Diffuse Context loaded.");
}

void Renderer::LoadPhongLightingContext() noexcept
{
  // Load Phong lighting context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGLIGHT);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGLIGHT);
  m_PhongLightingID = m_ContextManager.CreateNewContext("Phong Lighting", vID, fID);
  m_ContextManager.SetContext(m_PhongLightingID);

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "cam_vector");

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "global_att3");

  //TODO: Multiple lights and uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "light_pos");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "light_amb");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "light_dif");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "light_spc");

  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "model_matrix");

  //TODO: Uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_PhongLightingID, "mat_spc_exp");

  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
  ContextManager::VertexAttribute vaTexcoords("texcoords", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 2 * sizeof(vec3));
  m_ContextManager.AddNewVertexAttribute(m_PhongLightingID, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_PhongLightingID, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_PhongLightingID, vaTexcoords);

  Log::Trace("Phong Lighting Context loaded.");
}

void Renderer::LoadPhongShadingContext() noexcept
{
  // Load Phong lighting context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGSHADE);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGSHADE);
  m_PhongShadingID = m_ContextManager.CreateNewContext("Phong Shading", vID, fID);

  m_ContextManager.SetContext(m_PhongShadingID);

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "cam_position");

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "global_att3");

  //TODO: Multiple lights and uniform block
  //m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "light_pos");
  //m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "light_amb");
  //m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "light_dif");
  //m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "light_spc");

  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "model_matrix");

  //TODO: Uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_PhongShadingID, "mat_spc_exp");

  //ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  //ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
  //ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 2 * sizeof(vec3));

  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
  m_ContextManager.AddNewVertexAttribute(m_PhongShadingID, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_PhongShadingID, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_PhongShadingID, vaTexcoords);

  Log::Trace("Phong Shading Context loaded.");
}

void Renderer::LoadPhongTextureContext() noexcept
{
  // Load Phong lighting context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGTEXTURE);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGTEXTURE);
  m_PhongTextureID = m_ContextManager.CreateNewContext("Phong Texturing", vID, fID);

  m_ContextManager.SetContext(m_PhongTextureID);

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "cam_position");

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "global_att3");

  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "model_matrix");

  //TODO: Uniform block
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_PhongTextureID, "mat_spc_exp");

  //ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  //ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
  //ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 2 * sizeof(vec3));
  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
  m_ContextManager.AddNewVertexAttribute(m_PhongTextureID, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_PhongTextureID, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_PhongTextureID, vaTexcoords);

  Log::Trace("Phong Texture Context loaded.");
}

void Renderer::LoadDebugContext() noexcept
{
  // Load a Debug Drawing context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DEBUG);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DEBUG);
  m_DebugContextID = m_ContextManager.CreateNewContext("Debug", vID, fID);
  m_ContextManager.SetContext(m_DebugContextID);

  m_ContextManager.AddNewUniformAttribute(m_DebugContextID, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DebugContextID, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_DebugContextID, "model_matrix");

  ContextManager::VertexAttribute vaPosition =
    ContextManager::VertexAttribute("position", 4, GL_FLOAT, GL_FALSE, 2 * sizeof(vec4), 0u);
  ContextManager::VertexAttribute vaColor("color", 4, GL_FLOAT, GL_FALSE, 2 * sizeof(vec4), sizeof(vec4));
  m_ContextManager.AddNewVertexAttribute(m_DebugContextID, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_DebugContextID, vaColor);

  Log::Trace("DEBUG Context loaded.");
}

void Renderer::EnableDepthBuffer() noexcept
{
  if (DepthBufferIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to enable. Already enabled.");
    return;
  }

  glEnable(GL_DEPTH_TEST);
}

void Renderer::DisableDepthBuffer() noexcept
{
  if (!DepthBufferIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to disable. Already disabled.");
    return;
  }

  glDisable(GL_DEPTH_TEST);
}

bool Renderer::DepthBufferIsEnabled() const noexcept
{
  return glIsEnabled(GL_DEPTH_TEST);
}

void Renderer::EnableBackFaceCull() noexcept
{
  if (BackFaceCullIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to enable. Already enabled.");
    return;
  }

  glEnable(GL_CULL_FACE);
}

void Renderer::DisableBackFaceCull() noexcept
{
  if (!BackFaceCullIsEnabled())
  {
    Log::Warn("Redundant Depth Buffer request to disable. Already disabled.");
    return;
  }

  glDisable(GL_CULL_FACE);
}

bool Renderer::BackFaceCullIsEnabled() const noexcept
{
  return glIsEnabled(GL_CULL_FACE);
}

void Renderer::SetRenderModeFill() noexcept
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::SetRenderModeWireframe() noexcept
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
