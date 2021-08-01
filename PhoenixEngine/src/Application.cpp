#include "pch.h"
#include "Application.h"
#include "Logger.h"

shared_ptr<Logger> Log;

Application::Application() noexcept
{
  Log = make_shared<Logger>();
  Log->Trace("Start");
}

void Application::Run()
{
  Log->Trace("Run");
  while (true);
  Log->Trace("Terminate");
}
