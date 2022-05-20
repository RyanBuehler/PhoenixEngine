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
  
  void Run() const;

private:
  // Dynamic allocated block
  unique_ptr<Window> m_WindowPtr;
};