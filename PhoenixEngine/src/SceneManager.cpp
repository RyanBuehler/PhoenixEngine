//------------------------------------------------------------------------------
// File:    SceneManager.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    Handles the Scenes and Scene Transitions
//------------------------------------------------------------------------------
#include "pch.h"
#include "SceneManager.h"
#include "Scenes/SceneDemo.h"
#include "Scenes/SceneSingleObject.h"

SceneManager::SceneManager() noexcept :
  m_ReloadEnabled(false),
  m_NextScene(DEFAULT_SCENE)
{
}

bool SceneManager::SceneIsTransitioning() noexcept
{
  if (m_ReloadEnabled)
  {
    assert(m_CurrentScenePtr.get());

    // Reload the Scene
    reloadScene();

    return true;
  }

  if (m_NextScene != Scene::NONE)
  {
    // Transition to the next scene
    transitionScene();
    return true;
  }

  return false;
}

void SceneManager::SetNewScene(const Scene NextScene) noexcept
{
  assert(NextScene < Scene::COUNT);
  if (m_NextScene == NextScene)
  {
    ReloadScene();
    return;
  }
  m_NextScene = NextScene;
}

void SceneManager::ReloadScene() noexcept
{
  m_ReloadEnabled = true;
}

void SceneManager::Shutdown() const noexcept
{
  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnShutdown();
  m_CurrentScenePtr->OnUnload();
}

void SceneManager::OnUpdate(const float DeltaTime) const noexcept
{
  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnUpdate(DeltaTime);
}

void SceneManager::OnPollInput(GLFWwindow* WindowPtr, const float DeltaTime) const noexcept
{
  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnPollInput(WindowPtr, DeltaTime);
}

vector<GameObject>& SceneManager::GetCurrentSceneGameObjects() const noexcept
{
  assert(m_CurrentScenePtr);
  return m_CurrentScenePtr->GetGameObjectArray();
}

Camera& SceneManager::GetCurrentSceneActiveCamera() const noexcept
{
  assert(m_CurrentScenePtr);
  return m_CurrentScenePtr->GetCurrentCamera();
}

void SceneManager::transitionScene() noexcept
{
  if (m_CurrentScenePtr != nullptr)
  {
    m_CurrentScenePtr->OnShutdown();
    m_CurrentScenePtr->OnUnload();
  }

  switch (m_NextScene)
  {
  case Scene::NONE:
  case Scene::DEMO:
    m_CurrentScenePtr = make_unique<SceneDemo>();
    break;
  case Scene::SINGLE_OBJECT:
    m_CurrentScenePtr = make_unique<SceneSingleObject>();
    break;
  case Scene::COUNT:
    Log::error("Invalid Scene Transition");
    return;
  }

  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnLoad();
  m_CurrentScenePtr->OnInit();

  // Reset the transition Scene
  m_NextScene = Scene::NONE;
}

void SceneManager::reloadScene() noexcept
{
  assert(m_CurrentScenePtr);
  m_ReloadEnabled = false;
  m_CurrentScenePtr->OnShutdown();
  m_CurrentScenePtr->OnInit();
}
