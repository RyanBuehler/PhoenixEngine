#pragma once
#include "Window.h"

#include <GLFW/glfw3.h>

class GLFWWindow : public Window
{
public:
  GLFWWindow(const WindowProperties& properties = WindowProperties());

  inline unsigned GetWidth() const;
  inline unsigned GetHeight() const;

  void OnUpdate();
  void OnClose();

private:
  GLFWwindow* m_pWindow;
  WindowProperties m_WindowProperties;
};