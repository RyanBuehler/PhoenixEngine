//------------------------------------------------------------------------------
// File:    CameraManager.h
// Author:  Ryan Buehler
// Created: September 4, 2021
// Desc:    The CameraManager class declarations
//------------------------------------------------------------------------------
#pragma once
#include "Camera.h"

class CameraManager
{
public:

  CameraManager() noexcept;
  ~CameraManager();
  CameraManager(const CameraManager&) = delete;
  CameraManager& operator=(const CameraManager&) = delete;
  CameraManager(CameraManager&&) = delete;
  CameraManager& operator=(CameraManager&&) = delete;

  Camera& CreateCamera(const string& name) noexcept;
  //TODO: Delete Camera by Name

  void SetActiveCamera(Camera& activeCamera) noexcept;
  Camera& GetActiveCamera() noexcept;
  Camera& GetDefaultCamera() noexcept;

private:
  Camera m_DefaultCamera;
  vector<unique_ptr<Camera>> m_Cameras;

  Camera* m_ActiveCamera;
};

