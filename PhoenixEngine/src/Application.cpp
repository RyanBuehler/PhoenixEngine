#include "pch.h"
#include "Application.h"
#include "Logger.h"
#include "Window.h"

Application::Application() noexcept :
  m_WindowPtr(make_unique<Window>())
{}

void Application::Run() const
{
  while (!m_WindowPtr->WindowShouldClose())
  {
    m_WindowPtr->OnUpdate();
  }

  m_WindowPtr->OnClose();
  Log::trace("Application terminated.");
}

int main()
{
  const auto app = new Application();
  app->Run();
  delete app;
}
