//------------------------------------------------------------------------------
// File:    SceneManager.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    Handles the Scenes and Scene Transitions
//------------------------------------------------------------------------------
#pragma once
#include "IScene.h"
#include "GameObject.h"
#include "Camera.h"

class SceneManager
{
public:
  /// <summary>
  /// Hardcoded Scenes available
  /// </summary>
  enum class Scene
  {
    None,
    Scene1, // Spheres orbiting a centerpiece
    Scene2, // Phong Lighting scene
    ReflectionScene, // Reflection Scene
    SceneSingleObject, // Single object for testing
    SceneCount
  };
private:
  static constexpr Scene DEFAULTSCENE = Scene::ReflectionScene;

public:
  /// <summary>
  /// SceneManager's Default Constructor
  /// </summary>
  /// <returns></returns>
  SceneManager() noexcept;
  ~SceneManager() = default;
  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;
  SceneManager(SceneManager&&) = delete;
  SceneManager& operator=(SceneManager&&) = delete;

  /// <summary>
  /// Check to see if the Scene should transition
  /// </summary>
  /// <returns>A Scene transition happened. T/F</returns>
  bool SceneIsTransitioning() noexcept;

  /// <summary>
  /// Sets the next Scene, instigating a transition
  /// </summary>
  /// <param name="nextScene"></param>
  void SetNewScene(Scene nextScene) noexcept;

  /// <summary>
  /// Sets the Scene to Reload on next cycle
  /// </summary>
  /// <returns></returns>
  void ReloadScene() noexcept;

  /// <summary>
  /// Shuts the SceneManager down
  /// </summary>
  /// <returns></returns>
  void Shutdown() noexcept;

  /// <summary>
  /// Called every game loop cycle
  /// </summary>
  void OnUpdate(float dt) noexcept;

  /// <summary>
  /// Called after polling for input
  /// </summary>
  void OnPollInput(GLFWwindow* windowPtr, float dt) noexcept;

  /// <summary>
  /// Retrieves a vector of the current scene's game objects
  /// </summary>
  /// <returns>A vector reference of the current scene's game objects</returns>
  vector<GameObject>& GetCurrentSceneGameObjects() noexcept;
  Camera& GetCurrentSceneActiveCamera() noexcept;

private:
  bool m_ReloadEnabled;                  // Reloading scene next iteration
  unique_ptr<IScene> m_CurrentScenePtr;  // Pointer to the current scene
  Scene m_CurrentScene;                  // The current scene
  Scene m_NextScene;                     // The scene to transition to

  /// <summary>
  /// Private helper to transition the Scene
  /// </summary>
  /// <param name="scene">The Scene to transition to</param>
  void transitionScene() noexcept;

  /// <summary>
  /// Private helper to reload the Scene
  /// </summary>
  void reloadScene() noexcept;
};
