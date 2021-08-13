#include "pch.h"
#include "Application.h"
#include "Logger.h"
#include "Window.h"

Application::Application() noexcept :
  m_Window(make_unique<Window>())
{
  Log::Trace("Start");
}

void Application::Run()
{
  Log::Trace("Run");
  while (!m_Window->WindowShouldClose())
  {
    m_Window->OnUpdate();
  }

  m_Window->OnClose();
  Log::Trace("Terminate");
}
