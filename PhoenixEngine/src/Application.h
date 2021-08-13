#pragma once

class Window;

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
  // Dynamic allocated block
  unique_ptr<Window> m_Window;
};

int main()
{
  Application* app = new Application();
  app->Run();
  delete app;
}