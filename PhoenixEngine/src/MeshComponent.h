//------------------------------------------------------------------------------
// File:    MeshComponent.h
// Author:  Ryan Buehler
// Created: May 04, 2022
// Desc:    Renderable Mesh Component for game objects
//------------------------------------------------------------------------------
#pragma once
#include "Component.h"
#include "Material.h"

class MeshComponent : public Component
{
  public:

  MeshComponent(GameObject& parent) noexcept;
  ~MeshComponent() = default;
  MeshComponent(const MeshComponent&) = delete;
  MeshComponent& operator=(const MeshComponent&) = delete;
  MeshComponent(MeshComponent&&) = delete;
  MeshComponent& operator=(MeshComponent&&) = delete;

  virtual Component::Type GetType() const noexcept { return Component::Type::MESH; }

  void SetMeshFileName(const string& fileName) noexcept;
  inline const string& GetMeshFileName() const noexcept { return m_MeshFileName; }

  inline void SetMaterial(const Material& material) noexcept { m_Material = material; }

  private:
  unsigned m_MeshID;
  string m_MeshFileName;
  Material m_Material;
};
