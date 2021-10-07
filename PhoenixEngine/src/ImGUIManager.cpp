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

// Instantiation
namespace ImGui
{
  unique_ptr<ImGuiManager> Manager;
  bool GraphicsWindowEnabled = true;
  bool GraphicsDebugRenderVertexNormals = false;
  bool GraphicsDebugRenderSurfaceNormals = false;
  float GraphicsDebugNormalLength = 0.05f;
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
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
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

  ImGui::Text("Frame Stats");
  ImGui::Text("Frame: [%05d] Time: %lf", ImGui::GetFrameCount(), ImGui::GetTime());
  ImGui::Separator();
  IMGUISPACE;

  ImGui::Checkbox("Render Axes", &m_bRenderAxes);
  if(m_bRenderAxes)
  {
    DebugRenderer::I().AddLine(vec3(-10000.f, 0.f, 0.f), Colors::RED, vec3(10000.f, 0.f, 0.f), Colors::RED);
    DebugRenderer::I().AddLine(vec3(0.f, -10000.f, 0.f), Colors::GREEN, vec3(0.f, 10000.f, 0.f), Colors::GREEN);
    DebugRenderer::I().AddLine(vec3(0.f, 0.f, -10000.f), Colors::BLUE, vec3(0.f, 0.f, 10000.f), Colors::BLUE);
  }

  IMGUISPACE;

  if (ImGui::SliderFloat("Debug Line Width", &m_DebugLineWidth, 0.05f, 10.f))
  {
    DebugRenderer::I().SetLineWidth(m_DebugLineWidth);
  }

  IMGUISPACE;

  ImGui::Text("Show Normals");
  static int imguiNormals = 0;
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

  ImGui::SliderFloat("Normal Length", &ImGui::GraphicsDebugNormalLength, 0.001f, 1.f);

  IMGUISPACE;

  if (ImGui::SliderFloat("Render Mode", &m_DebugLineWidth, 0.05f, 10.f))
  {
    DebugRenderer::I().SetLineWidth(m_DebugLineWidth);
  }

  IMGUISPACE;

  ImGui::Text("Render Mode");
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
  if (ImGui::MenuItem("Test Scene", ""))
  {
    m_dOnSceneChange(SceneManager::Scene::TestScene);
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
  if (ImGui::MenuItem("Author", "Ryan Buehler", false, false)) {}  // Disabled item
}

#undef IMGUISPACE