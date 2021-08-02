#pragma once
#include "Window.h"

#include <GLFW/glfw3.h>

class OpenGLWindow : public Window
{
public:
  OpenGLWindow(const WindowProperties& properties = WindowProperties());

  inline unsigned GetWidth() const;
  inline unsigned GetHeight() const;

  void OnUpdate();
  void OnClose();

  inline bool ShouldClose();

private:
  GLFWwindow* m_pWindow;
  WindowProperties m_WindowProperties;
};