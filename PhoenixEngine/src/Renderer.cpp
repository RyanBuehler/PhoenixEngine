#include "pch.h"
#include "Renderer.h"
#include "GameObject.h"
#include "LightingSystem.h"
#include "Material.h"
#include "MeshComponent.h"
#include "DebugRenderer.h"

#pragma region ImGui

#ifdef _IMGUI

#include "ImGUIManager.h"

#endif

#pragma endregion

//TODO: Temporary solution for testing
const char* tempcubemap[6] = {
    "res/textures/skybox/posx.jpg",
    "res/textures/skybox/negx.jpg",
    "res/textures/skybox/posy.jpg",
    "res/textures/skybox/negy.jpg",
    "res/textures/skybox/posz.jpg",
    "res/textures/skybox/negz.jpg"
};

Renderer::Renderer(const bool DepthBufferEnabled, const bool BackFaceCullEnabled) noexcept :
  m_Skybox(tempcubemap),
  m_hSkyboxContext(Error::Context::INVALID_CONTEXT),
  m_hBlinnPhong(Error::INVALID_INDEX),
  m_hDebugContext(Error::Context::INVALID_CONTEXT),
  m_LightingBlockPrintId(Error::INVALID_INDEX),
  m_LightingBlockID(Error::INVALID_INDEX)
{
  DepthBufferEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  BackFaceCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.0);

  LoadContexts();

  Log::trace("Renderer initialized.");
}

Renderer::~Renderer()
{
  Log::trace("Renderer destroyed.");
}

void Renderer::OnBeginFrame() const noexcept
{
  // Clear the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_UniformBlockManager.SendData(m_LightingBlockID);
}

void Renderer::OnEndFrame() noexcept
{
#pragma region ImGUI
#ifdef _IMGUI

  if (ImGui::GRAPHICS_WINDOW_ENABLED)
  {
    ImGui::MANAGER->OnImGuiGraphicsUpdate();

    // TODO: Cool feature, but needs to be rebuilt and relocated
    if (ImGui::GRAPHICS_REBUILD_SHADERS)
    {
      ImGui::GRAPHICS_REBUILD_SHADERS = false;

      //m_ShaderManager.RelinkShader(
      //  m_ContextManager.GetProgram(m_hPhongLighting),
      //  m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGLIGHT),
      //  m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGLIGHT),
      //  "PhongLighting.vert", "PhongLighting.frag");

      //m_ShaderManager.RelinkShader(
      //  m_ContextManager.GetProgram(m_hPhongShading),
      //  m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGSHADE),
      //  m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGSHADE),
      //  "PhongShading.vert", "PhongShading.frag");

      m_ShaderManager.RelinkShader(
        m_ContextManager.GetProgram(m_hBlinnPhong),
        m_ShaderManager.GetVertexShaderID(Shader::Vertex::BLINN_PHONG),
        m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BLINN_PHONG),
        "BlinnPhong.vert", "BlinnPhong.frag");

      //m_ShaderManager.RelinkShader(
      //  m_ContextManager.GetProgram(m_hBlinnPhongRefract),
      //  m_ShaderManager.GetVertexShaderID(Shader::Vertex::BLINNPHONGREFRACT),
      //  m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BLINNPHONGREFRACT),
      //  "BlinnPhongRefract.vert", "BlinnPhongRefract.frag");

      //m_ShaderManager.RelinkShader(
      //  m_ContextManager.GetProgram(m_hPhongTexture),
      //  m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGTEXTURE),
      //  m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGTEXTURE),
      //  "PhongTexture.vert", "PhongTexture.frag");

      //m_ShaderManager.RelinkShader(
      //  m_ContextManager.GetProgram(m_hReflection),
      //  m_ShaderManager.GetVertexShaderID(Shader::Vertex::REFLECTION),
      //  m_ShaderManager.GetFragmentShaderID(Shader::Fragment::REFLECTION),
      //  "Reflection.vert", "Reflection.frag");
    }

    if (ImGui::GRAPHICS_REBUILD_MESHES)
    {
      ImGui::GRAPHICS_REBUILD_MESHES = false;

      m_MeshManager.UnloadMeshes();
    }

    for (int i = 0; i < 6; ++i)
    {
      ImGui::GRAPHICS_DISPLAY_TEXTURE[i] = m_EnvMap.GetTextureHandle(i);
    }
  }

#endif // _IMGUI
#pragma endregion

  glBindVertexArray(0u);
  glUseProgram(0u);

  if (m_RenderStats.IsActive())
  {
    m_RenderStats.OnEndFrame();
    if (m_RenderStats.GetFrameCount() % 60 == 0)
    {
      ImGui::GRAPHICS_FPS = m_RenderStats.GetFPS();
    }
  }
}

void Renderer::RenderScene(vector<GameObject>& GameObjects, Camera& ActiveCamera)
{
  //RenderFirstPass(gameObjects);

  RenderSecondPass(GameObjects, ActiveCamera);

#pragma region ImGui

#ifdef _IMGUI
  m_ContextManager.SetContext(m_hDebugContext);

  // Set Perspective Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[0].ID,
    1, GL_FALSE, &ActiveCamera.GetPersMatrix()[0][0]);
  // Set View Matrix
  glUniformMatrix4fv(
    m_ContextManager.GetCurrentUniformAttributes()[1].ID,
    1, GL_FALSE, &ActiveCamera.GetViewMatrix()[0][0]);

  static constexpr mat4 mat_identity(1.f);

  // Bind the identity for permanent line's model matrix
  glUniformMatrix4fv(m_ContextManager.GetCurrentUniformAttributes()[2].ID, 1, false, &mat_identity[0][0]);
  //DebugRenderer::I().RenderPermanentLines();


  if (ImGui::GRAPHICS_DEBUG_RENDER_SURFACE_NORMALS)
  {
    // RenderNormals(id, ImGui::GraphicsDebugNormalLength);
    // Render our list of game objects
    m_ContextManager.SetContext(m_hDebugContext);
    for (GameObject& go : GameObjects)
    {
      // Skip disabled game objects
      if (go.IsActive())
      {
        //RenderNormals(go, ImGui::GraphicsDebugNormalLength, Normals::Type::SURFACE);
      }
    }
    glUseProgram(0u);
  }
  else if (ImGui::GRAPHICS_DEBUG_RENDER_VERTEX_NORMALS)
  {
    m_ContextManager.SetContext(m_hDebugContext);

    // Render our list of game objects
    for (GameObject& go : GameObjects)
    {
      // Skip disabled game objects
      if (go.IsActive())
      {
        //RenderNormals(go, ImGui::GraphicsDebugNormalLength, Normals::Type::VERTEX);
      }
    }
    glUseProgram(0u);
  }
  else
  {
    m_ContextManager.SetContext(m_hDebugContext);
    DebugRenderer::I().RenderLines();
  }
#endif

#pragma endregion
}

void Renderer::RenderFirstPass(vector<GameObject>& GameObjects)
{
  Camera& activeCamera = m_EnvMap.GetCamera();
  glViewport(0, 0, 1024, 1024);

  for (int i = 0; i < 6; ++i)
  {
    m_EnvMap.Bind(i);
    switch (i)
    {
    case 0:
      activeCamera.LookAt({ 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
      break;
    case 1:
      activeCamera.LookAt({ -1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
      break;
    case 2:
      activeCamera.LookAt({ 0.f, -1.f, 0.f }, { 0.f, 0.f, 1.f });
      break;
    case 3:
      activeCamera.LookAt({ 0.f, 1.f, 0.f }, { 0.f, 0.f, -1.f });
      break;
    case 4:
      activeCamera.LookAt({ 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });
      break;
    case 5:
      activeCamera.LookAt({ 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f });
      break;
    default:
      return;
    }

    // Render our list of game objects
    for (GameObject& go : GameObjects)
    {
      // Skip disabled game objects
      if (!go.IsActive())
        continue;

      //switch (ImGui::GraphicsSelectedShader)
      //{
      //  // Reflection
      //case 3:
      //  if (go.GetMaterial().GetType() == Material::Type::REFLECTREFRACT)
      //  {
      //    continue;
      //  }
      //  else
      //  {
      //    m_ContextManager.SetContext(m_hPhongShading);
      //  }
      //  break;
      //  //Lighting
      //case 0:
      //  m_ContextManager.SetContext(m_hPhongLighting);
      //  break;
      //  //Shading
      //case 1:
      //  m_ContextManager.SetContext(m_hPhongShading);
      //  break;
      //  //Blinn
      //case 2:
      //default:
      //  m_ContextManager.SetContext(m_hBlinnPhong);
      //  break;
      //}
      m_ContextManager.SetContext(m_hBlinnPhong);

      const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();
      //TODO: Combine these for efficiency
      // Set Perspective Matrix
      glUniformMatrix4fv(uniforms[0].ID, 1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
      // Set View Matrix
      glUniformMatrix4fv(uniforms[1].ID, 1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);

      // Set Cam Position
      glUniform3fv(uniforms[2].ID, 1, &activeCamera.GetPosition()[0]);

      const LightingSystem::GlobalLightingData& globalLighting = ImGui::LIGHTING_GLOBAL_DATA;
      glUniform3fv(uniforms[3].ID, 1, &globalLighting.AmbientIntensity[0]);
      glUniform3fv(uniforms[4].ID, 1, &globalLighting.FogIntensity[0]);
      glUniform1f(uniforms[5].ID, globalLighting.FogNear);
      glUniform1f(uniforms[6].ID, globalLighting.FogFar);
      glUniform1f(uniforms[7].ID, globalLighting.AttConstant);
      glUniform1f(uniforms[8].ID, globalLighting.AttLinear);
      glUniform1f(uniforms[9].ID, globalLighting.AttQuadratic);

      RenderGameObject(go);
    }

    const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();
    //TODO: Combine these for efficiency
    // Set Perspective Matrix
    glUniformMatrix4fv(uniforms[0].ID, 1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
    // Set View Matrix
    glUniformMatrix4fv(uniforms[1].ID, 1, GL_FALSE, &activeCamera.GetViewMatrix()[0][0]);
    RenderSkybox(activeCamera);
  }

  glUseProgram(0u);
}

void Renderer::RenderSecondPass(vector<GameObject>& GameObjects, Camera& ActiveCamera)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  const Camera::Viewport& vp = ActiveCamera.GetViewport();
  glViewport(vp.X, vp.Y, vp.W, vp.H);

  m_ContextManager.SetContext(m_hBlinnPhong);

  //TODO: Render all Mesh Components
  // Render our list of game objects
  for (GameObject& go : GameObjects)
  {
    // Skip disabled game objects
    if (!go.IsActive())
      continue;

    const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();

    int x = 0;
    // Set View/Perspective Matrix
    glUniformMatrix4fv(uniforms[x++].ID, 1, GL_FALSE, &ActiveCamera.GetVPMatrix()[0][0]);

    // Set Cam Position
    glUniform3fv(uniforms[x++].ID, 1, &ActiveCamera.GetPosition()[0]);

    const LightingSystem::GlobalLightingData& globalLighting = ImGui::LIGHTING_GLOBAL_DATA;
    glUniform3fv(uniforms[x++].ID, 1, &globalLighting.AmbientIntensity[0]);
    glUniform3fv(uniforms[x++].ID, 1, &globalLighting.FogIntensity[0]);
    glUniform1f(uniforms[x++].ID, globalLighting.FogNear);
    glUniform1f(uniforms[x++].ID, globalLighting.FogFar);
    glUniform1f(uniforms[x++].ID, globalLighting.AttConstant);
    glUniform1f(uniforms[x++].ID, globalLighting.AttLinear);
    glUniform1f(uniforms[x++].ID, globalLighting.AttQuadratic);

    RenderGameObject(go);
  }

  //TODO: Don't render this first, and don't render it here
  //RenderSkybox(activeCamera);

  glUseProgram(0u);
}

void Renderer::RenderSkybox(Camera& ActiveCamera)
{
  m_ContextManager.SetContext(m_hSkyboxContext);

  glDepthFunc(GL_LEQUAL);
  //glDepthMask(GL_FALSE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox.GetID());
  glActiveTexture(GL_TEXTURE0);

  const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();

  glm::mat4 view = glm::mat4(glm::mat3(ActiveCamera.GetViewMatrix()));

  // Set Perspective Matrix
  glUniformMatrix4fv(uniforms[0].ID, 1, GL_FALSE, &ActiveCamera.GetPersMatrix()[0][0]);
  // Set View Matrix
  glUniformMatrix4fv(uniforms[1].ID, 1, GL_FALSE, &view[0][0]);

  m_MeshManager.RenderMesh(m_SkyboxMeshId);

  glDepthFunc(GL_LESS);
  //glDepthMask(GL_TRUE);
}

void Renderer::RenderGameObject(GameObject& GameObject)
{
  const auto meshComp = GameObject.GetFirstComponentByType(Component::Type::MESH);
  if (!meshComp.has_value())
    return;
  const shared_ptr<MeshComponent> meshCompPtr = dynamic_pointer_cast<MeshComponent>(meshComp.value());
  unsigned MeshID = meshCompPtr->GetMeshID();

  if (GameObject.m_bIsDirty)
  {
    // Unknown Mesh ID, check for new id with file name
    if (meshCompPtr->GetMeshID() == Error::INVALID_INDEX)
    {
      const string& meshFile = meshCompPtr->GetMeshFileName();
      meshCompPtr->SetMeshID(m_MeshManager.LoadMesh(meshFile, true, true, ImGui::GRAPHICS_SELECTED_PROJECTION));
      if (meshCompPtr->GetMeshID() == Error::INVALID_INDEX)
      {
        Log::error("Could not load mesh: " + meshFile);
        return;
      }
      MeshID = meshCompPtr->GetMeshID();
    }
  }

  const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();

  int x = 9;
  // Bind the model transform matrix
  glUniformMatrix4fv(uniforms[x++].ID, 1, false, &GameObject.GetMatrix()[0][0]);

  const Material& mat =
    meshCompPtr->GetMaterial().GetType() != Material::Type::GLOBAL ?
    meshCompPtr->GetMaterial() :
    ImGui::LIGHTING_GLOBAL_MATERIAL;

  // Mat emissive
  glUniform3fv(uniforms[x++].ID, 1, &mat.GetEmissive()[0]);
  // Mat ambient
  glUniform1f(uniforms[x++].ID, mat.GetAmbient());
  // Mat diffuse
  glUniform1f(uniforms[x++].ID, mat.GetDiffuse());
  // Mat spec
  glUniform1f(uniforms[x++].ID, mat.GetSpecular());
  // Mat spec exp
  glUniform1f(uniforms[x++].ID, mat.GetSpecularExp());

  m_MeshManager.RenderMesh(MeshID);
}

#pragma region ImGui

#ifdef _IMGUI

//void Renderer::RenderNormals(GameObject& gameObject, float length, Normals::Type normalType) noexcept
//{
//  if (gameObject.m_bIsDirty)
//  {
//    // Unknown Mesh ID, check for new id with file name
//    if (gameObject.m_MeshID == Error::INVALID_INDEX)
//    {
//      gameObject.m_MeshID = m_MeshManager.LoadMesh(gameObject.GetMeshFileName(), true, true);
//      if (gameObject.m_MeshID == Error::INVALID_INDEX)
//      {
//        Log::Error("Could not load mesh: " + gameObject.GetMeshFileName());
//        return;
//      }
//    }
//  }
//  // Bind the model transform matrix
//  glUniformMatrix4fv(m_ContextManager.GetCurrentUniformAttributes()[2].ID, 1, false, &gameObject.GetMatrix()[0][0]);
//
//  switch (normalType)
//  {
//    case Normals::Type::VERTEX:
//      m_MeshManager.RenderVertexNormals(gameObject.m_MeshID, length);
//      break;
//    case Normals::Type::TRIANGLE:
//      m_MeshManager.RenderSurfaceNormals(gameObject.m_MeshID, length);
//      break;
//    case Normals::Type::SURFACE:
//      m_MeshManager.RenderSurfaceNormals(gameObject.m_MeshID, length);
//      break;
//    case Normals::Type::COUNT:
//    default:
//      break;
//  }
//}

#endif

#pragma endregion

void Renderer::LoadContexts() noexcept
{
  LoadBlinnPhongContext();

  LoadDebugContext();

  LoadSkyboxContext();

  //TODO: Look into this
  const GLuint program = m_ContextManager.GetProgram(m_hBlinnPhong);

  //TODO: Temporarily hardcoded
  UniformBlockManager::UniformBlockPrint LightArrayPrint;
  LightArrayPrint.BlockName = "LightArray";
  LightArrayPrint.DataSize = sizeof(Light::Data) * 16;

  m_LightingBlockPrintId = m_UniformBlockManager.RegisterNewBlockPrint(LightArrayPrint);
  m_LightingBlockID = m_UniformBlockManager.CreateNewBlock(m_LightingBlockPrintId, program, &ImGui::LIGHTING_DATA_ARRAY[0]);
}

//void Renderer::LoadDiffuseContext() noexcept
//{
//  // Load a default context
//  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DIFFUSE);
//  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DIFFUSE);
//  m_hDiffuseContext = m_ContextManager.CreateNewContext("Diffuse", vID, fID);
//  m_ContextManager.SetContext(m_hDiffuseContext);
//
//  // TODO: Convert this to a Uniform Block
//  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "pers_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "view_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "model_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "diffuse_light");
//  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "ambient_light");
//
//  ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
//  ContextManager::VertexAttribute vaNormal("normal", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
//  m_ContextManager.AddNewVertexAttribute(m_hDiffuseContext, vaPosition);
//  m_ContextManager.AddNewVertexAttribute(m_hDiffuseContext, vaNormal);
//
//  Log::Trace("Diffuse Context loaded.");
//}
//
//void Renderer::LoadPhongLightingContext() noexcept
//{
//  // Load Phong lighting context
//  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGLIGHT);
//  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGLIGHT);
//  m_hPhongLighting = m_ContextManager.CreateNewContext("Phong Lighting", vID, fID);
//
//  m_ContextManager.SetContext(m_hPhongLighting);
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "pers_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "view_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "cam_position");
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_fog");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_fog_near");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_fog_far");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_att1");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_att2");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_att3");
//
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "model_matrix");
//
//  //TODO: Uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_emit");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_dif");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_spc");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_spc_exp");
//
//  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
//  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
//  ContextManager::VertexAttribute vaTexCoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongLighting, vaPosition);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongLighting, vaNormal);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongLighting, vaTexCoords);
//
//  Log::Trace("Phong Lighting Context loaded.");
//}
//
//void Renderer::LoadPhongShadingContext() noexcept
//{
//  // Load Phong shading context
//  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGSHADE);
//  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGSHADE);
//  m_hPhongShading = m_ContextManager.CreateNewContext("Phong Shading", vID, fID);
//
//  m_ContextManager.SetContext(m_hPhongShading);
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "pers_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "view_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "cam_position");
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_fog");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_fog_near");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_fog_far");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_att1");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_att2");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_att3");
//
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "model_matrix");
//
//  //TODO: Uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_emit");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_dif");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_spc");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_spc_exp");
//
//  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
//  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
//  ContextManager::VertexAttribute vaTexCoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongShading, vaPosition);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongShading, vaNormal);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongShading, vaTexCoords);
//
//  Log::Trace("Phong Shading Context loaded.");
//}

void Renderer::LoadBlinnPhongContext() noexcept
{
  // Load Phong lighting context
  const unsigned vId = m_ShaderManager.GetVertexShaderID(Shader::Vertex::BLINN_PHONG);
  const unsigned fId = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BLINN_PHONG);
  m_hBlinnPhong = m_ContextManager.CreateNewContext("Blinn Phong", vId, fId);

  m_ContextManager.SetContext(m_hBlinnPhong);

  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "vp_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "cam_position");

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "global_att3");

  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "model_matrix");

  //TODO: Uniform block
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "mat_spc_exp");

  const ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  const ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  const ContextManager::VertexAttribute vaTexCoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhong, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhong, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhong, vaTexCoords);

  Log::trace("Blinn-Phong Shading Context loaded.");
}

//void Renderer::LoadPhongTextureContext() noexcept
//{
//  // Load Phong lighting context
//  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGTEXTURE);
//  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGTEXTURE);
//  m_hPhongTexture = m_ContextManager.CreateNewContext("Phong Texturing", vID, fID);
//
//  m_ContextManager.SetContext(m_hPhongTexture);
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "pers_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "view_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "cam_position");
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_fog");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_fog_near");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_fog_far");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_att1");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_att2");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_att3");
//
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "model_matrix");
//
//  //TODO: Uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_emit");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_dif");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_spc");
//  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_spc_exp");
//
//  //ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
//  //ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
//  //ContextManager::VertexAttribute vaTexCoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 2 * sizeof(vec3));
//  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
//  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
//  ContextManager::VertexAttribute vaTexCoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongTexture, vaPosition);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongTexture, vaNormal);
//  m_ContextManager.AddNewVertexAttribute(m_hPhongTexture, vaTexCoords);
//
//  Log::Trace("Phong Texture Context loaded.");
//}

void Renderer::LoadDebugContext() noexcept
{
  // Load a Debug Drawing context
  const unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DEBUG);
  const unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DEBUG);
  m_hDebugContext = m_ContextManager.CreateNewContext("Debug", vID, fID);
  m_ContextManager.SetContext(m_hDebugContext);

  m_ContextManager.AddNewUniformAttribute(m_hDebugContext, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hDebugContext, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hDebugContext, "model_matrix");

  const ContextManager::VertexAttribute vaPosition =
    ContextManager::VertexAttribute("position", 4, GL_FLOAT, GL_FALSE, 2 * sizeof(vec4), 0u);
  const ContextManager::VertexAttribute vaColor("color", 4, GL_FLOAT, GL_FALSE, 2 * sizeof(vec4), sizeof(vec4));
  m_ContextManager.AddNewVertexAttribute(m_hDebugContext, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hDebugContext, vaColor);

  Log::trace("DEBUG Context loaded.");
}

void Renderer::LoadSkyboxContext() noexcept
{
  // Load skybox context
  const unsigned vId = m_ShaderManager.GetVertexShaderID(Shader::Vertex::SKYBOX);
  const unsigned fId = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::SKYBOX);
  m_hSkyboxContext = m_ContextManager.CreateNewContext("Skybox", vId, fId);

  m_ContextManager.SetContext(m_hSkyboxContext);

  m_ContextManager.AddNewUniformAttribute(m_hSkyboxContext, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hSkyboxContext, "view_matrix");

  const ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  m_ContextManager.AddNewVertexAttribute(m_hSkyboxContext, vaPosition);

  m_SkyboxMeshId = m_MeshManager.LoadMesh("cube2.obj");

  Log::trace("Skybox Context loaded.");
}

//void Renderer::LoadReflectionContext() noexcept
//{
//  // Load reflection context
//  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::REFLECTION);
//  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::REFLECTION);
//  m_hReflection = m_ContextManager.CreateNewContext("Reflection", vID, fID);
//
//  m_ContextManager.SetContext(m_hReflection);
//
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "pers_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "view_matrix");
//
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "cam_position");
//
//  // TODO: get rid of this
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_fog");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_fog_near");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_fog_far");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_att1");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_att2");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_att3");
//
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "model_matrix");
//
//  //TODO: get rid of this
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_emit");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_dif");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_spc");
//  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_spc_exp");
//
//  ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
//  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
//  m_ContextManager.AddNewVertexAttribute(m_hReflection, vaPosition);
//  m_ContextManager.AddNewVertexAttribute(m_hReflection, vaNormal);
//
//  Log::Trace("Reflection Context loaded.");
//}
//
//void Renderer::LoadBlinnPhongRefractContext() noexcept
//{
//  // Load Phong lighting context
//  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::BLINNPHONGREFRACT);
//  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BLINNPHONGREFRACT);
//  m_hBlinnPhongRefract = m_ContextManager.CreateNewContext("Blinn Phong Reflect/Refract", vID, fID);
//
//  m_ContextManager.SetContext(m_hBlinnPhongRefract);
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "pers_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "view_matrix");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "cam_position");
//
//  // TODO: Convert this to a uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "global_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "global_fog");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "global_fog_near");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "global_fog_far");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "global_att1");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "global_att2");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "global_att3");
//
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "model_matrix");
//
//  //TODO: Uniform block
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "mat_emit");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "mat_amb");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "mat_dif");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "mat_spc");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "mat_spc_exp");
//
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "refract_slider");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "IOR_R");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "IOR_G");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "IOR_B");
//
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "RefractEnabled");
//  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhongRefract, "ReflectEnabled");
//
//  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
//  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
//  ContextManager::VertexAttribute vaTexCoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
//  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhongRefract, vaPosition);
//  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhongRefract, vaNormal);
//  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhongRefract, vaTexCoords);
//
//  Log::Trace("Blinn Phong Refraction Context loaded.");
//}

void Renderer::EnableDepthBuffer() const noexcept
{
  if (DepthBufferIsEnabled())
  {
    Log::warn("Redundant Depth Buffer request to enable. Already enabled.");
    return;
  }

  glEnable(GL_DEPTH_TEST);
}

void Renderer::DisableDepthBuffer() const noexcept
{
  if (!DepthBufferIsEnabled())
  {
    Log::warn("Redundant Depth Buffer request to disable. Already disabled.");
    return;
  }

  glDisable(GL_DEPTH_TEST);
}

bool Renderer::DepthBufferIsEnabled() noexcept
{
  return glIsEnabled(GL_DEPTH_TEST);
}

void Renderer::EnableBackFaceCull() const noexcept
{
  if (BackFaceCullIsEnabled())
  {
    Log::warn("Redundant Depth Buffer request to enable. Already enabled.");
    return;
  }

  glEnable(GL_CULL_FACE);
}

void Renderer::DisableBackFaceCull() const noexcept
{
  if (!BackFaceCullIsEnabled())
  {
    Log::warn("Redundant Depth Buffer request to disable. Already disabled.");
    return;
  }

  glDisable(GL_CULL_FACE);
}

bool Renderer::BackFaceCullIsEnabled() noexcept
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
