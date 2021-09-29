//------------------------------------------------------------------------------
// File:    GameObject.h
// Author:  Ryan Buehler
// Created: 09/26/21
// Desc:    The universal object to display in engine
//------------------------------------------------------------------------------
#pragma once
#include "Transform.h"

class IMesh;

class GameObject
{
public:

  GameObject() noexcept;
  ~GameObject() = default;
  GameObject(const GameObject&) = delete;
  GameObject& operator=(const GameObject&) = delete;
  GameObject(GameObject&&) = delete;
  GameObject& operator=(GameObject&&) = delete;

  const Transform& GetTransform() const noexcept;
  const IMesh& GetMesh() const noexcept;

private:
  Transform m_Transform;

  unique_ptr<IMesh> m_MeshPtr;
};
