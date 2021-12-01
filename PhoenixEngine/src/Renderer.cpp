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

//TODO: Temporary solution for testing
const char* tempcubemap[6] = {
    "res/textures/skybox/posx.jpg",
    "res/textures/skybox/negx.jpg",
    "res/textures/skybox/posy.jpg",
    "res/textures/skybox/negy.jpg",
    "res/textures/skybox/posz.jpg",
    "res/textures/skybox/negz.jpg"
};

Renderer::Renderer(bool depthBufferEnabled, bool backFaceCullEnabled) noexcept :
  m_ShaderManager(),
  m_ContextManager(),
  m_MeshManager(),
  m_Lighting(),
  m_hDiffuseContext(ContextManager::CONTEXT_ERROR),
  m_hDebugContext(ContextManager::CONTEXT_ERROR),
  diffTex("Debug Diff Texture"),
  specTex("Debug Spec Texture"),
  m_Skybox(tempcubemap),
  envMap()
{
  depthBufferEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  backFaceCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.0);

  LoadContexts();

  Log::Trace("Renderer initialized.");

  //TODO: make these boolean results and move them elsewhere
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
  glBufferData(GL_UNIFORM_BUFFER, sizeof(Light::Data) * 16, &ImGui::LightingDataArray[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::OnEndFrame() noexcept
{
#pragma region ImGUI
#ifdef _IMGUI

  if (ImGui::GraphicsWindowEnabled)
  {
    ImGui::Manager->OnImGuiGraphicsUpdate();

    // TODO: Cool feature, but needs to be rebuilt and relocated
    if (ImGui::GraphicsRebuildShaders)
    {
      ImGui::GraphicsRebuildShaders = false;

      m_ShaderManager.RelinkShader(
        m_ContextManager.GetProgram(m_hPhongLighting),
        m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGLIGHT),
        m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGLIGHT),
        "PhongLighting.vert", "PhongLighting.frag");

      m_ShaderManager.RelinkShader(
        m_ContextManager.GetProgram(m_hPhongShading),
        m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGSHADE),
        m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGSHADE),
        "PhongShading.vert", "PhongShading.frag");

      m_ShaderManager.RelinkShader(
        m_ContextManager.GetProgram(m_hBlinnPhong),
        m_ShaderManager.GetVertexShaderID(Shader::Vertex::BLINNPHONG),
        m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BLINNPHONG),
        "BlinnPhong.vert", "BlinnPhong.frag");

      m_ShaderManager.RelinkShader(
        m_ContextManager.GetProgram(m_hPhongTexture),
        m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGTEXTURE),
        m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGTEXTURE),
        "PhongTexture.vert", "PhongTexture.frag");

      m_ShaderManager.RelinkShader(
        m_ContextManager.GetProgram(m_hReflection),
        m_ShaderManager.GetVertexShaderID(Shader::Vertex::REFLECTION),
        m_ShaderManager.GetFragmentShaderID(Shader::Fragment::REFLECTION),
        "Reflection.vert", "Reflection.frag");
    }

    if (ImGui::GraphicsRebuildMeshes)
    {
      ImGui::GraphicsRebuildMeshes = false;

      m_MeshManager.UnloadMeshes();
    }

    for (int i = 0; i < 6; ++i)
    {
      ImGui::GraphicsDisplayTexture[i] = envMap.GetTextureHandle(i);
    }
  }

#endif // _IMGUI
#pragma endregion

  glBindVertexArray(0u);
  glUseProgram(0u);
}

void Renderer::RenderScene(vector<GameObject>& gameObjects, Camera& activeCamera)
{
  RenderFirstPass(gameObjects);

  RenderSecondPass(gameObjects, activeCamera);

#pragma region ImGui

#ifdef _IMGUI
  m_ContextManager.SetContext(m_hDebugContext);

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
    m_ContextManager.SetContext(m_hDebugContext);
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
    m_ContextManager.SetContext(m_hDebugContext);

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
    m_ContextManager.SetContext(m_hDebugContext);
    DebugRenderer::I().RenderLines();
  }
#endif

#pragma endregion
}

void Renderer::RenderFirstPass(vector<GameObject>& gameObjects)
{
  Camera& activeCamera = envMap.GetCamera();

  for (int i = 0; i < 6; ++i)
  {
    envMap.Bind(i);
    switch (i)
    {
    case 0:
      activeCamera.LookAt({ -1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
      break;
    case 1:
      activeCamera.LookAt({ 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
      break;
    case 2:
      activeCamera.LookAt({ 0.f, -1.f, 0.f }, { 0.f, 0.f, 1.f });
      break;
    case 3:
      activeCamera.LookAt({ 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f });
      break;
    case 4:
      activeCamera.LookAt({ 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f });
      break;
    case 5:
      activeCamera.LookAt({ 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });
      break;
    default:
      return;
    }

    // Render our list of game objects
    for (GameObject& go : gameObjects)
    {
      // Skip disabled game objects
      if (!go.IsActive())
        continue;

      switch (ImGui::GraphicsSelectedShader)
      {
        // Reflection
      case 4:
        if (go.GetMaterial().GetType() == Material::Type::REFLECTIVE)
        {
          continue;
        }
        else
        {
          m_ContextManager.SetContext(m_hPhongShading);
        }
        break;
        //Texture
      case 3:
        if (go.GetMaterial().GetType() == Material::Type::TEXTURE)
        {
          m_ContextManager.SetContext(m_hPhongTexture);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, diffTex.GetTextureID());
          glActiveTexture(GL_TEXTURE1);
          glBindTexture(GL_TEXTURE_2D, specTex.GetTextureID());
        }
        else
        {
          m_ContextManager.SetContext(m_hPhongShading);
        }
        break;
        //Lighting
      case 0:
        m_ContextManager.SetContext(m_hPhongLighting);
        break;
        //Shading
      case 1:
        m_ContextManager.SetContext(m_hPhongShading);
        break;
        //Blinn
      case 2:
      default:
        m_ContextManager.SetContext(m_hBlinnPhong);
        break;
      }

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

      RenderGameObject(go);
    }

    RenderSkybox(activeCamera);
  }

  glUseProgram(0u);
}

void Renderer::RenderSecondPass(vector<GameObject>& gameObjects, Camera& activeCamera)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // Render our list of game objects
  for (GameObject& go : gameObjects)
  {
    if (!go.IsActive())
      continue;

    switch (ImGui::GraphicsSelectedShader)
    {
      // Reflection
    case 4:
      if (go.GetMaterial().GetType() == Material::Type::REFLECTIVE)
      {
        m_ContextManager.SetContext(m_hReflection);
        envMap.EnableTextures();
      }
      else
      {
        m_ContextManager.SetContext(m_hPhongShading);
      }
      break;
      //Texture
    case 3:
      if (go.GetMaterial().GetType() == Material::Type::TEXTURE)
      {
        m_ContextManager.SetContext(m_hPhongTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffTex.GetTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specTex.GetTextureID());
      }
      else
      {
        m_ContextManager.SetContext(m_hPhongShading);
      }
      break;
      //Lighting
    case 0:
      m_ContextManager.SetContext(m_hPhongLighting);
      break;
      //Shading
    case 1:
      m_ContextManager.SetContext(m_hPhongShading);
      break;
      //Blinn
    case 2:
    default:
      m_ContextManager.SetContext(m_hBlinnPhong);
      break;
    }

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

  //TODO: Don't render this first, and don't render it here
  RenderSkybox(activeCamera);

  glUseProgram(0u);
}

void Renderer::RenderSkybox(Camera& activeCamera)
{
  m_ContextManager.SetContext(m_hSkyboxContext);

  glDepthFunc(GL_LEQUAL);
  //glDepthMask(GL_FALSE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox.GetID());
  glActiveTexture(GL_TEXTURE0);

  const vector<ContextManager::UniformAttribute>& uniforms = m_ContextManager.GetCurrentUniformAttributes();

  glm::mat4 view = glm::mat4(glm::mat3(activeCamera.GetViewMatrix()));

  // Set Perspective Matrix
  glUniformMatrix4fv(uniforms[0].ID, 1, GL_FALSE, &activeCamera.GetPersMatrix()[0][0]);
  // Set View Matrix
  glUniformMatrix4fv(uniforms[1].ID, 1, GL_FALSE, &view[0][0]);

  m_MeshManager.RenderMesh(SkyboxMeshID);

  glDepthFunc(GL_LESS);
  //glDepthMask(GL_TRUE);
}

void Renderer::RenderGameObject(GameObject& gameObject)
{
  if (gameObject.m_bIsDirty)
  {
    // Unknown Mesh ID, check for new id with file name
    if (gameObject.m_MeshID == Error::INVALID_INDEX)
    {
      gameObject.m_MeshID = m_MeshManager.LoadMesh(gameObject.GetMeshFileName(), true, true, ImGui::GraphicsSelectedProjection);
      if (gameObject.m_MeshID == Error::INVALID_INDEX)
      {
        Log::Error("Could not load mesh: " + gameObject.GetMeshFileName());
        return;
      }
    }
  }

  if (gameObject.GetMaterial().GetType() == Material::Type::TEXTURE)
  {
    m_ContextManager.SetContext(m_hPhongTexture);
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
    if (gameObject.m_MeshID == Error::INVALID_INDEX)
    {
      gameObject.m_MeshID = m_MeshManager.LoadMesh(gameObject.GetMeshFileName(), true, true);
      if (gameObject.m_MeshID == Error::INVALID_INDEX)
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
  LoadPhongLightingContext();

  LoadPhongShadingContext();

  LoadBlinnPhongContext();

  LoadPhongTextureContext();

  LoadDebugContext();

  LoadSkyboxContext();

  LoadReflectionContext();

  //TODO: Look into this
  GLuint program = m_ContextManager.GetProgram(m_hPhongLighting);

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
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboBuffer, 0, uboSize);
}

void Renderer::LoadDiffuseContext() noexcept
{
  // Load a default context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DIFFUSE);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DIFFUSE);
  m_hDiffuseContext = m_ContextManager.CreateNewContext("Diffuse", vID, fID);
  m_ContextManager.SetContext(m_hDiffuseContext);

  // TODO: Convert this to a Uniform Block
  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "model_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "diffuse_light");
  m_ContextManager.AddNewUniformAttribute(m_hDiffuseContext, "ambient_light");

  ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  ContextManager::VertexAttribute vaNormal("normal", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
  m_ContextManager.AddNewVertexAttribute(m_hDiffuseContext, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hDiffuseContext, vaNormal);

  Log::Trace("Diffuse Context loaded.");
}

void Renderer::LoadPhongLightingContext() noexcept
{
  // Load Phong lighting context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGLIGHT);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGLIGHT);
  m_hPhongLighting = m_ContextManager.CreateNewContext("Phong Lighting", vID, fID);

  m_ContextManager.SetContext(m_hPhongLighting);

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "cam_position");

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "global_att3");

  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "model_matrix");

  //TODO: Uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_hPhongLighting, "mat_spc_exp");

  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
  m_ContextManager.AddNewVertexAttribute(m_hPhongLighting, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hPhongLighting, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_hPhongLighting, vaTexcoords);

  Log::Trace("Phong Lighting Context loaded.");
}

void Renderer::LoadPhongShadingContext() noexcept
{
  // Load Phong shading context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGSHADE);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGSHADE);
  m_hPhongShading = m_ContextManager.CreateNewContext("Phong Shading", vID, fID);

  m_ContextManager.SetContext(m_hPhongShading);

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "cam_position");

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "global_att3");

  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "model_matrix");

  //TODO: Uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_hPhongShading, "mat_spc_exp");

  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
  m_ContextManager.AddNewVertexAttribute(m_hPhongShading, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hPhongShading, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_hPhongShading, vaTexcoords);

  Log::Trace("Phong Shading Context loaded.");
}

void Renderer::LoadBlinnPhongContext() noexcept
{
  // Load Phong lighting context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::BLINNPHONG);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::BLINNPHONG);
  m_hBlinnPhong = m_ContextManager.CreateNewContext("Blinn Phong", vID, fID);

  m_ContextManager.SetContext(m_hBlinnPhong);

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hBlinnPhong, "view_matrix");
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

  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhong, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhong, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_hBlinnPhong, vaTexcoords);

  Log::Trace("Phong Shading Context loaded.");
}

void Renderer::LoadPhongTextureContext() noexcept
{
  // Load Phong lighting context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::PHONGTEXTURE);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::PHONGTEXTURE);
  m_hPhongTexture = m_ContextManager.CreateNewContext("Phong Texturing", vID, fID);

  m_ContextManager.SetContext(m_hPhongTexture);

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "cam_position");

  // TODO: Convert this to a uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "global_att3");

  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "model_matrix");

  //TODO: Uniform block
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_hPhongTexture, "mat_spc_exp");

  //ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  //ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(vec3), sizeof(vec3));
  //ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 2 * sizeof(vec3));
  ContextManager::VertexAttribute vaPosition("position", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), 0);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  ContextManager::VertexAttribute vaTexcoords("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vec2), sizeof(vec3) * 2);
  m_ContextManager.AddNewVertexAttribute(m_hPhongTexture, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hPhongTexture, vaNormal);
  m_ContextManager.AddNewVertexAttribute(m_hPhongTexture, vaTexcoords);

  Log::Trace("Phong Texture Context loaded.");
}

void Renderer::LoadDebugContext() noexcept
{
  // Load a Debug Drawing context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::DEBUG);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::DEBUG);
  m_hDebugContext = m_ContextManager.CreateNewContext("Debug", vID, fID);
  m_ContextManager.SetContext(m_hDebugContext);

  m_ContextManager.AddNewUniformAttribute(m_hDebugContext, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hDebugContext, "view_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hDebugContext, "model_matrix");

  ContextManager::VertexAttribute vaPosition =
    ContextManager::VertexAttribute("position", 4, GL_FLOAT, GL_FALSE, 2 * sizeof(vec4), 0u);
  ContextManager::VertexAttribute vaColor("color", 4, GL_FLOAT, GL_FALSE, 2 * sizeof(vec4), sizeof(vec4));
  m_ContextManager.AddNewVertexAttribute(m_hDebugContext, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hDebugContext, vaColor);

  Log::Trace("DEBUG Context loaded.");
}

void Renderer::LoadSkyboxContext() noexcept
{
  // Load skybox context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::SKYBOX);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::SKYBOX);
  m_hSkyboxContext = m_ContextManager.CreateNewContext("Skybox", vID, fID);

  m_ContextManager.SetContext(m_hSkyboxContext);

  m_ContextManager.AddNewUniformAttribute(m_hSkyboxContext, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hSkyboxContext, "view_matrix");

  ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  m_ContextManager.AddNewVertexAttribute(m_hSkyboxContext, vaPosition);

  SkyboxMeshID = m_MeshManager.LoadMesh("cube2.obj");

  Log::Trace("Skybox Context loaded.");
}

void Renderer::LoadReflectionContext() noexcept
{
  // Load reflection context
  unsigned vID = m_ShaderManager.GetVertexShaderID(Shader::Vertex::REFLECTION);
  unsigned fID = m_ShaderManager.GetFragmentShaderID(Shader::Fragment::REFLECTION);
  m_hReflection = m_ContextManager.CreateNewContext("Reflection", vID, fID);

  m_ContextManager.SetContext(m_hReflection);

  m_ContextManager.AddNewUniformAttribute(m_hReflection, "pers_matrix");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "view_matrix");

  m_ContextManager.AddNewUniformAttribute(m_hReflection, "cam_position");

  // TODO: get rid of this
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_amb");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_fog");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_fog_near");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_fog_far");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_att1");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_att2");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "global_att3");

  m_ContextManager.AddNewUniformAttribute(m_hReflection, "model_matrix");

  //TODO: get rid of this
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_emit");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_amb");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_dif");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_spc");
  m_ContextManager.AddNewUniformAttribute(m_hReflection, "mat_spc_exp");

  ContextManager::VertexAttribute vaPosition("position", 4, GL_FLOAT, GL_FALSE, sizeof(vec3), 0u);
  ContextManager::VertexAttribute vaNormal("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), sizeof(vec3));
  m_ContextManager.AddNewVertexAttribute(m_hReflection, vaPosition);
  m_ContextManager.AddNewVertexAttribute(m_hReflection, vaNormal);

  Log::Trace("Reflection Context loaded.");
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
