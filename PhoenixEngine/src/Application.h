#pragma once
#include "Window.h"

#ifdef PE_BUILD_WINDOWS
#include "GLFWWindow.h"
#endif

class Application
{
public:

  Application() noexcept;
  ~Application() = default;
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application&&) = delete;
  
  void Run();

private:
#ifdef PE_BUILD_WINDOWS
  // For Windows Build
  unique_ptr<GLFWWindow> m_Window;
#else
  // For Linux/Mac Build
  unique_ptr<GLFWWindow> m_Window;
#endif
};

#ifdef PE_BUILD_WINDOWS

int main()
{
  Application* app = new Application();
  app->Run();
  delete app;
}

#endif // PE_BUILD_WINDOWS