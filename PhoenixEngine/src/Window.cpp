#include "pch.h"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include <chrono>

#pragma region ImGUI
#ifdef _IMGUI
#include "ImGUIManager.h"
#endif // _IMGUI
#pragma endregion

Window::Window(const WindowProperties& properties) :
  m_pWindow(nullptr),
  m_WindowProperties(properties),
  m_SceneManager(),
  m_ShaderManager(),
  m_ContextManager(),
  m_MeshRenderer(),
  m_LineRenderer(),
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

  //TODO: Move this up to member initializer list
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

  //TODO: Get rid of Init
  // Initialize the Renderer
  m_ShaderManager.Init();
  m_MeshRenderer.Init(m_ShaderManager, m_ContextManager);
  //m_LineRenderer.Init(m_ShaderManager, m_ContextManager);

#pragma region ImGUI

#ifdef _IMGUI

  // Set up ImGui Close Window Event
  ImGui::Manager = make_unique<ImGuiManager>(m_pWindow);
  std::function<void()> cbClose = [=]() { OnImGuiCloseWindow(); };
  ImGui::Manager->SetOnCloseHandler(cbClose);

  // Set up ImGui Change Scene Event
  std::function<void(SceneManager::Scene scene)> cbSceneChange =
    [=](SceneManager::Scene scene) { OnImGuiChangeScene(scene); };
  ImGui::Manager->SetOnSceneChangeHandler(cbSceneChange);

#endif // _IMGUI

#pragma endregion

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

#pragma region ImGUI

#ifdef _IMGUI
  ImGui::Manager->OnImGuiUpdateStart();
#endif // _IMGUI

#pragma endregion

  // Update the Scene this cycle
  m_SceneManager.OnUpdate(delta);

  // Update the Renderer
  m_MeshRenderer.RenderGameObjects(
    m_SceneManager.GetCurrentSceneGameObjects(),
    m_SceneManager.GetCurrentSceneActiveCamera());

  //TODO:
  //m_LineRenderer.RenderLines();

#pragma region ImGUI

#ifdef _IMGUI
  ImGui::Manager->OnImGuiUpdateEnd();
#endif // _IMGUI

#pragma endregion

  // Swap the back/front buffers
  glfwSwapBuffers(m_pWindow);
}

void Window::OnClose() noexcept
{
#pragma region ImGUI

#ifdef _IMGUI
  ImGui::Manager->OnImGuiClose();
#endif // _IMGUI

#pragma endregion

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

#pragma region ImGUI

#ifdef _IMGUI

void Window::OnImGuiCloseWindow() noexcept
{
  m_bWindowShouldClose = true;
}

void Window::OnImGuiChangeScene(SceneManager::Scene scene)
{
  m_SceneManager.SetNewScene(scene);
}

#endif // _IMGUI

#pragma endregion
