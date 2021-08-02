#include "pch.h"
#include "GLEW/glew.h"
#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(const WindowProperties& properties) :
  m_pWindow(nullptr),
  m_WindowProperties(properties)
{
  if (m_pWindow)
    glfwTerminate();

  /* Initialize the library */
  if (!glfwInit())
  {
    Log::Error("Couldn't initialize OpenGL Window!");
    return;
  }

  /* Create a windowed mode window and its OpenGL context */
  m_pWindow = glfwCreateWindow(m_WindowProperties.Width, m_WindowProperties.Height,
    m_WindowProperties.Title.c_str(), NULL, NULL);

  if (!m_pWindow)
  {
    glfwTerminate();
    Log::Error("OpenGL Window could not be created!");
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_pWindow);

  if (glewInit() != GLEW_OK)
  {
    Log::Error("Couldn't initialize GLEW!");
    return;
  };
}

unsigned OpenGLWindow::GetWidth() const
{
  return m_WindowProperties.Width;
}

unsigned OpenGLWindow::GetHeight() const
{
  return m_WindowProperties.Height;
}

void OpenGLWindow::OnUpdate()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_pWindow);

    /* Poll for and process events */
    glfwPollEvents();
}

void OpenGLWindow::OnClose()
{
  glfwTerminate();
}

inline bool OpenGLWindow::ShouldClose()
{
  return glfwWindowShouldClose(m_pWindow);
}
