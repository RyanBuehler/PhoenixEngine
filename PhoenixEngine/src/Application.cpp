#include "pch.h"
#include "Application.h"
#include "Logger.h"

Application::Application() noexcept :
#ifdef PE_BUILD_WINDOWS
  // Build for Windows
  m_Window(make_unique<GLFWWindow>())
#else
  // Build for Linux/Mac
  m_Window(make_unique<GLFWWindow>())
#endif
{
  //Log = make_unique<Logger>();
  Log::Trace("Start");
}

void Application::Run()
{
  Log::Trace("Run");
  while (true)
  {
    m_Window->OnUpdate();
  }

  m_Window->OnClose();
  Log::Trace("Terminate");
}
