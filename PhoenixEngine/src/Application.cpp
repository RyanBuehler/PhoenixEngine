#include "pch.h"
#include "Application.h"
#include "Logger.h"
#include "Window.h"

Application::Application() noexcept :
  m_Window(make_unique<Window>())
{}

void Application::Run()
{
  while (!m_Window->WindowShouldClose())
  {
    m_Window->OnUpdate();
  }

  m_Window->OnClose();
  Log::Trace("Application terminated.");
}
