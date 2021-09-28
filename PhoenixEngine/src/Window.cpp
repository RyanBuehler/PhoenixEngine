#include "pch.h"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include <chrono>

Window::Window(const WindowProperties& properties) :
  m_pWindow(nullptr),
  m_WindowProperties(properties),
  m_SceneManager(),
  m_MeshRenderer(),
  m_LastFrameTime(std::chrono::high_resolution_clock::now()),
  m_Clock()
{
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
    std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::steady_clock::now() - m_LastFrameTime).count() / 1000.f
    );
  m_LastFrameTime = std::chrono::steady_clock::now();

  // Change scenes
  if (m_SceneManager.SceneIsTransitioning())
  {
    // Skip drawing this cycle if Scene is transitioning
    //TODO: consider an OnSceneTransitioned for the Renderer
    return;
  }

  // Update the Scene this cycle
  m_SceneManager.OnUpdate(delta);

  // Update the Renderer
  m_MeshRenderer.RenderFrame();

  // Swap the back/front buffers
  glfwSwapBuffers(m_pWindow);

  // Poll for events
  glfwPollEvents();
}

void Window::OnClose() noexcept
{
  glfwTerminate();
}

bool Window::WindowShouldClose() noexcept
{
  return glfwWindowShouldClose(m_pWindow);
}

float Window::calculateDelta() noexcept
{
  return 0.0f;
}
