//------------------------------------------------------------------------------
// File:    ImGUIManager.cpp
// Author:  Ryan Buehler
// Created: 09/30/21
// Desc:    Interface to the ImGUI system
//------------------------------------------------------------------------------
// ReSharper disable CppCStyleCast
#include "pch.h"
#pragma warning( push, 0 )
#include "ImGUIManager.h"

#include <utility>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#pragma warning( pop )
#include "GLFW/glfw3.h"
#include "DebugRenderer.h"
#include "Colors.h"

#define IMGUISPACE ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing()

#pragma region Local Namespace

// For quicker iteration
namespace
{
  const ImVec4 IMGREEN(0.f, 1.f, 0.f, 1.f);
  const ImVec4 IMCYAN(0.f, 1.f, 0.5f, 1.f);

  const ImGui::DemoObject DEMOOBJECTS[static_cast<size_t>(ImGui::DemoObject::COUNT)] =
  {
    ImGui::DemoObject::BUNNY,
    ImGui::DemoObject::BUNNY_HIGH_POLY,
    //ImGui::DemoObject::Cube,
    ImGui::DemoObject::CUBE2,
    ImGui::DemoObject::CUP,
    ImGui::DemoObject::LUCY,
    ImGui::DemoObject::QUAD,
    ImGui::DemoObject::SPHERE,
    ImGui::DemoObject::STAR_WARS
  };

  const char* DEMOOBJECTNAMES[static_cast<size_t>(ImGui::DemoObject::COUNT)] =
  {
    "Bunny",
    "BunnyHighPoly",
    //"Cube",
    "Cube2",
    "Cup",
    "Lucy",
    "Quad",
    "Sphere",
    "StarWars",
    "Backpack"
  };

  const char* DEMOOBJECTFILENAMES[static_cast<size_t>(ImGui::DemoObject::COUNT)] =
  {
    "bunny.obj",
    "bunny_high_poly.obj",
    //"cube.obj",
    "cube2.obj",
    "cup.obj",
    "lucy_princeton.obj",
    "quad.obj",
    "sphere.obj",
    "starwars1.obj",
    "backpack.fbx"
  };

  const char* SHADERNAMES[1] =
  {
    //"Phong Lighting",
    //"Phong Shading",
    "Blinn-Phong",
    //"Reflect/Refract"
  };
}

#pragma endregion

// Instantiation
namespace ImGui
{
  unique_ptr<ImGuiManager> MANAGER;
  bool GRAPHICS_WINDOW_ENABLED = true;
  bool GRAPHICS_DEBUG_RENDER_VERTEX_NORMALS = false;
  bool GRAPHICS_DEBUG_RENDER_SURFACE_NORMALS = false;
  float GRAPHICS_DEBUG_NORMAL_LENGTH = 0.05f;
  float GRAPHICS_FPS = 0.0f;
  bool GRAPHICS_REBUILD_SHADERS = false;
  bool GRAPHICS_REBUILD_MESHES = false;
  int GRAPHICS_SELECTED_SHADER = 3;
  UV::Generation GRAPHICS_SELECTED_PROJECTION = UV::Generation::PLANAR;
  GLuint GRAPHICS_DISPLAY_TEXTURE[6] =
  {
    Error::INVALID_INDEX,
    Error::INVALID_INDEX,
    Error::INVALID_INDEX,
    Error::INVALID_INDEX,
    Error::INVALID_INDEX,
    Error::INVALID_INDEX
  };
  //float GraphicsRefractSlider = 1.f;
  //float GraphicsRedIOR = 2.413f;
  //float GraphicsGreenIOR = 2.420f;
  //float GraphicsBlueIOR = 2.427f;
  //bool GraphicsRefractEnabled = true;
  //bool GraphicsReflectEnabled = true;

  int SCENE_SCENARIO = 2;
  bool SCENE_DRAW_ORBIT = false;
  bool SCENE_ORBIT_OBJECTS = true;

  LightingSystem::GlobalLightingData LIGHTING_GLOBAL_DATA;
  Light::Data LIGHTING_DATA_ARRAY[16];
  int LIGHTING_CURRENT_LIGHT = 0;
  int LIGHTING_ACTIVE_LIGHTS = 8;
  Material LIGHTING_GLOBAL_MATERIAL;

  DemoObject DEMO_OBJECT_MAIN = DemoObject::LUCY;
  const char* DEMO_OBJECT_FILE = DEMOOBJECTFILENAMES[static_cast<size_t>(DemoObject::LUCY)];
}

ImGuiManager::ImGuiManager(GLFWwindow* Window) noexcept :
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
  const char* glslVersion = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backend
  ImGui_ImplGlfw_InitForOpenGL(Window, true);
  ImGui_ImplOpenGL3_Init(glslVersion);
}

void ImGuiManager::OnImGuiUpdateStart() noexcept
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ShowMainMenu();

  ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

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
  ImGui::Begin("Graphics Settings", &ImGui::GRAPHICS_WINDOW_ENABLED);

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

  IMGUISPACE;
  ImGui::Separator();
  IMGUISPACE;

  graphicsUpdateTexture();

  ImGui::End();
}

void ImGuiManager::SetOnCloseHandler(std::function<void()> Callback)
{
  m_dOnClose = std::move(Callback);
}

void ImGuiManager::SetOnSceneChangeHandler(function<void(SceneManager::Scene)> Callback)
{
  m_dOnSceneChange = std::move(Callback);
}

void ImGuiManager::SetOnDemoObjectHandler(function<void()> Callback)
{
  m_dOnDemoObjectChange = std::move(Callback);
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

void ImGuiManager::ShowMainMenu_File() const noexcept
{
  if (ImGui::MenuItem("Demo Scene", ""))
  {
    m_dOnSceneChange(SceneManager::Scene::DEMO);
  }
  if (ImGui::MenuItem("Single Object", ""))
  {
    m_dOnSceneChange(SceneManager::Scene::SINGLE_OBJECT);
  }
  if (ImGui::MenuItem("Close", "Close the Engine"))
  {
    m_dOnClose();
  }
}

void ImGuiManager::ShowMainMenu_Edit() const noexcept
{
  if (ImGui::MenuItem("Graphics Window Enabled", "", ImGui::GRAPHICS_WINDOW_ENABLED))
  {
    ImGui::GRAPHICS_WINDOW_ENABLED = !ImGui::GRAPHICS_WINDOW_ENABLED;
  }
}

void ImGuiManager::ShowMainMenu_About() const noexcept
{
  if (ImGui::MenuItem("Phoenix Engine", PE_VERSION, false, false)) {}  // Disabled item
  if (ImGui::MenuItem("Author", "Ryan Buehler", false, false)) {}      // Disabled item
}

void ImGuiManager::graphicsUpdateStats() const noexcept
{
  ImGui::TextColored(IMCYAN, "Graphics Statistics");
  ImGui::TextColored(IMCYAN, "-------------------");

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "Frame Stats: "); ImGui::SameLine();
  ImGui::Text("Frame: [%05d] Time: %lf", ImGui::GetFrameCount(), ImGui::GetTime());

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "FPS: "); ImGui::SameLine();
  ImGui::Text("[%.2f]", ImGui::GRAPHICS_FPS);

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "Shader: "); ImGui::SameLine();
  static const char* ShaderString = SHADERNAMES[0];
  if (ImGui::BeginCombo("##Selected Shader", ShaderString))
  {
    for (int i = 0; i < std::size(SHADERNAMES); ++i)
    {
      ImGui::PushID((void*)SHADERNAMES[i]);
      if (ImGui::Selectable(SHADERNAMES[i], ImGui::GRAPHICS_SELECTED_SHADER == i))
      {
        ShaderString = SHADERNAMES[i];
        ImGui::GRAPHICS_SELECTED_SHADER = i;
      }
      ImGui::PopID();
    }

    ImGui::EndCombo();
  }

  //  IMGUISPACE;
  //
  //  ImGui::TextColored(IMGREEN, "Refraction Enabled: "); ImGui::SameLine();
  //  ImGui::Checkbox("##Refraction Enabled", &ImGui::GraphicsRefractEnabled);
  //
  //  ImGui::TextColored(IMGREEN, "Reflection Enabled: "); ImGui::SameLine();
  //  ImGui::Checkbox("##Reflection Enabled", &ImGui::GraphicsReflectEnabled);
  //
  //  IMGUISPACE;
  //
  //  ImGui::TextColored(IMGREEN, "Reflective Slider: "); ImGui::SameLine();
  //  ImGui::SliderFloat("##Reflective Slider", &ImGui::GraphicsRefractSlider, 0.f, 1.f);
  //
  //  IMGUISPACE;
  //
  //
  //#pragma region IOR Presets
  //
  //  ImGui::TextColored(IMGREEN, "IOR [PRESETS]: "); ImGui::SameLine();
  //  static const char* PresetIORs = "Diamond";
  //  static const float RF = 0.996f;
  //  static const float BF = 1.004f;
  //  if (ImGui::BeginCombo("##IOR [PRESETS]: ", PresetIORs))
  //  {
  //    ImGui::PushID((void*)"Air");
  //    if (ImGui::Selectable("Air", PresetIORs == "Air"))
  //    {
  //      PresetIORs = "Air";
  //      ImGui::GraphicsRedIOR = 1.000293f * RF;
  //      ImGui::GraphicsGreenIOR = 1.000293f;
  //      ImGui::GraphicsBlueIOR = 1.000293f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::PushID((void*)"Hydrogen");
  //    if (ImGui::Selectable("Hydrogen", PresetIORs == "Hydrogen"))
  //    {
  //      PresetIORs = "Hydrogen";
  //      ImGui::GraphicsRedIOR = 1.000132f * RF;
  //      ImGui::GraphicsGreenIOR = 1.000132f;
  //      ImGui::GraphicsBlueIOR = 1.000132f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::PushID((void*)"Water");
  //    if (ImGui::Selectable("Water", PresetIORs == "Water"))
  //    {
  //      PresetIORs = "Water";
  //      ImGui::GraphicsRedIOR = 1.333f * RF;
  //      ImGui::GraphicsGreenIOR = 1.333f;
  //      ImGui::GraphicsBlueIOR = 1.333f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::PushID((void*)"Olive Oil");
  //    if (ImGui::Selectable("Olive Oil", PresetIORs == "Olive Oil"))
  //    {
  //      PresetIORs = "Olive Oil";
  //      ImGui::GraphicsRedIOR = 1.47f * RF;
  //      ImGui::GraphicsGreenIOR = 1.47f;
  //      ImGui::GraphicsBlueIOR = 1.47f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::PushID((void*)"Ice");
  //    if (ImGui::Selectable("Ice", PresetIORs == "Ice"))
  //    {
  //      PresetIORs = "Ice";
  //      ImGui::GraphicsRedIOR = 1.31f * RF;
  //      ImGui::GraphicsGreenIOR = 1.31f;
  //      ImGui::GraphicsBlueIOR = 1.31f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::PushID((void*)"Quartz");
  //    if (ImGui::Selectable("Quartz", PresetIORs == "Quartz"))
  //    {
  //      PresetIORs = "Quartz";
  //      ImGui::GraphicsRedIOR = 1.46f * RF;
  //      ImGui::GraphicsGreenIOR = 1.46f;
  //      ImGui::GraphicsBlueIOR = 1.46f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::PushID((void*)"Diamond");
  //    if (ImGui::Selectable("Diamond", PresetIORs == "Diamond"))
  //    {
  //      PresetIORs = "Diamond";
  //      ImGui::GraphicsRedIOR = 2.42f * RF;
  //      ImGui::GraphicsGreenIOR = 2.42f;
  //      ImGui::GraphicsBlueIOR = 2.42f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::PushID((void*)"Acrylic");
  //    if (ImGui::Selectable("Acrylic", PresetIORs == "Acrylic"))
  //    {
  //      PresetIORs = "Acrylic";
  //      ImGui::GraphicsRedIOR = 1.49f * RF;
  //      ImGui::GraphicsGreenIOR = 1.49f;
  //      ImGui::GraphicsBlueIOR = 1.49f * BF;
  //    }
  //    ImGui::PopID();
  //
  //    ImGui::EndCombo();
  //  }
  //
  //  ImGui::NewLine();
  //#pragma endregion
  //
  //  ImGui::TextColored(IMGREEN, "IOR [MASTER]: "); ImGui::SameLine();
  //  static float IORmaster = 1.f;
  //  
  //  if (ImGui::DragFloat("##IOR [MASTER]", &IORmaster, 0.01f, 1.f, 100.f))
  //  {
  //    ImGui::GraphicsRedIOR = IORmaster * 0.998f;
  //    ImGui::GraphicsGreenIOR = IORmaster * 1.f;
  //    ImGui::GraphicsBlueIOR = IORmaster * 1.002f;
  //  }
  //
  //  IMGUISPACE;
  //
  //
  //  ImGui::TextColored(IMGREEN, "IOR [Red]: "); ImGui::SameLine();
  //  ImGui::SliderFloat("##IOR [Red]", &ImGui::GraphicsRedIOR, 1.f, 100.f);
  //
  //  ImGui::TextColored(IMGREEN, "IOR [Green]: "); ImGui::SameLine();
  //  ImGui::SliderFloat("##IOR [Green]", &ImGui::GraphicsGreenIOR, 1.f, 100.f);
  //
  //  ImGui::TextColored(IMGREEN, "IOR [Blue]: "); ImGui::SameLine();
  //  ImGui::SliderFloat("##IOR [Blue]", &ImGui::GraphicsBlueIOR, 1.f, 100.f);

    //ImGui::TextColored(IMGREEN, "Projection: "); ImGui::SameLine();
    //static const char* ProjectString = "Planar";
    //if (ImGui::BeginCombo("##Projection", ProjectString))
    //{
    //  ImGui::PushID((void*)"Planar");
    //  if (ImGui::Selectable("Planar", ImGui::GraphicsSelectedProjection == UV::Generation::PLANAR))
    //  {
    //    ProjectString = "Planar";
    //    ImGui::GraphicsSelectedProjection = UV::Generation::PLANAR;
    //    ImGui::GraphicsRebuildMeshes = true;
    //    m_dOnSceneChange(SceneManager::Scene::SceneDemo);
    //  }
    //  ImGui::PopID();

    //  ImGui::PushID((void*)"Spherical");
    //  if (ImGui::Selectable("Spherical", ImGui::GraphicsSelectedProjection == UV::Generation::SPHERICAL))
    //  {
    //    ProjectString = "Spherical";
    //    ImGui::GraphicsSelectedProjection = UV::Generation::SPHERICAL;
    //    ImGui::GraphicsRebuildMeshes = true;
    //    m_dOnSceneChange(SceneManager::Scene::SceneDemo);
    //  }
    //  ImGui::PopID();

    //  ImGui::PushID((void*)"Cylindrical");
    //  if (ImGui::Selectable("Cylindrical", ImGui::GraphicsSelectedProjection == UV::Generation::CYLINDRICAL))
    //  {
    //    ProjectString = "Cylindrical";
    //    ImGui::GraphicsSelectedProjection = UV::Generation::CYLINDRICAL;
    //    ImGui::GraphicsRebuildMeshes = true;
    //    m_dOnSceneChange(SceneManager::Scene::SceneDemo);
    //  }
    //  ImGui::PopID();

    //  ImGui::EndCombo();
    //}

    //IMGUISPACE;

  if (ImGui::Button("Rebuild Shaders", { 120, 32 }))
  {
    ImGui::GRAPHICS_REBUILD_SHADERS = true;
  }
}

void ImGuiManager::graphicsUpdateObjects() const noexcept
{
  ImGui::TextColored(IMCYAN, "Global Object Settings");
  ImGui::TextColored(IMCYAN, "----------------------");

  IMGUISPACE;

  ImGui::TextColored(IMGREEN, "Demo Object: "); ImGui::SameLine();
  static const char* DemoObjectString = DEMOOBJECTNAMES[static_cast<size_t>(ImGui::DEMO_OBJECT_MAIN)];
  if (ImGui::BeginCombo("##Demo Object", DemoObjectString))
  {
    for (int i = 0; i < static_cast<size_t>(ImGui::DemoObject::COUNT); ++i)
    {
      ImGui::PushID((void*)DEMOOBJECTNAMES[i]);
      if (ImGui::Selectable(DEMOOBJECTNAMES[i], ImGui::DEMO_OBJECT_MAIN == DEMOOBJECTS[i]))
      {
        ImGui::DEMO_OBJECT_MAIN = DEMOOBJECTS[i];
        ImGui::DEMO_OBJECT_FILE = DEMOOBJECTFILENAMES[i];
        DemoObjectString = DEMOOBJECTNAMES[i];
        m_dOnDemoObjectChange();
      }
      ImGui::PopID();
    }

    ImGui::EndCombo();
  }

  ImGui::TextColored(IMGREEN, "Enable Orbiting: "); ImGui::SameLine();
  ImGui::Checkbox("##Enable Orbiting", &ImGui::SCENE_ORBIT_OBJECTS);

  if (ImGui::Button("Scenario 1", { 140, 40 }))
  {
    ImGui::SCENE_SCENARIO = 1;
    m_dOnSceneChange(SceneManager::Scene::DEMO);
  }
  ImGui::SameLine();
  if (ImGui::Button("Scenario 2", { 140, 40 }))
  {
    ImGui::SCENE_SCENARIO = 2;
    m_dOnSceneChange(SceneManager::Scene::DEMO);
  }
  //ImGui::SameLine();
  //if (ImGui::Button("Scenario 3", { 140, 40 }))
  //{
  //  ImGui::SceneScenario = 3;
  //  m_dOnSceneChange(SceneManager::Scene::SceneDemo);
  //  ImGui::GraphicsSelectedShader = 3;
  //  ImGui::DemoObjectMain = DEMOOBJECTS[4];
  //  ImGui::DemoObjectFile = DEMOOBJECTFILENAMES[4];
  //}
}

void ImGuiManager::graphicsUpdateLighting() const noexcept
{
  if (ImGui::CollapsingHeader("Lighting"))
  {
    ImGui::TextColored(IMCYAN, "Global Lighting Settings");
    ImGui::TextColored(IMCYAN, "------------------------");

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Global Ambience:   "); ImGui::SameLine();
    ImGui::ColorEdit3("##Global Ambient Intensity", &ImGui::LIGHTING_GLOBAL_DATA.AmbientIntensity[0]);
    ImGui::TextColored(IMGREEN, "Fog Intensity:     "); ImGui::SameLine();
    ImGui::ColorEdit3("##Global Fog Intensity", &ImGui::LIGHTING_GLOBAL_DATA.FogIntensity[0]);

    ImGui::TextColored(IMGREEN, "Fog Near: "); ImGui::SameLine();
    ImGui::SliderFloat("##Global Fog Near", &ImGui::LIGHTING_GLOBAL_DATA.FogNear, 0.f, ImGui::LIGHTING_GLOBAL_DATA.FogFar);
    ImGui::TextColored(IMGREEN, "Fog Far:  "); ImGui::SameLine();
    ImGui::SliderFloat("##Global Fog Far", &ImGui::LIGHTING_GLOBAL_DATA.FogFar, ImGui::LIGHTING_GLOBAL_DATA.FogNear, 50.f);

    ImGui::TextColored(IMGREEN, "Light Attenuation (constant):  "); ImGui::SameLine();
    ImGui::SliderFloat("##Light Attenuation (constant)", &ImGui::LIGHTING_GLOBAL_DATA.AttConstant, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Light Attenuation  (linear):   "); ImGui::SameLine();
    ImGui::SliderFloat("##Light Attenuation (linear)", &ImGui::LIGHTING_GLOBAL_DATA.AttLinear, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Light Attenuation (quadratic): "); ImGui::SameLine();
    ImGui::SliderFloat("##Light Attenuation (quadratic)", &ImGui::LIGHTING_GLOBAL_DATA.AttQuadratic, 0.f, 1.f);

    IMGUISPACE;
    IMGUISPACE;

    ImGui::TextColored(IMCYAN, "Individual Light Settings");
    ImGui::TextColored(IMCYAN, "-------------------------");

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Active Lights:   "); ImGui::SameLine();
    ImGui::SliderInt("##Active Lights", &ImGui::LIGHTING_ACTIVE_LIGHTS, 0, 16);

    ImGui::TextColored(IMGREEN, "Selected Light:  "); ImGui::SameLine();
    ImGui::SliderInt("##Selected Light", &ImGui::LIGHTING_CURRENT_LIGHT, 0, 15);

    Light::Data& lightData = ImGui::LIGHTING_DATA_ARRAY[ImGui::LIGHTING_CURRENT_LIGHT];

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
    ImGui::ColorEdit3("##Material Emission", &ImGui::LIGHTING_GLOBAL_MATERIAL.m_Emissive[0]);
    ImGui::TextColored(IMGREEN, "Material Ambient:      "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Ambient", &ImGui::LIGHTING_GLOBAL_MATERIAL.m_AmbientFactor, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Material Diffuse:      "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Diffuse", &ImGui::LIGHTING_GLOBAL_MATERIAL.m_DiffuseFactor, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Material Specular:     "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Specular", &ImGui::LIGHTING_GLOBAL_MATERIAL.m_SpecularFactor, 0.f, 1.f);
    ImGui::TextColored(IMGREEN, "Material Specular Exp: "); ImGui::SameLine();
    ImGui::SliderFloat("##Material Specular Exp", &ImGui::LIGHTING_GLOBAL_MATERIAL.m_SpecularExp, 1.f, 1000.f);
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
      ImGui::GRAPHICS_DEBUG_RENDER_VERTEX_NORMALS = true;
      ImGui::GRAPHICS_DEBUG_RENDER_SURFACE_NORMALS = false;
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("Per Triangle", &imguiNormals, 1))
    {
      ImGui::GRAPHICS_DEBUG_RENDER_VERTEX_NORMALS = false;
      ImGui::GRAPHICS_DEBUG_RENDER_SURFACE_NORMALS = true;
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("None", &imguiNormals, 2))
    {
      ImGui::GRAPHICS_DEBUG_RENDER_VERTEX_NORMALS = false;
      ImGui::GRAPHICS_DEBUG_RENDER_SURFACE_NORMALS = false;
    }

    IMGUISPACE;

    ImGui::TextColored(IMGREEN, "Normal Length: "); ImGui::SameLine();
    ImGui::SliderFloat("##Normal Length", &ImGui::GRAPHICS_DEBUG_NORMAL_LENGTH, 0.001f, 0.5f);

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
  ImGui::Text("F: Focus on the Demo Object");
  ImGui::Text("Note: Camera currently focuses on the center object.");
}

void ImGuiManager::graphicsUpdateTexture() noexcept
{
  for (int i = 0; i < 6; ++i)
  {
    if (ImGui::GRAPHICS_DISPLAY_TEXTURE[i] != Error::INVALID_INDEX)
    {
      if ((i + 3) % 3 != 0)
      {
        ImGui::SameLine();
      }
      ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(ImGui::GRAPHICS_DISPLAY_TEXTURE[i])),  // NOLINT(performance-no-int-to-ptr)
        ImVec2(128, 128));
    }
  }
}

#undef IMGUISPACE