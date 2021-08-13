#include "pch.h"
#include "GLEW/glew.h"
#include "Window.h"
//TODO:
#include "VertexArrayObject.h"

Window::Window(const WindowProperties& properties) :
  m_pWindow(nullptr),
  m_WindowProperties(properties)
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

  //TODO:
  vao = make_unique<VertexArrayObject>(Graphics::DataUsage::STATIC);
  vao->AddVertex(-0.5f, -0.5f, 0.f);
  vao->AddVertex(0.f, 0.f, 0.f);
  vao->AddVertex(0.5f, -0.5f, 0.f);
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
    // Clear the back buffer
    glClear(GL_COLOR_BUFFER_BIT);

    //TODO:
    vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap the back/front buffers
    glfwSwapBuffers(m_pWindow);

    // Clear the bound buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0u);

    // Poll for events
    glfwPollEvents();
}

void Window::OnClose() noexcept
{
  glfwTerminate();
}