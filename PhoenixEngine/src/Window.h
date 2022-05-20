#pragma once
#include "SceneManager.h"
#include "Renderer.h"

struct GLFWwindow;

class Window
{
  struct WindowProperties
  {
    WindowProperties(string Title = "Phoenix Engine - Ryan Buehler",
                              const int Width = 1920u,
                              const int Height = 1080u) :
      Title(std::move(Title)),
      Width(Width),
      Height(Height) {}

    string Title;
    int Width;
    int Height;
  };

public:
  /// <summary>
  /// Window Constructor
  /// </summary>
  /// <param name="Properties">The properties of the Window to create</param>
  Window(WindowProperties Properties = WindowProperties());

  /// <summary>
  /// Get the width of the Window
  /// </summary>
  /// <returns>The width of the Window</returns>
  [[nodiscard]] inline unsigned GetWidth() const noexcept;

  /// <summary>
  /// Get the height of the Window
  /// </summary>
  /// <returns>The height of the Window</returns>
  [[nodiscard]] inline unsigned GetHeight() const noexcept;

  /// <summary>
  /// Called every game loop cycle
  /// </summary>
  void OnUpdate() noexcept;

  /// <summary>
  /// Called when the Window is shut down
  /// </summary>
  void OnClose() const noexcept;

  /// <summary>
  /// Interface to GLFW to see if Window was closed
  /// </summary>
  /// <returns>Window should close. T/F</returns>
  [[nodiscard]] bool WindowShouldClose() const noexcept;


private:
  GLFWwindow* m_WindowPtr;
  WindowProperties m_WindowProperties;

  unique_ptr<SceneManager> m_SceneManagerPtr;
  unique_ptr<Renderer> m_RendererPtr;

  std::chrono::steady_clock::time_point m_LastFrameTime;

  bool m_bWindowShouldClose;

  /// <summary>
  /// Window related Input Queries
  /// </summary>
  void OnPollInput() noexcept;

#pragma region ImGUI

#ifdef _IMGUI
  void OnImGuiCloseWindow() noexcept;
  void OnImGuiChangeScene(SceneManager::Scene Scene) const;

#endif // _IMGUI

#pragma endregion
};