#include "pch.h"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include <chrono>

#ifdef _IMGUI
//TODO: ImGUI Test Stuff
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#endif // _IMGUI

Window::Window(const WindowProperties& properties) :
  m_pWindow(nullptr),
  m_WindowProperties(properties),
  m_SceneManager(),
  m_MeshRenderer(),
  m_LastFrameTime(std::chrono::steady_clock::now()),
  m_Clock(),
  m_bWindowShouldClose(false)
{
  // Close the window if it is already open
  if (m_pWindow)
    glfwTerminate();

  // Initialize the library
  if (!glfwInit())
  {
    Log::Error("Couldn't initialize OpenGL Window!");
    return;
  }

  // Create a windowed mode window and its OpenGL context
  m_pWindow = glfwCreateWindow(
    m_WindowProperties.Width,
    m_WindowProperties.Height,
    m_WindowProperties.Title.c_str(),
    NULL, NULL);

  // TODO: What does this do?
  glfwMakeContextCurrent(m_pWindow);

  // TODO: VSync?
  glfwSwapInterval(1);

#ifdef _IMGUI
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
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
#endif // _IMGUI

  // Verify the Window's creation
  if (!m_pWindow)
  {
    glfwTerminate();
    Log::Error("OpenGL Window could not be created!");
    return;
  }

  // Make the window's context current
  glfwMakeContextCurrent(m_pWindow);

  Log::Trace("Window created.");

  // Verify GLEW initialized
  if (glewInit() != GLEW_OK)
  {
    Log::Error("Couldn't initialize GLEW!");
    return;
  };

  // Print the version to the logger
  stringstream ss("Initialized OpenGL version: ", SSIO);
  ss << glGetString(GL_VERSION);
  Log::Trace(ss.str());

  // Initialize the Renderer
  m_MeshRenderer.Init();
}

unsigned Window::GetWidth() const noexcept
{
  return m_WindowProperties.Width;
}

unsigned Window::GetHeight() const noexcept
{
  return m_WindowProperties.Height;
}

void Window::OnUpdate() noexcept
{
  // Calculate delta time
  float delta = static_cast<float>(
    std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::steady_clock::now() - m_LastFrameTime).count() / 1000000.f
    );
  m_LastFrameTime = std::chrono::steady_clock::now();

  // Change scenes
  if (m_SceneManager.SceneIsTransitioning())
  {
    // Skip drawing this cycle if Scene is transitioning
    //TODO: consider an OnSceneTransitioned for the Renderer
    return;
  }

  // Poll for input events
  glfwPollEvents();

  // Check Window related input
  OnPollInput(delta);

  // Check input per scene
  m_SceneManager.OnPollInput(m_pWindow, delta);

  // Update the Scene this cycle
  m_SceneManager.OnUpdate(delta);

  // Update the Renderer
  m_MeshRenderer.RenderGameObjects(
    m_SceneManager.GetCurrentSceneGameObjects(),
    m_SceneManager.GetCurrentSceneActiveCamera());

#ifdef _IMGUI
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  bool open = true;
  ImGui::ShowDemoWindow(&open);

  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // _IMGUI

  // Swap the back/front buffers
  glfwSwapBuffers(m_pWindow);
}

void Window::OnClose() noexcept
{
#ifdef _IMGUI
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif // _IMGUI

  // Shut down the scenes
  m_SceneManager.Shutdown();
  // Close the window
  glfwTerminate();
}

void Window::OnPollInput(float dt) noexcept
{
  if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    Log::Trace("Esc key pressed. Shutting down.");
    m_bWindowShouldClose = true;
  }
}

bool Window::WindowShouldClose() noexcept
{
  // Check if the window should be closed
  return m_bWindowShouldClose || glfwWindowShouldClose(m_pWindow);
}
