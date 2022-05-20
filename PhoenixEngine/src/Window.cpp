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

Window::Window(WindowProperties Properties) :
  m_WindowPtr(),
  m_WindowProperties(std::move(Properties)),
  m_LastFrameTime(std::chrono::steady_clock::now()),
  m_bWindowShouldClose(false)
{
#pragma region GLFW

  // Close the window if it is already open
  if (m_WindowPtr)
    glfwTerminate();

  // Initialize the library
  if (!glfwInit())
  {
    Log::error("Couldn't initialize OpenGL Window!");
    return;
  }

  m_WindowPtr = glfwCreateWindow(
    m_WindowProperties.Width,
    m_WindowProperties.Height,
    m_WindowProperties.Title.c_str(),
    nullptr, nullptr);

  glfwMakeContextCurrent(m_WindowPtr);

  glfwSwapInterval(1);

  // Verify the Window's creation
  if (!m_WindowPtr)
  {
    glfwTerminate();
    Log::error("OpenGL Window could not be created!");
    return;
  }

  // Make the window's context current
  glfwMakeContextCurrent(m_WindowPtr);

#pragma endregion

  Log::trace("Window created.");

#pragma region GLEW

  // Verify GLEW initialized
  if (glewInit() != GLEW_OK)
  {
    Log::error("Couldn't initialize GLEW!");
    return;
  }

  // Print the version to the logger
  stringstream ss("Initialized OpenGL version: ", SSIO);
  ss << glGetString(GL_VERSION);
  Log::trace(ss.str());

#pragma endregion

  m_SceneManagerPtr = make_unique<SceneManager>();
  m_RendererPtr = make_unique<Renderer>();

#pragma region ImGUI

#ifdef _IMGUI

  // Set up ImGui Close Window Event
  ImGui::MANAGER = make_unique<ImGuiManager>(m_WindowPtr);
  const std::function cbClose = [this] { OnImGuiCloseWindow(); };
  ImGui::MANAGER->SetOnCloseHandler(cbClose);

  // Set up ImGui Change Scene Event
  const std::function cbSceneChange =
    [this](const SceneManager::Scene Scene) { OnImGuiChangeScene(Scene); };
  ImGui::MANAGER->SetOnSceneChangeHandler(cbSceneChange);

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
  const auto delta = 
    std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::steady_clock::now() - m_LastFrameTime).count() / 1000000.f;
  m_LastFrameTime = std::chrono::steady_clock::now();

  // Change scenes
  if (m_SceneManagerPtr->SceneIsTransitioning())
  {
    // Skip drawing this cycle if Scene is transitioning
    return;
  }

  // Poll for input events
  glfwPollEvents();

  // Check Window related input
  OnPollInput();

  // Check input per scene
  m_SceneManagerPtr->OnPollInput(m_WindowPtr, delta);

  // Update the Scene this cycle
  m_SceneManagerPtr->OnUpdate(delta);

  m_RendererPtr->OnBeginFrame();

#pragma region ImGUI

#ifdef _IMGUI
  ImGui::MANAGER->OnImGuiUpdateStart();
#endif // _IMGUI

#pragma endregion

  // Update the Renderer
  m_RendererPtr->RenderScene(
    m_SceneManagerPtr->GetCurrentSceneGameObjects(),
    m_SceneManagerPtr->GetCurrentSceneActiveCamera());

  m_RendererPtr->OnEndFrame();

#pragma region ImGUI

#ifdef _IMGUI
  ImGui::MANAGER->OnImGuiUpdateEnd();
#endif // _IMGUI

#pragma endregion

  // Swap the back/front buffers
  glfwSwapBuffers(m_WindowPtr);
}

void Window::OnClose() const noexcept
{
#pragma region ImGUI

#ifdef _IMGUI
  ImGui::MANAGER->OnImGuiClose();
#endif // _IMGUI

#pragma endregion

  // Shut down the scenes
  m_SceneManagerPtr->Shutdown();

  // Close the window
  glfwTerminate();
}

void Window::OnPollInput() noexcept
{
  if (glfwGetKey(m_WindowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    Log::trace("Esc key pressed. Shutting down.");
    m_bWindowShouldClose = true;
  }
}

bool Window::WindowShouldClose() const noexcept
{
  // Check if the window should be closed
  return m_bWindowShouldClose || glfwWindowShouldClose(m_WindowPtr);
}

#pragma region ImGUI

#ifdef _IMGUI

void Window::OnImGuiCloseWindow() noexcept
{
  m_bWindowShouldClose = true;
}

void Window::OnImGuiChangeScene(const SceneManager::Scene Scene) const
{
  m_SceneManagerPtr->SetNewScene(Scene);
}

#endif // _IMGUI

#pragma endregion
