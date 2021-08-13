#pragma once
#include <GLFW/glfw3.h>

//TODO:
#include "VertexArrayObject.h"

class Window
{
  struct WindowProperties
  {
    WindowProperties(const string& title = "Phoenix Engine",
      unsigned width = 1920u,
      unsigned height = 1080u) :
      Title(title),
      Width(width),
      Height(height) {}

    string Title;
    unsigned Width;
    unsigned Height;
  };

public:
  Window(const WindowProperties& properties = WindowProperties());

  inline unsigned GetWidth() const noexcept;
  inline unsigned GetHeight() const noexcept;

  void OnUpdate() noexcept;
  void OnClose() noexcept;

  inline bool WindowShouldClose() noexcept
  {
    return glfwWindowShouldClose(m_pWindow);
  }

private:
  GLFWwindow* m_pWindow;
  WindowProperties m_WindowProperties;

  //TODO:
  unique_ptr<VertexArrayObject> vao;
};