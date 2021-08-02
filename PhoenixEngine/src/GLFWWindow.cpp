#include "pch.h"
#include "GLFWWindow.h"

GLFWWindow::GLFWWindow(const WindowProperties& properties) :
  m_WindowProperties(properties)
{
  if (m_pWindow)
    glfwTerminate();

  /* Initialize the library */
  if (!glfwInit())
  {
    Log::Error("Couldn't initialize GLFWWindow!");
    return;
  }

  /* Create a windowed mode window and its OpenGL context */
  m_pWindow = glfwCreateWindow(m_WindowProperties.Width, m_WindowProperties.Height,
    m_WindowProperties.Title.c_str(), NULL, NULL);

  if (!m_pWindow)
  {
    glfwTerminate();
    Log::Error("GLFWWindow could not be created!");
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_pWindow);
}

unsigned GLFWWindow::GetWidth() const
{
  return m_WindowProperties.Width;
}

unsigned GLFWWindow::GetHeight() const
{
  return m_WindowProperties.Height;
}

void GLFWWindow::OnUpdate()
{
  if (!glfwWindowShouldClose(m_pWindow))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(m_pWindow);

    /* Poll for and process events */
    glfwPollEvents();
  }
}

void GLFWWindow::OnClose()
{
  glfwTerminate();
}
