//------------------------------------------------------------------------------
// File:    ImGUIManager.cpp
// Author:  Ryan Buehler
// Created: 09/30/21
// Desc:    Interface to the ImGUI system
//------------------------------------------------------------------------------
#include "pch.h"
#pragma warning( push, 0 )
#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#pragma warning( pop )
#include "GLFW/glfw3.h"
#include "DebugRenderer.h"

#define IMGUISPACE ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing()

#pragma region Local Namespace

// For quicker iteration
namespace
{
  static const ImVec4 IMGREEN(0.f, 1.f, 0.f, 1.f);
  static const ImVec4 IMCYAN(0.f, 1.f, 0.5f, 1.f);

  static const ImGui::DemoObject DEMOOBJECTS[(size_t)ImGui::DemoObject::COUNT] =
  {
    ImGui::DemoObject::Bunny,
    ImGui::DemoObject::BunnyHighPoly,
    ImGui::DemoObject::Cube,
    ImGui::DemoObject::Cube2,
    ImGui::DemoObject::Cup,
    ImGui::DemoObject::Lucy,
    ImGui::DemoObject::Quad,
    ImGui::DemoObject::Sphere,
    ImGui::DemoObject::StarWars
  };

  static const char* DEMOOBJECTNAMES[(size_t)ImGui::DemoObject::COUNT] =
  {
    "Bunny",
    "BunnyHighPoly",
    "Cube",
    "Cube2",
    "Cup",
    "Lucy",
    "Quad",
    "Sphere",
    "StarWars"
  };

  static const char* DEMOOBJECTFILENAMES[(size_t)ImGui::DemoObject::COUNT] =
  {
    "bunny.obj",
    "bunny_high_poly.obj",
    "cube.obj",
    "cube2.obj",
    "cup.obj",
    "lucy_princeton.obj",
    "quad.obj",
    "sphere.obj",
    "starwars1.obj"
  };

  static const char* SHADERNAMES[4] =
  {
    "Phong Lighting",
    "Phong Shading",
    "Blinn-Phong",
    "Phong Texture"
  };
}

#pragma endregion

// Instantiation
namespace ImGui
{
  unique_ptr<ImGuiManager> Manager;
  bool GraphicsWindowEnabled = true;
  bool GraphicsDebugRenderVertexNormals = false;
  bool GraphicsDebugRenderSurfaceNormals = false;
  float GraphicsDebugNormalLength = 0.05f;
  bool GraphicsRebuildShaders = false;
  int GraphicsSelectedShader = 1;
  UV::Generation GraphicsSelectedProjection = UV::Generation::PLANAR;

  int SceneScenario = 1;
  bool SceneDrawOrbit = false;
  bool SceneOrbitObjects = true;

  LightingSystem::GlobalLightingData LightingGlobalData;
  Light::Data LightingDataArray[16];
  int LightingCurrentLight = 0;
  int LightingActiveLights = 8;
  Material LightingGlobalMaterial;

  DemoObject DemoObjectMain = DemoObject::Lucy;
  const char* DemoObjectFile = DEMOOBJECTFILENAMES[(size_t)ImGui::DemoObject::Lucy];
}


ImGuiManager::ImGuiManager(GLFWwindow* window) noexcept :
  m_bRenderAxes(false),
  m_DebugLineWidth(1.f)
{
  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiManager::OnImGuiUpdateStart() noexcept
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ShowMainMenu();

  ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

  //static bool dopen = true;
  //ImGui::ShowDemoWindow(&dopen);
}

void ImGuiManager::OnImGuiUpdateEnd() noexcept
{
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::OnImGuiClose() noexcept
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiManager::OnImGuiGraphicsUpdate() noexcept
{
  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
  ImGui::Begin("Graphics Settings", &ImGui::GraphicsWindowEnabled);

  graphicsUpdateStats();

  IMGUISPACE;
  ImGui::Separator();
  IMGUISPACE;

  graphicsUpdateObjects();

  IMGUISPACE;
  ImGui::Separator();
  IMGUISPACE;

  graphicsUpdateLighting();

  IMGUISPACE;
  ImGui::Separator();
  IMGUISPACE;

  graphicsUpdateRendering();

  IMGUISPACE;
  ImGui::Separator();
  IMGUISPACE;

  graphicsUpdateControls();

  ImGui::End();
}

void ImGuiManager::SetOnCloseHandler(std::function<void()> callback)
{
  m_dOnClose = callback;
}

void ImGuiManager::SetOnSceneChangeHandler(function<void(SceneManager::Scene)> callback)
{
  m_dOnSceneChange = callback;
}

void ImGuiManager::SetOnDemoObjectHandler(function<void()> callback)
{
  m_dOnDemoObjectChange = callback;
}

void ImGuiManager::ShowMainMenu() noexcept
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      ShowMainMenu_File();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
      ShowMainMenu_Edit();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About"))
    {
      ShowMainMenu_About();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void ImGuiManager::ShowMainMenu_File() noexcept
{
  if (ImGui::MenuItem("Scene 1", ""))
  {
    m_dOnSceneChange(SceneManager::Scene::Scene1);
  }
  if (ImGui::MenuItem("Scene 2", ""))
  {
    m_dOnSceneChange(SceneManager::Scene::Scene2);
  }
  if (ImGui::MenuItem("Scene Single Object", ""))
  {
    m_dOnSceneChange(SceneManager::Scene::SceneSingleObject);
  }
  if (ImGui::MenuItem("Exit", "Close the Engine"))
  {
    m_dOnClose();
  }
}

void ImGuiManager::ShowMainMenu_Edit() noexcept
{
  if (ImGui::MenuItem("Graphics Window Enabled", "", ImGui::GraphicsWindowEnabled))
  {
    ImGui::GraphicsWindowEnabled = !ImGui::GraphicsWindowEnabled;
  }
}

void ImGuiManager::ShowMainMenu_About() noexcept
{
  if (ImGui::MenuItem("Phoenix Engine", PE_VERSION, false, false)) {}  // Disabled item
  if (ImGui::MenuItem("Author", "Ryan Buehler", false, false)) {}      // Disabled item
}

void ImGuiManager::graphicsUpdateStats() noexcept
{
  ImGui::TextColored(IMCYAN, "Graphics Statistics");
  ImGui::TextColored(IMCYAN, "-------------------");

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "Frame Stats: "); ImGui::SameLine();
  ImGui::Text("Frame: [%05d] Time: %lf", ImGui::GetFrameCount(), ImGui::GetTime());

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "Shader: "); ImGui::SameLine();
  static const char* ShaderString = SHADERNAMES[1];
  if (ImGui::BeginCombo("##Selected Shader", ShaderString))
  {
    for (int i = 0; i < 4; ++i)
    {
      ImGui::PushID((void*)SHADERNAMES[i]);
      if (ImGui::Selectable(SHADERNAMES[i], ImGui::GraphicsSelectedShader == i))
      {
        ShaderString = SHADERNAMES[i];
        ImGui::GraphicsSelectedShader = i;
      }
      ImGui::PopID();
    }

    ImGui::EndCombo();
  }

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "Projection: "); ImGui::SameLine();
  static const char* ProjectString = "Planar";
  if (ImGui::BeginCombo("##Projection", ProjectString))
  {
    ImGui::PushID((void*)"Planar");
    if (ImGui::Selectable("Planar", ImGui::GraphicsSelectedProjection == UV::Generation::PLANAR))
    {
      ProjectString = "Planar";
      ImGui::GraphicsSelectedProjection = UV::Generation::PLANAR;
      m_dOnSceneChange(SceneManager::Scene::Scene2);
      ImGui::PopID();
    }

    ImGui::PushID((void*)"Spherical");
    if (ImGui::Selectable("Spherical", ImGui::GraphicsSelectedProjection == UV::Generation::SPHERICAL))
    {
      ProjectString = "Spherical";
      ImGui::GraphicsSelectedProjection = UV::Generation::SPHERICAL;
      m_dOnSceneChange(SceneManager::Scene::Scene2);
      ImGui::PopID();
    }

    ImGui::PushID((void*)"Cylindrical");
    if (ImGui::Selectable("Cylindrical", ImGui::GraphicsSelectedProjection == UV::Generation::CYLINDRICAL))
    {
      ProjectString = "Cylindrical";
      ImGui::GraphicsSelectedProjection = UV::Generation::CYLINDRICAL;
      m_dOnSceneChange(SceneManager::Scene::Scene2);
      ImGui::PopID();
    }

    ImGui::EndCombo();
  }

  IMGUISPACE;

  if (ImGui::Button("Rebuild Shaders", { 120, 32 }))
  {
    ImGui::GraphicsRebuildShaders = true;
  }
}

void ImGuiManager::graphicsUpdateObjects() noexcept
{
  ImGui::TextColored(IMCYAN, "Global Object Settings");
  ImGui::TextColored(IMCYAN, "----------------------");

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "Demo Object: "); ImGui::SameLine();
  static const char* DemoObjectString = DEMOOBJECTNAMES[(size_t)ImGui::DemoObject::Lucy];
  if (ImGui::BeginCombo("##Demo Object", DemoObjectString))
  {
    for (int i = 0; i < (size_t)ImGui::DemoObject::COUNT; ++i)
    {
      ImGui::PushID((void*)DEMOOBJECTNAMES[i]);
      if (ImGui::Selectable(DEMOOBJECTNAMES[i], ImGui::DemoObjectMain == DEMOOBJECTS[i]))
      {
        ImGui::DemoObjectMain = DEMOOBJECTS[i];
        ImGui::DemoObjectFile = DEMOOBJECTFILENAMES[i];
        DemoObjectString = DEMOOBJECTNAMES[i];
        m_dOnDemoObjectChange();
      }
      ImGui::PopID();
    }

    ImGui::EndCombo();
  }

  ImGui::TextColored(IMGREEN, "Enable Orbiting: "); ImGui::SameLine();
  ImGui::Checkbox("##Enable Orbiting", &ImGui::SceneOrbitObjects);

  if (ImGui::Button("Scenario 1", { 140, 40 }))
  {
    ImGui::SceneScenario = 1;
    m_dOnSceneChange(SceneManager::Scene::Scene2);
  }
  ImGui::SameLine();
  if (ImGui::Button("Scenario 2", { 140, 40 }))
  {
    ImGui::SceneScenario = 2;
    m_dOnSceneChange(SceneManager::Scene::Scene2);
  }
  ImGui::SameLine();
  if (ImGui::Button("Scenario 3", { 140, 40 }))
  {
    ImGui::SceneScenario = 3;
    m_dOnSceneChange(SceneManager::Scene::Scene2);
    ImGui::GraphicsSelectedShader = 3;
    ImGui::DemoObjectMain = DEMOOBJECTS[4];
    ImGui::DemoObjectFile = DEMOOBJECTFILENAMES[4];
  }
}

void ImGuiManager::graphicsUpdateLighting() noexcept
{
  if (ImGui::CollapsingHeader("Lighting"))
  {
    ImGui::TextColored(IMCYAN, "Global Lighting Settings");
    ImGui::TextColored(IMCYAN, "------------------------");

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Global Ambience:   "); ImGui::SameLine();
    ImGui::ColorEdit3("##Global Ambient Intensity", &ImGui::LightingGlobalData.AmbientIntensity[0]);
    ImGui::TextColored(IMGREEN, "Fog Intensity:     "); ImGui::SameLine();
    ImGui::ColorEdit3("##Global Fog Intensity", &ImGui::LightingGlobalData.FogIntensity[0]);

    ImGui::TextColored(IMGREEN, "Fog Near: "); ImGui::SameLine();
    ImGui::SliderFloat("##Global Fog Near", &ImGui::LightingGlobalData.FogNear, 0.f, ImGui::LightingGlobalData.FogFar);
    ImGui::TextColored(IMGREEN, "Fog Far:  "); ImGui::SameLine();
    ImGui::SliderFloat("##Global Fog Far", &ImGui::LightingGlobalData.FogFar, ImGui::LightingGlobalData.FogNear, 50.f);

    ImGui::TextColored(IMGREEN, "Light Attenuation (constant):  "); ImGui::SameLine();
    ImGui::SliderFloat("##Light Attenuation (constant)", &ImGui::LightingGlobalData.AttConstant, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Light Attenuation  (linear):   "); ImGui::SameLine();
    ImGui::SliderFloat("##Light Attenuation (linear)", &ImGui::LightingGlobalData.AttLinear, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Light Attenuation (quadratic): "); ImGui::SameLine();
    ImGui::SliderFloat("##Light Attenuation (quadratic)", &ImGui::LightingGlobalData.AttQuadratic, 0.f, 1.f);

    IMGUISPACE;
    IMGUISPACE;

    ImGui::TextColored(IMCYAN, "Individual Light Settings");
    ImGui::TextColored(IMCYAN, "-------------------------");

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Active Lights:   "); ImGui::SameLine();
    ImGui::SliderInt("##Active Lights", &ImGui::LightingActiveLights, 0, 16);

    ImGui::TextColored(IMGREEN, "Selected Light:  "); ImGui::SameLine();
    ImGui::SliderInt("##Selected Light", &ImGui::LightingCurrentLight, 0, 15);

    Light::Data& lightData = ImGui::LightingDataArray[ImGui::LightingCurrentLight];

    static const char* LightTypeStr = "Point";
    ImGui::TextColored(IMGREEN, "Light Type:      "); ImGui::SameLine();
    if (ImGui::BeginCombo("##Light Type", LightTypeStr))
    {
      ImGui::PushID((void*)"Point");
      if (ImGui::Selectable("Point", lightData.Type == Light::POINT_LIGHT))
      {
        lightData.Type = Light::POINT_LIGHT;
        LightTypeStr = "Point";
      }
      ImGui::PopID();
      ImGui::PushID((void*)"Directional");
      if (ImGui::Selectable("Directional", lightData.Type == Light::DIRECTION_LIGHT))
      {
        lightData.Type = Light::DIRECTION_LIGHT;
        LightTypeStr = "Directional";
      }
      ImGui::PopID();
      ImGui::PushID((void*)"Spotlight");
      if (ImGui::Selectable("Spotlight", lightData.Type == Light::SPOT_LIGHT))
      {
        lightData.Type = Light::SPOT_LIGHT;
        LightTypeStr = "Spotlight";
      }
      ImGui::PopID();
      ImGui::EndCombo();
    }

    if (lightData.Type != Light::POINT_LIGHT)
    {
      ImGui::TextColored(IMGREEN, "Light Direction:     ");
      ImGui::SliderFloat3("##Light Direction", &lightData.Direction[0], -1.f, 1.f);
      if (lightData.Type == Light::SPOT_LIGHT)
      {
        ImGui::TextColored(IMGREEN, "Light Inner Falloff:     ");
        ImGui::SliderFloat("##Light Inner Falloff", &lightData.InnerFalloff, 0.f, 45.f);

        ImGui::TextColored(IMGREEN, "Light Outer Falloff:     ");
        ImGui::SliderFloat("##Light Outer Falloff", &lightData.OuterFalloff, lightData.InnerFalloff, 45.f);
      }
    }

    ImGui::TextColored(IMGREEN, "Ambient Intensity:     ");
    ImGui::ColorEdit4("##Light Ambient Intensity", &lightData.AmbientIntensity[0]);
    ImGui::TextColored(IMGREEN, "Diffuse Intensity:     ");
    ImGui::ColorEdit4("##Light Diffuse Intensity", &lightData.DiffuseIntensity[0]);
    ImGui::TextColored(IMGREEN, "Specular Intensity:     ");
    ImGui::ColorEdit4("##Light Specular Intensity", &lightData.SpecularIntensity[0]);

    IMGUISPACE;
    IMGUISPACE;

    ImGui::TextColored(IMCYAN, "Global Material Settings");
    ImGui::TextColored(IMCYAN, "------------------------");

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Material Emission:     ");
    ImGui::ColorEdit3("##Material Emission", &ImGui::LightingGlobalMaterial.m_Emissive[0]);
    ImGui::TextColored(IMGREEN, "Material Ambient:      "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Ambient", &ImGui::LightingGlobalMaterial.m_AmbientFactor, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Material Diffuse:      "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Diffuse", &ImGui::LightingGlobalMaterial.m_DiffuseFactor, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Material Specular:     "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Specular", &ImGui::LightingGlobalMaterial.m_SpecularFactor, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Material Specular Exp: "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Specular Exp", &ImGui::LightingGlobalMaterial.m_SpecularExp, 1.f, 1000.f);
  }
}

void ImGuiManager::graphicsUpdateRendering() noexcept
{
  if (ImGui::CollapsingHeader("Rendering"))
  {
    ImGui::TextColored(IMCYAN, "Renderer Settings");
    ImGui::TextColored(IMCYAN, "-----------------");

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Render Axes: "); ImGui::SameLine();
    ImGui::Checkbox("##Render Axes", &m_bRenderAxes);
    if (m_bRenderAxes)
    {
      DebugRenderer::I().AddLine(vec3(-10000.f, 0.f, 0.f), Colors::RED, vec3(10000.f, 0.f, 0.f), Colors::RED);
      DebugRenderer::I().AddLine(vec3(0.f, -10000.f, 0.f), Colors::GREEN, vec3(0.f, 10000.f, 0.f), Colors::GREEN);
      DebugRenderer::I().AddLine(vec3(0.f, 0.f, -10000.f), Colors::BLUE, vec3(0.f, 0.f, 10000.f), Colors::BLUE);
    }

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Debug Line Width: "); ImGui::SameLine();
    if (ImGui::SliderFloat("##Debug Line Width", &m_DebugLineWidth, 0.05f, 5.f))
    {
      DebugRenderer::I().SetLineWidth(m_DebugLineWidth);
    }

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Show Normals: "); ImGui::SameLine();

    static int imguiNormals = 2;
    if (ImGui::RadioButton("Per Vertex", &imguiNormals, 0))
    {
      ImGui::GraphicsDebugRenderVertexNormals = true;
      ImGui::GraphicsDebugRenderSurfaceNormals = false;
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("Per Triangle", &imguiNormals, 1))
    {
      ImGui::GraphicsDebugRenderVertexNormals = false;
      ImGui::GraphicsDebugRenderSurfaceNormals = true;
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("None", &imguiNormals, 2))
    {
      ImGui::GraphicsDebugRenderVertexNormals = false;
      ImGui::GraphicsDebugRenderSurfaceNormals = false;
    }

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Normal Length: "); ImGui::SameLine();
    ImGui::SliderFloat("##Normal Length", &ImGui::GraphicsDebugNormalLength, 0.001f, 0.5f);

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Render Mode: "); ImGui::SameLine();
    static int imguiRenderMode = 0;
    if (ImGui::RadioButton("Fill", &imguiRenderMode, 0))
    {
      Renderer::SetRenderModeFill();
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("Wireframe", &imguiRenderMode, 1))
    {
      Renderer::SetRenderModeWireframe();
    }
  }
}

void ImGuiManager::graphicsUpdateControls() noexcept
{
  ImGui::TextColored(IMCYAN, "Engine Camera Controls");
  ImGui::TextColored(IMCYAN, "----------------------");

  IMGUISPACE;

  ImGui::Text("Camera Controls:");
  ImGui::Text("WASD: Move Around");
  ImGui::Text("Q/E: Move Up and Down");
  ImGui::Text("Note: Camera currently focuses on the center object.");
}

#undef IMGUISPACE