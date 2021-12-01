//------------------------------------------------------------------------------
// File:    SceneManager.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    Handles the Scenes and Scene Transitions
//------------------------------------------------------------------------------
#include "pch.h"
#include "SceneManager.h"
#include "Scene1.h"
#include "Scene2.h"
#include "ReflectionScene.h"
#include "SceneSingleObject.h"

SceneManager::SceneManager() noexcept :
  m_ReloadEnabled(false),
  m_CurrentScenePtr(),
  m_CurrentScene(Scene::None),
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

  if (m_NextScene != Scene::None)
  {
    // Transition to the next scene
    transitionScene();
    return true;
  }

  return false;
}

void SceneManager::SetNewScene(Scene nextScene) noexcept
{
  assert(nextScene < Scene::SceneCount);
  if (m_NextScene == nextScene)
  {
    ReloadScene();
    return;
  }
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

void SceneManager::transitionScene() noexcept
{
  if (m_CurrentScenePtr.get() != nullptr)
  {
    m_CurrentScenePtr->OnShutdown();
    m_CurrentScenePtr->OnUnload();
  }

  switch (m_NextScene)
  {
  case SceneManager::Scene::None:
  case SceneManager::Scene::Scene1:
    m_CurrentScenePtr.release();
    m_CurrentScenePtr = make_unique<Scene1>();
    break;
  case SceneManager::Scene::Scene2:
    m_CurrentScenePtr.release();
    m_CurrentScenePtr = make_unique<Scene2>();
    break;
  case SceneManager::Scene::ReflectionScene:
    m_CurrentScenePtr.release();
    m_CurrentScenePtr = make_unique<ReflectionScene>();
    break;
  case SceneManager::Scene::SceneSingleObject:
    m_CurrentScenePtr.release();
    m_CurrentScenePtr = make_unique<SceneSingleObject>();
    break;
  case SceneManager::Scene::SceneCount:
  default:
    Log::Error("Invalid Scene Transition");
    return;
  }

  assert(m_CurrentScenePtr);
  m_CurrentScenePtr->OnLoad();
  m_CurrentScenePtr->OnInit();

  // Reset the transition Scene
  m_NextScene = Scene::None;
}

void SceneManager::reloadScene() noexcept
{
  assert(m_CurrentScenePtr);
  m_ReloadEnabled = false;
  m_CurrentScenePtr->OnShutdown();
  m_CurrentScenePtr->OnInit();
}
