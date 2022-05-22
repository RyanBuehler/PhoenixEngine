//------------------------------------------------------------------------------
// File:    Application.cpp
// Author:  Ryan Buehler
// Created: Friday, May 20, 2022
// Desc:    The entry point of the Phoenix Engine application
//------------------------------------------------------------------------------
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
  Log::Trace("Application terminated.");
}

int main()
{
  const auto app = new Application();
  app->Run();
  delete app;
}
