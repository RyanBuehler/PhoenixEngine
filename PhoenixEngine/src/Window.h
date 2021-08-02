#pragma once

struct WindowProperties
{
  WindowProperties(const std::string& title = "Phoenix Engine",
    unsigned width = 1920u,
    unsigned height = 1080u) :
    Title(title),
    Width(width),
    Height(height) {}

  string Title;
  unsigned Width;
  unsigned Height;
};

// Cross Platform Interface for a Window
class Window
{
public:
  Window() {};
  virtual ~Window() {}

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  virtual unsigned GetWidth() const = 0;
  virtual unsigned GetHeight() const = 0;

  virtual void OnUpdate() = 0;
  virtual void OnClose() = 0;

  virtual bool ShouldClose() = 0;
};