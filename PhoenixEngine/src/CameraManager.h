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

  Camera& CreateCamera(const string& Name) noexcept;
  void DeleteCameraByName(const string& Name) noexcept;

  void SetActiveCamera(Camera& ActiveCamera) noexcept;
  Camera& GetActiveCamera() const noexcept;
  Camera& GetDefaultCamera() noexcept;

private:
  Camera m_DefaultCamera;
  forward_list<unique_ptr<Camera>> m_Cameras;

  Camera* m_ActiveCamera;
};

