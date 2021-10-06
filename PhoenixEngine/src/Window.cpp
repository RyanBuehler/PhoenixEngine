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
  m_WindowPtr(),
  m_WindowProperties(properties),
  m_SceneManagerPtr(),
  m_RendererPtr(),
  m_LastFrameTime(std::chrono::steady_clock::now()),
  m_Clock(),
  m_bWindowShouldClose(false)
{
#pragma region GLFW

  // Close the window if it is already open
  if (m_WindowPtr)
    glfwTerminate();

  // Initialize the library
  if (!glfwInit())
  {
    Log::Error("Couldn't initialize OpenGL Window!");
    return;
  }

  m_WindowPtr = glfwCreateWindow(
    m_WindowProperties.Width,
    m_WindowProperties.Height,
    m_WindowProperties.Title.c_str(),
    NULL, NULL);

  // TODO: What does this do?
  glfwMakeContextCurrent(m_WindowPtr);

  // TODO: VSync?
  glfwSwapInterval(1);

  // Verify the Window's creation
  if (!m_WindowPtr)
  {
    glfwTerminate();
    Log::Error("OpenGL Window could not be created!");
    return;
  }

  // Make the window's context current
  glfwMakeContextCurrent(m_WindowPtr);

#pragma endregion

  Log::Trace("Window created.");

#pragma region GLEW

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

#pragma endregion

  m_SceneManagerPtr = make_unique<SceneManager>();
  m_RendererPtr = make_unique<Renderer>();

#pragma region ImGUI

#ifdef _IMGUI

  // Set up ImGui Close Window Event
  ImGui::Manager = make_unique<ImGuiManager>(m_WindowPtr);
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
  if (m_SceneManagerPtr->SceneIsTransitioning())
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
  m_SceneManagerPtr->OnPollInput(m_WindowPtr, delta);

  // Update the Scene this cycle
  m_SceneManagerPtr->OnUpdate(delta);

  m_RendererPtr->OnBeginFrame();

#pragma region ImGUI

#ifdef _IMGUI
  ImGui::Manager->OnImGuiUpdateStart();
#endif // _IMGUI

#pragma endregion

  // Update the Renderer
  m_RendererPtr->RenderGameObjects(
    m_SceneManagerPtr->GetCurrentSceneGameObjects(),
    m_SceneManagerPtr->GetCurrentSceneActiveCamera());

  m_RendererPtr->OnEndFrame();

#pragma region ImGUI

#ifdef _IMGUI
  ImGui::Manager->OnImGuiUpdateEnd();
#endif // _IMGUI

#pragma endregion


  // Swap the back/front buffers
  glfwSwapBuffers(m_WindowPtr);
}

void Window::OnClose() noexcept
{
#pragma region ImGUI

#ifdef _IMGUI
  ImGui::Manager->OnImGuiClose();
#endif // _IMGUI

#pragma endregion

  // Shut down the scenes
  m_SceneManagerPtr->Shutdown();
  // Close the window
  glfwTerminate();
}

void Window::OnPollInput(float dt) noexcept
{
  if (glfwGetKey(m_WindowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    Log::Trace("Esc key pressed. Shutting down.");
    m_bWindowShouldClose = true;
  }
}

bool Window::WindowShouldClose() noexcept
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

void Window::OnImGuiChangeScene(SceneManager::Scene scene)
{
  m_SceneManagerPtr->SetNewScene(scene);
}

#endif // _IMGUI

#pragma endregion
