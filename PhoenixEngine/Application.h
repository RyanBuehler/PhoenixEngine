#pragma once

#ifdef PE_BUILD_WINDOWS

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

};

int main()
{
  Application* app = new Application();
  app->Run();
  delete app;
}

#endif // PE_BUILD_WINDOWS