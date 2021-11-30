//------------------------------------------------------------------------------
// File:    CameraManager.cpp
// Author:  Ryan Buehler
// Created: September 4, 2021
// Desc:    The CameraManager class creates and contains all of the engine's
//          cameras, also allowing for functionality to switch between them
//------------------------------------------------------------------------------
#include "pch.h"
#include "CameraManager.h"

CameraManager::CameraManager() noexcept :
  m_DefaultCamera(),
  m_Cameras(),
  m_ActiveCamera(&m_DefaultCamera)
{
  SetActiveCamera(m_DefaultCamera);
  Log::Trace("CameraManager and Default Camera created for Scene.");
}

CameraManager::~CameraManager()
{
  m_ActiveCamera = nullptr;
  Log::Trace("CameraManager destroyed.");
}

Camera& CameraManager::CreateCamera(const string& name) noexcept
{
  m_Cameras.push_front(make_unique<Camera>(name));
  return *m_Cameras.front();
}

void CameraManager::DeleteCameraByName(const string& name) noexcept
{
  for (const auto& camera : m_Cameras)
  {
    if (camera->GetName() == name)
    {
      m_Cameras.remove(camera);
      Log::Trace("Deleted camera by name: " + name + ".");
      return;
    }
  }

  Log::Error("Tried to delete camera by name: " + name + ", but such a camera doesn't exist.");
}

void CameraManager::SetActiveCamera(Camera& activeCamera) noexcept
{
  m_ActiveCamera = &activeCamera;
}

Camera& CameraManager::GetActiveCamera() noexcept
{
  return *m_ActiveCamera;
}

Camera& CameraManager::GetDefaultCamera() noexcept
{
  return m_DefaultCamera;
}
