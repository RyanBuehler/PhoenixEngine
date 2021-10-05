#pragma once

struct GLFWwindow;
#include "SceneManager.h"
#include "Renderer.h"

class Window
{
  struct WindowProperties
  {
    WindowProperties(const string& title = "Phoenix Engine",
      unsigned width = 1920u,
      unsigned height = 1080u) :
      Title(title),
      Width(width),
      Height(height) {}

    string Title;
    unsigned Width;
    unsigned Height;
  };

public:
  /// <summary>
  /// Window Constructor
  /// </summary>
  /// <param name="properties">The properties of the Window to create</param>
  Window(const WindowProperties& properties = WindowProperties());

  /// <summary>
  /// Get the width of the Window
  /// </summary>
  /// <returns>The width of the Window</returns>
  inline unsigned GetWidth() const noexcept;

  /// <summary>
  /// Get the height of the Window
  /// </summary>
  /// <returns>The height of the Window</returns>
  inline unsigned GetHeight() const noexcept;

  /// <summary>
  /// Called every game loop cycle
  /// </summary>
  void OnUpdate() noexcept;

  /// <summary>
  /// Called when the Window is shut down
  /// </summary>
  void OnClose() noexcept;

  /// <summary>
  /// Interface to GLFW to see if Window was closed
  /// </summary>
  /// <returns>Window should close. T/F</returns>
  bool WindowShouldClose() noexcept;


private:
  GLFWwindow* m_WindowPtr;
  WindowProperties m_WindowProperties;

  unique_ptr<SceneManager> m_SceneManagerPtr;
  unique_ptr<Renderer> m_RendererPtr;

  std::chrono::steady_clock::time_point m_LastFrameTime;
  std::chrono::steady_clock m_Clock;

  bool m_bWindowShouldClose;

  /// <summary>
  /// Window related Input Queries
  /// </summary>
  void OnPollInput(float dt) noexcept;

#pragma region ImGUI

#ifdef _IMGUI
  void OnImGuiCloseWindow() noexcept;
  void OnImGuiChangeScene(SceneManager::Scene scene);

#endif // _IMGUI

#pragma endregion
};