//------------------------------------------------------------------------------
// File:    SceneManager.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    Handles the Scenes and Scene Transitions
//------------------------------------------------------------------------------
#include "pch.h"
#include "SceneManager.h"
#include "TestScene.h"

SceneManager::SceneManager() noexcept :
  m_TransitionEnabled(true),
  m_ReloadEnabled(false),
  m_CurrentScenePtr(),
  m_NextScene(DEFAULTSCENE)
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

  if (m_TransitionEnabled)
  {
    // Set the next Scene to current Scene
    transitionScene(m_NextScene);

    // Reset the transition Scene
    m_NextScene = DEFAULTSCENE;

    return true;
  }

  return false;
}

void SceneManager::SetNewScene(Scene nextScene) noexcept
{
  assert(nextScene < Scene::SceneCount);
  m_NextScene = nextScene;
}

void SceneManager::ReloadScene() noexcept
{
  m_ReloadEnabled = true;
}

void SceneManager::Shutdown() noexcept
{
  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnShutdown();
  m_CurrentScenePtr->OnUnload();
}

void SceneManager::OnUpdate(float dt) noexcept
{
  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnUpdate(dt);
}

void SceneManager::OnPollInput(GLFWwindow* windowPtr, float dt) noexcept
{
  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnPollInput(windowPtr, dt);
}

vector<GameObject>& SceneManager::GetCurrentSceneGameObjects() noexcept
{
  assert(m_CurrentScenePtr);
  return m_CurrentScenePtr->GetGameObjectArray();
}

Camera& SceneManager::GetCurrentSceneActiveCamera() noexcept
{
  assert(m_CurrentScenePtr);
  return m_CurrentScenePtr->GetCurrentCamera();
}

void SceneManager::transitionScene(Scene scene) noexcept
{
  m_TransitionEnabled = false;

  if (m_CurrentScenePtr.get() != nullptr)
  {
    m_CurrentScenePtr->OnShutdown();
    m_CurrentScenePtr->OnUnload();
  }

  switch (scene)
  {
  case SceneManager::Scene::TestScene:
    m_CurrentScenePtr.release();
    m_CurrentScenePtr = make_unique<TestScene>();
    break;
  case SceneManager::Scene::SceneCount:
  default:
    Log::Error("Invalid Scene Transition");
    return;
  }

  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnLoad();
  m_CurrentScenePtr->OnInit();
}

void SceneManager::reloadScene() noexcept
{
  assert(m_CurrentScenePtr);
  m_ReloadEnabled = false;
  m_CurrentScenePtr->OnShutdown();
  m_CurrentScenePtr->OnInit();
}
